#ifndef XSPI_H_
#define XSPI_H_

#ifdef __cplusplus

//#include <SdSpi.h>
#include <SPI.h>
#include "pins.h"

class XSPIClass : public SPIClass {
public:
    void powerOnXbox();
    void powerOffXbox();
    void leaveFlashMode();
};

extern XSPIClass XSPI;

#endif
#endif
