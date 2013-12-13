#ifndef XSPI_H_
#define XSPI_H_

#ifdef __cplusplus

#include <SdSpi.h>
//#include <SPI.h>
#include "pins.h"

class XSPIClass : public SdSpi {
public:
    bool inFlashMode = false;
    void powerOnXbox();
    void powerOffXbox();
    void enterFlashMode();
    void leaveFlashMode();
    
    void readRegister(uint8_t reg, uint8_t *buf) {
        reg = ((reg << 2) | 1);
        uint8_t final[2] = {reg, 0xFF};

        digitalWriteFast(SS, LOW);
        send(final, 2);
        receive(buf, 4);
        digitalWriteFast(SS, HIGH);
    }

    uint16_t readRegisterWord(uint8_t reg) {
        reg = ((reg << 2) | 1);
        uint8_t final[2] = {reg, 0xFF};

        digitalWriteFast(SS, LOW);
        send(final, 2);
        receive(final, 2);
        digitalWriteFast(SS, HIGH);

        return (final[0] | (final[1] << 8));
    }
    
    uint8_t readRegisterByte(uint8_t reg) {
        reg = ((reg << 2) | 1);
        uint8_t final[2] = {reg, 0xFF};

        digitalWriteFast(SS, LOW);
        send(final, 2);
        final[0] = receive();
        digitalWriteFast(SS, HIGH);

        return final[0];
    }

    void writeRegister(uint8_t reg, uint8_t *buf) {
        reg = ((reg << 2) | 2);
        uint8_t final[5] = {reg, buf[0], buf[1], buf[2], buf[3]};

        digitalWriteFast(SS, LOW);
        send(final, 5);
        digitalWriteFast(SS, HIGH);
    }

    void writeRegisterByte(uint8_t reg, uint8_t byte) {
        reg = ((reg << 2) | 2);
        uint8_t final[5] = {reg, byte, 0, 0, 0};

        digitalWriteFast(SS, LOW);
        send(final, 5);
        digitalWriteFast(SS, HIGH);
    }

    void writeRegisterDword(uint8_t reg, uint32_t dword) {
        writeRegister(reg, (uint8_t*)&dword);
    }

    void clearRegister(uint8_t reg) {
        reg = ((reg << 2) | 2);
        uint8_t final[5] = {reg, 0, 0, 0, 0};

        digitalWriteFast(SS, LOW);
        send(final, 5);
        digitalWriteFast(SS, HIGH);
    }
};

extern XSPIClass XSPI;

#endif
#endif
