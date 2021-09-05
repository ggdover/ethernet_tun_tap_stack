
#ifndef _ETHERNET_H_
#define _ETHERNET_H_

#include "types.h" // dev, eth_hdr (stdint.h: uint16_t)

void ETH_transmit(dev d, uint16_t ethertype, unsigned char* payload, int payloadsize, unsigned char* dst);

#endif // _ETHERNET_H_
