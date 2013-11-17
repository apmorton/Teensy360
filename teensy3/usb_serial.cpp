#include "usb_serial.h"
#include "core_pins.h"

uint32_t usb_cdc_line_coding[2];
volatile uint8_t usb_cdc_line_rtsdtr=0;
uint32_t usb_cdc2_line_coding[2];
volatile uint8_t usb_cdc2_line_rtsdtr=0;
volatile uint8_t usb_cdc_transmit_flush_timer=0;
volatile uint8_t usb_cdc2_transmit_flush_timer=0;

int usb_serial_class::available() {
	int count;
	count = usb_rx_byte_count(rx_endpoint);
	if (rx_packet) count += rx_packet->len - rx_packet->index;
	return count;
}

int usb_serial_class::peek() {
	if (!rx_packet) {
		if (!usb_configuration) return -1;
		rx_packet = usb_rx(rx_endpoint);
		if (!rx_packet) return -1;
	}
	if (!rx_packet) return -1;
	return rx_packet->buf[rx_packet->index];
}

int usb_serial_class::read() {
	unsigned int i;
	int c;

	if (!rx_packet) {
		if (!usb_configuration) return -1;
		rx_packet = usb_rx(rx_endpoint);
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

void usb_serial_class::flush() {
	if (!usb_configuration) return;
	tx_noautoflush = 1;
	if (tx_packet) {
		*flush_timer = 0;
		tx_packet->len = tx_packet->index;
		usb_tx(tx_endpoint, tx_packet);
		tx_packet = NULL;
	} else {
		usb_packet_t *tx = usb_malloc();
		if (tx) {
			*flush_timer = 0;
			usb_tx(tx_endpoint, tx);
		} else {
			*flush_timer = 1;
		}
	}
	tx_noautoflush = 0;
}

#define TRANSMIT_FLUSH_TIMEOUT	5   /* in milliseconds */

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

size_t usb_serial_class::write(const uint8_t *buffer, size_t size) {
	uint32_t len;
	uint32_t wait_count;
	const uint8_t *src = (const uint8_t *)buffer;
	uint8_t *dest;

	tx_noautoflush = 1;
	while (size > 0) {
		if (!tx_packet) {
			wait_count = 0;
			while (1) {
				if (!usb_configuration) {
					tx_noautoflush = 0;
					return -1;
				}
				if (usb_tx_packet_count(tx_endpoint) < TX_PACKET_LIMIT) {
					tx_noautoflush = 1;
					tx_packet = usb_malloc();
					if (tx_packet) break;
					tx_noautoflush = 0;
				}
				if (++wait_count > TX_TIMEOUT || transmit_previous_timeout) {
					transmit_previous_timeout = 1;
					return -1;
				}
				yield();
			}
		}
		transmit_previous_timeout = 0;
		len = tx_size - tx_packet->index;
		if (len > size) len = size;
		dest = tx_packet->buf + tx_packet->index;
		tx_packet->index += len;
		size -= len;
		while (len-- > 0) *dest++ = *src++;
		if (tx_packet->index >= tx_size) {
			tx_packet->len = tx_size;
			usb_tx(tx_endpoint, tx_packet);
			tx_packet = NULL;
		}
		*flush_timer = TRANSMIT_FLUSH_TIMEOUT;
	}
	tx_noautoflush = 0;
	return 0;
}

int usb_serial_class::usb_read(void *buffer, uint32_t size)
{
	uint8_t *p = (uint8_t *)buffer;
	uint32_t qty, count=0;

	while (size) {
		if (!usb_configuration) break;
		if (!rx_packet) {
			rx:
			rx_packet = usb_rx(rx_endpoint);
			if (!rx_packet) break;
			if (rx_packet->len == 0) {
				usb_free(rx_packet);
				goto rx;
			}
		}
		qty = rx_packet->len - rx_packet->index;
		if (qty > size) qty = size;
		memcpy(p, rx_packet->buf + rx_packet->index, qty);
		p += qty;
		count += qty;
		size -= qty;
		rx_packet->index += qty;
		if (rx_packet->index >= rx_packet->len) {
			usb_free(rx_packet);
			rx_packet = NULL;
		}
	}
	return count;
}

void usb_serial_class::usb_flush_cb() {
	if (tx_noautoflush) return;
	if (tx_packet) {
		tx_packet->len = tx_packet->index;
		usb_tx(tx_endpoint, tx_packet);
		tx_packet = NULL;
	} else {
		usb_packet_t *tx = usb_malloc();
		if (tx) {
			usb_tx(tx_endpoint, tx);
		} else {
			*flush_timer = 1;
		}
	}
}

void usb_serial_flush_callback(int i)
{
	if (i == 0)
		UsbSerial1.usb_flush_cb();
	else
		UsbSerial2.usb_flush_cb();
}

