#include <WProgram.h>

#include "XSPI.h"
#include "usb_flasher.h"
#include "usb_dev.h"

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
			
			break;
		case CMD_DATA_WRITE:
			
			break;
		case CMD_DATA_INIT:
			
			break;
		case CMD_DATA_DEINIT:
			
			break;
		case CMD_DATA_STATUS:
			
			break;
		case CMD_DATA_ERASE:
			
			break;
		case CMD_DEV_VERSION:
			
			break;
		case CMD_XBOX_PWRON:
			XSPI.leaveFlashMode();
			XSPI.powerOnXbox();
			break;
		case CMD_XBOX_PWROFF:
			XSPI.leaveFlashMode();
			XSPI.powerOffXbox();
			break;
		case CMD_DEV_UPDATE:
			_reboot_Teensyduino_();
			break;
	}
	
	commandPending = false;
	commandReady = false;
	commandCode = 0;
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
