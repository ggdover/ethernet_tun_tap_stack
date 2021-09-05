
#ifndef _ARP_H_
#define _ARP_H_

#include "types.h" // eth_hdr, arp_hdr, arp_ipv4, dev

#define ARP_TRANS_TABLE_LEN 32 // ARP Translation table length
#define ARP_ETHERNET 0x0001 // Hardware type = Ethernet, for ARP header
#define ARP_IPV4 0x0800 // Protocol type = IPv4, for ARP header

#define ARP_OP_REQUEST 0x0001 // opcode for "ARP request"
#define ARP_OP_REPLY 0x0002 // opcode for "ARP reply"

void ARP_init();
void ARP_handle_frame(dev d, eth_hdr* ethhdr);

#endif // _ARP_H_
