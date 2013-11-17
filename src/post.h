#ifndef POST_H_
#define POST_H_

#ifdef __cplusplus

#include <stdint.h>

class PostReader {
	uint8_t readPost();
public:
	bool hasChanged();
	uint8_t value;
};

extern PostReader POST;

#endif
#endif
