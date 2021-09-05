
#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h> // uint32_t, uint16_t

/***********************************************************************
 *                           DEVICE TYPES
 ***********************************************************************/

// dev - Represents a enpoint/node/device in a network that can be receiver/sender or ethernet/arp frames of data
typedef struct _dev
{
    unsigned char hwaddr[6]; // MAC address (6 bytes)
    uint32_t addr; // IP address (version 4, 4 bytes) (in binary form, see function "IP_parse" in "ip.h")
} dev;

/***********************************************************************
 *                           ETHERNET TYPES
 ***********************************************************************/

// eth_hdr - Represents a Ethernet frame
typedef struct _eth_hdr
{
    unsigned char dmac[6]; // destination MAC address
    unsigned char smac[6]; // source MAC address
    uint16_t ethertype; // 2-octet (16 bits, 2 bytes) field. 
                        // If the fields value is greater or equal to 1536, 
                        // the field contains the type of the payload (e.g. IPv4, ARP). 
                        // If the value is less than that, it contains the length of the payload.
    unsigned char payload[];
} __attribute__((packed)) eth_hdr;

/***********************************************************************
 *                             ARP TYPES
 ***********************************************************************/

// arp_hdr - Represents a ARP (Address resolution protocol) frame
//           Received in the 'payload' field of an ethernet frame (struct eth_hdr)
typedef struct _arp_hdr
{
    uint16_t hwtype; // 2-octet (16 bits, 2 bytes) field.
                     // Determines the link layer type used.
                     // Example: esthernet == 0x0001
    uint16_t protype; // 2-octet (16 bits, 2 bytes) field
                      // Indicates the protocol type
                      // Example: IPv4 == 0x0800
    unsigned char hwsize; // 1-octet (8 bits, 1 byte) field
                          // Size (number of bytes) of hardware 
                          // field (hwtype).
                          // Example: 6 bytes for MAC addresses
    unsigned char prosize; // 1-octet (8 bits, 1 byte) field
                           // Size (number of bytes) of protocol 
                           // field (protype).
                           // Example: 4 bytes for IP (version 4)
                           //          addresses.
    uint16_t opcode; // 2-octet (16 bits, 2 bytes) field
                     // Declares the type of ARP message.
                     // Example: ARP request = 1, ARP reply = 2, 
                     //          RARP request = 3, RARP reply = 4
    unsigned char data[]; // aka payload
} __attribute__((packed)) arp_hdr;

// arp_ipv4 - Represents the payload/data field in a ARP frame
//            if the protocol type is IPv4
typedef struct _arp_ipv4
{
    unsigned char smac[6]; // Source (Sender) MAC address (6 bytes)
    uint32_t sip; // Source (Sender) IP address (version 4, 4 bytes)
    unsigned char dmac[6]; // Destination (Receiver) MAC address (6 bytes)
    uint32_t dip; // Destination (Receiver) IP address (version 4, 4 bytes)
} __attribute__((packed)) arp_ipv4;

typedef enum _arp_state
{
    ARP_STATE_FREE,
    ARP_STATE_WAITING,
    ARP_STATE_RESOLVED
} arp_state;

// Represent a entry in the arp translation table
typedef struct _arp_trans_table_entry
{
    uint16_t hwtype;
    uint32_t sip;
    unsigned char smac[6];
    arp_state state;
} arp_trans_table_entry;

#endif // _TYPES_H_
