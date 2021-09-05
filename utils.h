
#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h> // fprintf

#include "types.h" // eth_hdr, arp_hdr, arp_ipv4

// Use this to print all diagnostic output
#define print_err(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)

// Uncomment this line to have the compiler exclude all the 'print_debug' calls/statements.
#define DEBUG 1

// These print functions are only included by compiler if 'DEBUG' is defined.
#define print_debug(fmt, ...) \
	do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt "\n", __FILE__, \
							__LINE__, __func__, ##__VA_ARGS__); } while (0)

// Used to print raw data in hex format
#define print_raw(data, len) \
	do { \
		if (DEBUG) { \
			fprintf(stderr, "%s:%d:%s(): raw hex data:\n", __FILE__, __LINE__, __func__); \
			raw_print(data, len); \
		} \
	} while (0)

// Used to print a ethernet frame, pass in parameter of type "eth_hdr"
#define print_eth(ethhdr) \
	do { \
		if (DEBUG) { \
			fprintf(stderr, "%s:%d:%s(): eth_print\n", __FILE__, \
			__LINE__, __func__); \
			eth_print(ethhdr); \
		} \
	} while (0)

// Used to print a arp frame (everythin except the payload), pass in parameter of type "arp_hdr"
#define print_arp(arphdr) \
	do { \
		if (DEBUG) { \
			fprintf(stderr, "%s:%d:%s(): arp_print\n", __FILE__, \
			__LINE__, __func__); \
			arp_print(arphdr); \
		} \
	} while (0)

// Used to print a arp payload when protocol is ipv4, pass in parameter of type "arp_ipv4"
#define print_arp_ipv4(arpipv4) \
	do { \
		if (DEBUG) { \
			fprintf(stderr, "%s:%d:%s(): arp_ipv4_print\n", __FILE__, \
			__LINE__, __func__); \
			arp_ipv4_print(arpipv4); \
		} \
	} while (0)

void raw_print(unsigned char* data, size_t len);
void eth_print(eth_hdr ethhdr);
void arp_print(arp_hdr arphdr);
void arp_ipv4_print(arp_ipv4 arpdata);
int run_cmd(char *cmd, ...);

#endif // _UTILS_H_
