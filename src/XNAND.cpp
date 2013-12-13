#include "pins.h"
#include "WProgram.h"
#include "XNAND.h"
#include "XSPI.h"

XNandClass XNAND;

void XNandClass::clearStatus() {
    uint8_t tmp[4];
    XSPI.readRegister(0x04, tmp);
    XSPI.writeRegister(0x04, tmp);
}

uint16_t XNandClass::getStatus() {
    return XSPI.readRegisterWord(0x04);
}

bool XNandClass::waitReady(uint16_t timeout) {
    do {
        if (!(XSPI.readRegisterByte(0x04) & 0x01))
            return true;
    } while (timeout--);
    
    return false;
}

uint16_t XNandClass::eraseBlock(uint32_t blockNumber) {
    uint8_t tmp[4];

    clearStatus();
    
    XSPI.readRegister(0x00, tmp);
    tmp[0] |= 0x08;
    XSPI.writeRegister(0x00, tmp);
    
    XSPI.writeRegisterDword(0x0C, blockNumber << 9);
    
    if (!waitReady(0x1000))
        return 0x8001;
    
    XSPI.writeRegisterByte(0x08, 0xAA);
    XSPI.writeRegisterByte(0x08, 0x55);
    
    if (!waitReady(0x1000))
        return 0x8002;
    
    XSPI.writeRegisterByte(0x08, 0x05);
    
    if (!waitReady(0x1000))
        return 0x8003;
    
    return getStatus();
}

uint16_t XNandClass::beginRead(uint32_t blockNumber) {
    uint16_t ret;
    
    clearStatus();
    
    XSPI.writeRegisterDword(0x0C, blockNumber << 9);
    XSPI.writeRegisterByte(0x08, 0x03);
    
    if (!waitReady(0x1000))
        return 0x8011;
    
    ret = getStatus();
    
    XSPI.clearRegister(0x0C);
    
    return ret;
}

void XNandClass::beginWrite() {
    clearStatus();
    XSPI.clearRegister(0x0C);
}

void XNandClass::readBuffer(uint8_t *buf, uint8_t wordCount) {
    while (wordCount--) {
        XSPI.clearRegister(0x08);
        XSPI.readRegister(0x10, buf);
        buf += 4;
    }
}

void XNandClass::writeBuffer(uint8_t *buf, uint8_t wordCount) {
    while (wordCount--) {
        XSPI.writeRegister(0x10, buf);
        XSPI.writeRegisterByte(0x08, 0x01);
        buf += 4;
    }
}

uint16_t XNandClass::writeExecute(uint32_t blockNumber) {
    XSPI.writeRegisterDword(0x0C, blockNumber << 9);
    XSPI.writeRegisterByte(0x08, 0x55);
    XSPI.writeRegisterByte(0x08, 0xAA);
    XSPI.writeRegisterByte(0x08, 0x04);
    
    if (!waitReady(0x1000))
        return 0x8021;
    
    return getStatus();
}
