#include "XSPI.h"
#include "pins.h"
#include "WProgram.h"

XSPIClass XSPI;


void XSPIClass::powerOnXbox() {
    for (int i = 0; i < 8; i++) {
        digitalWriteFast(XBOX_KIOSK, ((i % 2) == 0) ? LOW : HIGH);
        delay(50); // 50ms
    }
}

void XSPIClass::powerOffXbox() {
    digitalWriteFast(SS, HIGH);
    digitalWriteFast(XBSB_XX, LOW);
    digitalWriteFast(XBSB_EJ, LOW);
    
    delay(50); // 50ms
    
    digitalWriteFast(XBSB_EJ, HIGH);
}

void XSPIClass::leaveFlashMode() {
    digitalWriteFast(SS, HIGH);
    digitalWriteFast(XBSB_EJ, LOW);
    
    delay(50); // 50ms
    
    digitalWriteFast(XBSB_XX, LOW);
    digitalWriteFast(XBSB_EJ, HIGH);
}
