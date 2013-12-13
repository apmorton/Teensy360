#ifndef XNAND_H_
#define XNAND_H_

#include "pins.h"

#ifdef __cplusplus

class XNandClass {
public:
    void clearStatus();
    uint16_t getStatus();
    bool waitReady(uint16_t timeout);
    uint16_t eraseBlock(uint32_t blockNumber);
    uint16_t beginRead(uint32_t blockNumber);
    void beginWrite();
    void readBuffer(uint8_t *buf, uint8_t wordCount);
    void writeBuffer(uint8_t *buf, uint8_t wordCount);
    uint16_t writeExecute(uint32_t blockNumber);
};

extern XNandClass XNAND;

#endif
#endif
