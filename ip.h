
#ifndef _IP_H_
#define _IP_H_

#include <stdint.h> // uint32_t

// Convert IP version 4 address from textual to binary form
uint32_t IP_parse(char* addr);

#endif // _IP_H_
