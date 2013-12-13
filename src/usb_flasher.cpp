#include <WProgram.h>

#include "XSPI.h"
#include "usb_flasher.h"
#include "usb_dev.h"
#include "config.h"

static uint8_t version[4] = {FLASHER_ARM_VERSION, 0, 0, 0};

void NandFlasher::handleDataInit() {
	uint8_t flashConfig[4] = {0};
	XSPI.enterFlashMode();
	XSPI.readRegister(0x00, flashConfig);
	XSPI.readRegister(0x00, flashConfig);
	write(flashConfig, 4);
	flush();
}

void NandFlasher::handleDataDeInit() {
	XSPI.leaveFlashMode();
}

void NandFlasher::handleDataStatus() {
	uint8_t data[4] = {(uint8_t)(status >> 8), (uint8_t)(status & 0xFF), 0, 0};
	write(data, 4);
	flush();
}

void NandFlasher::handleDataRead() {
	uint32_t length = argB / 4;
	
	while (length) {
	
	}
}

void NandFlasher::handleDataWrite() {

}

void NandFlasher::handleDataErase() {

}

void NandFlasher::handleXboxPwrOn() {
	XSPI.leaveFlashMode();
	XSPI.powerOnXbox();
}

void NandFlasher::handleXboxPwrOff() {
	XSPI.leaveFlashMode();
	XSPI.powerOffXbox();
}

void NandFlasher::handleDevVersion() {
	write(version, 4);
	flush();
}

int NandFlasher::vendorCallback(uint8_t bRequest, uint16_t wLength) {
	if (commandPending || commandReady || wLength != 8) {
		commandPending = false;
		commandReady = false;
		return 0;
	}

	switch (bRequest) {
		case CMD_DATA_READ:
		case CMD_DATA_WRITE:
		case CMD_DATA_INIT:
		case CMD_DATA_DEINIT:
		case CMD_DATA_STATUS:
		case CMD_DATA_ERASE:
		case CMD_DEV_VERSION:
		case CMD_XBOX_PWRON:
		case CMD_XBOX_PWROFF:
		case CMD_DEV_UPDATE:
			commandPending = true;
			commandReady = false;
			commandCode = bRequest;

			return 1;
		default:
			return 0;
	}
}

void NandFlasher::vendorDataCallback(uint8_t bRequest, uint32_t argA, uint32_t argB) {
	if (!commandPending || bRequest != commandCode || commandReady) {
		commandCode = false;
		commandReady = false;
		return;
	}

	commandPending = false;
	commandReady = true;
	commandCode = bRequest;
	this->argA = argA;
	this->argB = argB;
}

void NandFlasher::runReadyCommand() {
	if (!commandReady) return;

	UsbSerial1.print("CMD: ");
	UsbSerial1.print(commandCode, HEX);
	UsbSerial1.print(" argA: ");
	UsbSerial1.print(argA, HEX);
	UsbSerial1.print(" argB: ");
	UsbSerial1.print(argB, HEX);
	UsbSerial1.println();
	
	switch (commandCode) {
		case CMD_DATA_READ:
			handleDataRead();
			break;
		case CMD_DATA_WRITE:
			handleDataWrite();
			break;
		case CMD_DATA_INIT:
			handleDataInit();
			break;
		case CMD_DATA_DEINIT:
			handleDataDeInit();
			break;
		case CMD_DATA_STATUS:
			handleDataStatus();
			break;
		case CMD_DATA_ERASE:
			handleDataErase();
			break;
		case CMD_DEV_VERSION:
			handleDevVersion();
			break;
		case CMD_XBOX_PWRON:
			handleXboxPwrOn();
			break;
		case CMD_XBOX_PWROFF:
			handleXboxPwrOff();
			break;
		case CMD_DEV_UPDATE:
			_reboot_Teensyduino_();
			break;
	}
	
	commandPending = false;
	commandReady = false;
	commandCode = 0;
}

// Maximum number of transmit packets to queue so we don't starve other endpoints for memory
#define TX_PACKET_LIMIT 8

// When the PC isn't listening, how long do we wait before discarding data?  If this is
// too short, we risk losing data during the stalls that are common with ordinary desktop
// software.  If it's too long, we stall the user's program when no software is running.
#define TX_TIMEOUT_MSEC 70

#if F_CPU == 96000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 596)
#elif F_CPU == 48000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 428)
#elif F_CPU == 24000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 262)
#endif

size_t NandFlasher::write(const uint8_t *buffer, size_t size) {
	uint32_t len;
	uint32_t wait_count;
	const uint8_t *src = (const uint8_t *)buffer;
	uint8_t *dest;

	while (size > 0) {
		if (!tx_packet) {
			wait_count = 0;
			while (1) {
				if (!usb_configuration) {
					return -1;
				}
				if (usb_tx_packet_count(FLASHER_TX_ENDPOINT) < TX_PACKET_LIMIT) {
					tx_packet = usb_malloc();
					if (tx_packet) break;
				}
				if (++wait_count > TX_TIMEOUT || transmit_previous_timeout) {
					transmit_previous_timeout = 1;
					return -1;
				}
				yield();
			}
		}
		transmit_previous_timeout = 0;
		len = FLASHER_TX_SIZE - tx_packet->index;
		if (len > size) len = size;
		dest = tx_packet->buf + tx_packet->index;
		tx_packet->index += len;
		size -= len;
		while (len-- > 0) *dest++ = *src++;
		if (tx_packet->index >= FLASHER_TX_SIZE) {
			tx_packet->len = FLASHER_TX_SIZE;
			usb_tx(FLASHER_TX_ENDPOINT, tx_packet);
			tx_packet = NULL;
		}
	}
	return 0;
}

int NandFlasher::available() {
	int count;
	count = usb_rx_byte_count(FLASHER_RX_ENDPOINT);
	if (rx_packet) count += rx_packet->len - rx_packet->index;
	return count;
}

int NandFlasher::peek() {
	if (!rx_packet) {
		if (!usb_configuration) return -1;
		rx_packet = usb_rx(FLASHER_RX_ENDPOINT);
		if (!rx_packet) return -1;
	}
	if (!rx_packet) return -1;
	return rx_packet->buf[rx_packet->index];
}

int NandFlasher::read() {
	unsigned int i;
	int c;

	if (!rx_packet) {
		if (!usb_configuration) return -1;
		rx_packet = usb_rx(FLASHER_RX_ENDPOINT);
		if (!rx_packet) return -1;
	}
	i = rx_packet->index;
	c = rx_packet->buf[i++];
	if (i >= rx_packet->len) {
		usb_free(rx_packet);
		rx_packet = NULL;
	} else {
		rx_packet->index = i;
	}
	return c;
}

void NandFlasher::flush() {
	if (!usb_configuration) return;
	if (tx_packet) {
		tx_packet->len = tx_packet->index;
		usb_tx(FLASHER_TX_ENDPOINT, tx_packet);
		tx_packet = NULL;
	} else {
		usb_packet_t *tx = usb_malloc();
		if (tx) {
			usb_tx(FLASHER_TX_ENDPOINT, tx);
		}
	}
}


NandFlasher Flasher;

extern "C" {

	int usb_vendor_callback(union _setup *request, uint8_t *data) {
		return Flasher.vendorCallback(request->bRequest, request->wLength);
	}

	void usb_vendor_data_callback(union _setup *request, uint8_t *data) {
		uint32_t argA = *(uint32_t *)(data);
		uint32_t argB = *(uint32_t *)(data + 4);

		Flasher.vendorDataCallback(request->bRequest, argA, argB);
	}

};
