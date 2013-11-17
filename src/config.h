#ifndef CONFIG_H_
#define CONFIG_H_

// Analog values over this will be considered '1'
#define POST_ANALOG_THRESHOLD   300

// Number of consecutive reads which must be identical
#define POST_NUMBER_OF_READS    2

// Number of milliseconds between post polls
#define POST_POLL_FREQUENCY     10

// Default baud rate to open UART on
#define UART_DEFAULT_BAUD       115200

#endif
