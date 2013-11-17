/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2013 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be 
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows 
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef USBserial_h_
#define USBserial_h_

#if defined(USB_SERIAL) || defined(USB_SERIAL_HID)

#include <inttypes.h>
#include "usb_dev.h"

// C language implementation
#ifdef __cplusplus
extern "C" {
#endif
extern volatile uint32_t systick_millis_count;

extern uint32_t usb_cdc_line_coding[2];
extern volatile uint8_t usb_cdc_line_rtsdtr;
extern uint32_t usb_cdc2_line_coding[2];
extern volatile uint8_t usb_cdc2_line_rtsdtr;
extern volatile uint8_t usb_cdc_transmit_flush_timer;
extern volatile uint8_t usb_cdc2_transmit_flush_timer;
extern volatile uint8_t usb_configuration;
#ifdef __cplusplus
}
#endif

#define USB_SERIAL_DTR  0x01
#define USB_SERIAL_RTS  0x02


// C++ interface
#ifdef __cplusplus
#include "Stream.h"

class usb_serial_class : public Stream {
    volatile uint8_t *line_rtsdtr;
    uint32_t *line_coding;
    volatile uint8_t *flush_timer;
    uint8_t rx_endpoint;
    uint8_t tx_endpoint;
    size_t tx_size;
    usb_packet_t *rx_packet=NULL;
    usb_packet_t *tx_packet=NULL;
    volatile uint8_t tx_noautoflush=0;
    uint8_t transmit_previous_timeout=0;
    uint8_t N;
public:
    usb_serial_class(int number) {
        if (number == 0) {
            line_rtsdtr = &usb_cdc_line_rtsdtr;
            line_coding = (uint32_t *)&usb_cdc_line_coding;
            flush_timer = &usb_cdc_transmit_flush_timer;
            rx_endpoint = CDC_RX_ENDPOINT;
            tx_endpoint = CDC_TX_ENDPOINT;
            tx_size = CDC_TX_SIZE;
        } else {
            line_rtsdtr = &usb_cdc2_line_rtsdtr;
            line_coding = (uint32_t *)&usb_cdc2_line_coding;
            flush_timer = &usb_cdc2_transmit_flush_timer;
            rx_endpoint = CDC2_RX_ENDPOINT;
            tx_endpoint = CDC2_TX_ENDPOINT;
            tx_size = CDC2_TX_SIZE;
        }
    }
    void begin(long) { /* TODO: call a function that tries to wait for enumeration */ };
    void end() { /* TODO: flush output and shut down USB port */ };
    size_t write(unsigned long n) { return write((uint8_t)n); }
    size_t write(long n) { return write((uint8_t)n); }
    size_t write(unsigned int n) { return write((uint8_t)n); }
    size_t write(int n) { return write((uint8_t)n); }

    virtual int available();
    virtual int read();
    virtual int peek();
    virtual void flush();
    virtual size_t write(uint8_t c) { return write(&c, 1); }
    virtual size_t write(const uint8_t *buffer, size_t size);
    uint32_t baud(void) { return line_coding[0]; }
    uint8_t stopbits(void) { uint8_t b = line_coding[1]; if (!b) b = 1; return b; }
    uint8_t paritytype(void) { return line_coding[1] >> 8; } // 0=none, 1=odd, 2=even
    uint8_t numbits(void) { return line_coding[1] >> 16; }
    uint8_t dtr(void) { return (*line_rtsdtr & USB_SERIAL_DTR) ? 1 : 0; }
    uint8_t rts(void) { return (*line_rtsdtr & USB_SERIAL_RTS) ? 1 : 0; }
    operator bool() { return usb_configuration && (*line_rtsdtr & (USB_SERIAL_DTR | USB_SERIAL_RTS)); }
    int usb_read(void *buffer, uint32_t size);
    void usb_flush_cb();
    size_t readBytes(char *buffer, size_t length) {
        size_t count=0;
        unsigned long startMillis = systick_millis_count;
        do {
            count += usb_read(buffer + count, length - count);
            if (count >= length) return count;
        } while(systick_millis_count - startMillis < _timeout);
        setReadError();
        return count;
    }
};

extern usb_serial_class UsbSerial1;
extern usb_serial_class UsbSerial2;

#endif // __cplusplus

#endif // USB_SERIAL || USB_SERIAL_HID
#endif // USBserial_h_
