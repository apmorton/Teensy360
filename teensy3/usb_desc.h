/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2013 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be 
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows 
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _usb_desc_h_
#define _usb_desc_h_

// This header is NOT meant to be included when compiling
// user sketches in Arduino.  The low-level functions
// provided by usb_dev.c are meant to be called only by
// code which provides higher-level interfaces to the user.

#include <stdint.h>
#include <stddef.h>

#define ENDPOINT_UNUSED			0x00
#define ENDPOINT_TRANSIMIT_ONLY		0x15
#define ENDPOINT_RECEIVE_ONLY		0x19
#define ENDPOINT_TRANSMIT_AND_RECEIVE	0x1D

/*
To modify a USB Type to have different interfaces, start in this
file.  Delete the XYZ_INTERFACE lines for any interfaces you
wish to remove, and copy them from another USB Type for any you
want to add.

Give each interface a unique number, and edit NUM_INTERFACE to
reflect the number of interfaces.

Within each interface, make sure it uses a unique set of endpoints.
Edit NUM_ENDPOINTS to be at least the largest endpoint number used.
Then edit the ENDPOINT*_CONFIG lines so each endpoint is configured
the proper way (transmit, receive, or both).

The CONFIG_DESC_SIZE and any XYZ_DESC_OFFSET numbers must be
edited to the correct sizes.  See usb_desc.c for the giant array
of bytes.  Someday these may be done automatically..... (but how?)

If you are using existing interfaces, the code in each file should
automatically adapt to the changes you specify.  If you need to
create a new type of interface, you'll need to write the code which
sends and receives packets, and presents an API to the user.

Finally, edit usb_inst.cpp, which creats instances of the C++
objects for each combination.

Some operating systems, especially Windows, may cache USB device
info.  Changes to the device name may not update on the same
computer unless the vendor or product ID numbers change, or the
"bcdDevice" revision code is increased.

If these instructions are missing steps or could be improved, please
let me know?  http://forum.pjrc.com/forums/4-Suggestions-amp-Bug-Reports
*/



#if defined(USB_SERIAL)
  #define VENDOR_ID             0xFFFF
  #define PRODUCT_ID            0x0004
  #define DEVICE_CLASS          0xEF	// 0xEF = Miscellaneous
  #define DEVICE_SUBCLASS	0x02
  #define DEVICE_PROTOCOL	0x01	// 0x02, 0x01: Interface Association Descriptor
  #define MANUFACTURER_NAME     {'T','e','e','n','s','y','d','u','i','n','o'}
  #define MANUFACTURER_NAME_LEN 11
  #define PRODUCT_NAME          {'U','S','B',' ','S','e','r','i','a','l'}
  #define PRODUCT_NAME_LEN      10
  #define CDCI1_NAME  {'C','D','C','-','A','C','M',' ','1'}	//Names for interfaces
  #define CDCI1_NAME_LEN 9
  #define CDCI2_NAME  {'C','D','C','-','A','C','M',' ','2'}
  #define CDCI2_NAME_LEN 9
  #define FLASH_NAME  {'X','F','L','A','S','H'}
  #define FLASH_NAME_LEN 6
  #define EP0_SIZE              64
  #define NUM_ENDPOINTS         11
  #define NUM_USB_BUFFERS       12
  #define NUM_INTERFACE         5
  
  #define CDC_STATUS_INTERFACE  1
  #define CDC_DATA_INTERFACE    2
  #define CDC_ACM_ENDPOINT      6
  #define CDC_RX_ENDPOINT       7
  #define CDC_TX_ENDPOINT       8
  #define CDC_ACM_SIZE          16
  #define CDC_RX_SIZE           64
  #define CDC_TX_SIZE           64
  
  #define CDC2_STATUS_INTERFACE 3
  #define CDC2_DATA_INTERFACE   4
  #define CDC2_ACM_ENDPOINT     9
  #define CDC2_RX_ENDPOINT      10
  #define CDC2_TX_ENDPOINT      11
  #define CDC2_ACM_SIZE         16
  #define CDC2_RX_SIZE          64
  #define CDC2_TX_SIZE          64
  
  #define FLASHER_INTERFACE     0
  #define FLASHER_RX_ENDPOINT   5
  #define FLASHER_TX_ENDPOINT   2
  #define FLASHER_RX_SIZE       64
  #define FLASHER_TX_SIZE       64

  #define CONFIG_DESC_SIZE      (9+(8+9+7+7)+(8+9+5+5+4+5+7+9+7+7)+(8+9+5+5+4+5+7+9+7+7))
  
  #define ENDPOINT2_CONFIG      ENDPOINT_TRANSIMIT_ONLY
  #define ENDPOINT5_CONFIG      ENDPOINT_RECEIVE_ONLY
  
  #define ENDPOINT6_CONFIG      ENDPOINT_TRANSIMIT_ONLY
  #define ENDPOINT7_CONFIG      ENDPOINT_RECEIVE_ONLY
  #define ENDPOINT8_CONFIG      ENDPOINT_TRANSIMIT_ONLY
  
  #define ENDPOINT9_CONFIG      ENDPOINT_TRANSIMIT_ONLY
  #define ENDPOINT10_CONFIG     ENDPOINT_RECEIVE_ONLY
  #define ENDPOINT11_CONFIG     ENDPOINT_TRANSIMIT_ONLY
#endif

// NUM_ENDPOINTS = number of non-zero endpoints (0 to 15)
extern const uint8_t usb_endpoint_config_table[NUM_ENDPOINTS];

typedef struct {
	uint16_t	wValue;
	uint16_t	wIndex;
	const uint8_t	*addr;
	uint16_t	length;
} usb_descriptor_list_t;

extern const usb_descriptor_list_t usb_descriptor_list[];


#endif
