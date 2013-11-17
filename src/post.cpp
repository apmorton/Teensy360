#include "post.h"

#include "pins.h"
#include "config.h"

PostReader POST;


#define readBit(bit) ((analogRead(bit) > POST_ANALOG_THRESHOLD) ? 1 : 0)

#define _readPost() ( \
    (readBit(POST_BIT0) << 0) | \
    (readBit(POST_BIT1) << 1) | \
    (readBit(POST_BIT2) << 2) | \
    (readBit(POST_BIT3) << 3) | \
    (readBit(POST_BIT4) << 4) | \
    (readBit(POST_BIT5) << 5) | \
    (readBit(POST_BIT6) << 6) | \
    (readBit(POST_BIT7) << 7))

bool PostReader::hasChanged() {
    uint8_t curVal = readPost();
    
    if (curVal != value) {
        value = curVal;
        return true;
    }
    
    return false;
}

uint8_t PostReader::readPost() {
    uint8_t count = 0;
    uint8_t lastRead = 0;
    
    while (count < POST_NUMBER_OF_READS) {
        // read the post value
        uint8_t currentRead = _readPost();
        
        // increment count
        count++;
        
        // values do not match, restart count
        if (currentRead != lastRead) {
            lastRead = currentRead;
            count = 0;
        }
    }
    
    return lastRead;
}
