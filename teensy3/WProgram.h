#ifndef WProgram_h
#define WProgram_h

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "wiring.h"
#include "HardwareSerial.h"

#define DMAMEM __attribute__ ((section(".dmabuffers"), used))

#ifdef __cplusplus

#include "usb_serial.h"

#include "elapsedMillis.h"
#include "IntervalTimer.h"

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);

#include "pins_arduino.h"

#endif // __cplusplus

#endif // WProgram_h
