#include <string.h> // memcpy
#include <arpa/inet.h> // htons

#include "ethernet.h" // (types.h: dev, eth_hdr, arp_hdr, arp_ipv4)
#include "tuntap.h" // TUN_write
#include "utils.h" // print_debug, print_eth, print_arp, print_arp_ipv4

void ETH_transmit(dev d, uint16_t ethertype, unsigned char* payload, int payloadsize, unsigned char* dst)
{
    static eth_hdr ethhdr;

    memcpy(ethhdr.smac, d.hwaddr, 6);
    memcpy(ethhdr.dmac, dst, 6);
    ethhdr.ethertype = htons(ethertype);
    memcpy(ethhdr.payload, payload, payloadsize);

    int len = sizeof(eth_hdr) + payloadsize;

    print_debug("======== ETH TRANSMIT =========");
    print_eth(ethhdr);

    arp_hdr* arphdr = (arp_hdr*)ethhdr.payload;

    print_debug("ethhdr payload (e.g. arphdr) printed as raw data");
    print_raw(payload, payloadsize);
    print_debug("same payload printed as an arphdr");
    print_arp(*arphdr);

    arp_ipv4* arpdata = (arp_ipv4*)arphdr->data;
    print_debug("payload of that arphdr printed as arp_ipv4");
    print_arp_ipv4(*arpdata);

    print_debug("===== TUN_write raw data:");
    print_raw((unsigned char*)&ethhdr, len);
    TUN_write((char*) &ethhdr, len);
}
