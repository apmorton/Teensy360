#include <WProgram.h>

#include "config.h"
#include "pins.h"
#include "post.h"
#include "usb_flasher.h"
#include "XSPI.h"

void initPins(void) {
    pinMode(SS, OUTPUT);
    pinMode(XBSB_XX, OUTPUT);
    pinMode(XBSB_EJ, OUTPUT);
    pinMode(XBOX_KIOSK, OUTPUT);

    digitalWriteFast(SS, HIGH);
    digitalWriteFast(XBSB_XX, HIGH);
    digitalWriteFast(XBSB_EJ, HIGH);
    digitalWriteFast(XBOX_KIOSK, HIGH);
    XSPI.begin();
    //XSPI.init(2);
    
    Serial1.begin(UART_DEFAULT_BAUD);
}

void handleSerial(void) {
    static unsigned long baud = 0;
    int available = 0;
    uint8_t usb_buffer[64];
    
    if ((available = UsbSerial2.available()) > 0) {
        available = min(64, available);
        available = UsbSerial2.readBytes((char*)usb_buffer, available);
        Serial1.write(usb_buffer, available);
    }
    if ((available = Serial1.available()) > 0) {
        UsbSerial2.write(Serial1.read());
    }
    if (UsbSerial2.baud() != baud) {
        baud = UsbSerial2.baud();
        UsbSerial1.print("Baud Change: ");
        UsbSerial1.println(baud);
        Serial1.begin(baud);
    }
}

void handlePost(void) {
    static uint32_t _next = 0;
    uint32_t _now = micros();
    
    // not yet
    if (_next >= _now) return;
    
    if (POST.hasChanged()) {
        UsbSerial1.println(POST.value, HEX);
        UsbSerial1.flush();
    }
    
    _next = _now + (POST_POLL_FREQUENCY * 1000);
}

int main(void) {
    initPins();

    while (1) {
        handleSerial();
        handlePost();
        Flasher.runReadyCommand();
    }
}
