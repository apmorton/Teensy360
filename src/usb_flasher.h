#ifndef USB_FLASHER_H_
#define USB_FLASHER_H_

#include <stdint.h>

#define CMD_DATA_READ		0x01
#define CMD_DATA_WRITE		0x02
#define CMD_DATA_INIT		0x03
#define CMD_DATA_DEINIT		0x04
#define CMD_DATA_STATUS		0x05
#define CMD_DATA_ERASE		0x06
#define CMD_DEV_VERSION		0x08
#define CMD_XBOX_PWRON		0x10
#define CMD_XBOX_PWROFF		0x11
#define CMD_DEV_UPDATE		0xF0

#ifdef __cplusplus

class NandFlasher : public Stream {
	bool commandPending = false;
	bool commandReady = false;
	
	uint8_t commandCode = 0;
	uint32_t argA;
	uint32_t argB;
	
	uint16_t status;
	
	void handleDataInit();
	void handleDataDeInit();
	void handleDataStatus();
	void handleDataRead();
	void handleDataWrite();
	void handleDataErase();
	void handleXboxPwrOn();
	void handleXboxPwrOff();
	void handleDevVersion();
	
	usb_packet_t *rx_packet=NULL;
	usb_packet_t *tx_packet=NULL;
	uint8_t transmit_previous_timeout=0;
public:
	int vendorCallback(uint8_t bRequest, uint16_t wLength);
	void vendorDataCallback(uint8_t bRequest, uint32_t argA, uint32_t argB);
	void runReadyCommand();
	
	virtual int available();
	virtual int read();
	virtual int peek();
	virtual void flush();
	virtual size_t write(uint8_t c) { return write(&c, 1); }
	virtual size_t write(const uint8_t *buffer, size_t size);
};

extern NandFlasher Flasher;
#endif

#endif
