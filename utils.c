#include <stdarg.h> // va_list 
#include <stdio.h> // vsnprintf
#include <stdlib.h> // system

#include "utils.h" // (types.h: eth_hdr, arp_hdr, arp_ipv4)

#define CMDBUFLEN 100

int run_cmd(char *cmd, ...)
{
    va_list ap;
    char buf[CMDBUFLEN];
    va_start(ap, cmd);
    vsnprintf(buf, CMDBUFLEN, cmd, ap);

    va_end(ap);

    if (1) { // debug = TRUE
        printf("EXEC: %s\n", buf);
    }

    return system(buf);
}

void raw_print(unsigned char* data, size_t len)
{
    unsigned int i = 0;
    for (; i < len; ++i)
    {
        printf(" %x", data[i]);
    }
    printf("\n");
}

void eth_print(eth_hdr ethhdr)
{
    printf("  raw:");
    int i = 0;
    for (; i < 30; ++i)
    {
        unsigned char* buf = (unsigned char*)&ethhdr;
        printf(" %02x", buf[i]);
    }
    printf("\n");
    printf("  dmac: %02x:%02x:%02x:%02x:%02x:%02x\n", ethhdr.dmac[0], ethhdr.dmac[1], ethhdr.dmac[2], ethhdr.dmac[3], ethhdr.dmac[4], ethhdr.dmac[5]);
    printf("  smac: %02x:%02x:%02x:%02x:%02x:%02x\n", ethhdr.smac[0], ethhdr.smac[1], ethhdr.smac[2], ethhdr.smac[3], ethhdr.smac[4], ethhdr.smac[5]);
    printf("  ethertype: %x\n", ethhdr.ethertype);
}

void arp_print(arp_hdr arphdr)
{
    printf("  raw:");
    int i = 0;
    for (; i < 8; ++i)
    {
        unsigned char* buf = (unsigned char*)&arphdr;
        printf(" %x", buf[i]);
    }
    printf("\n");
    printf("  hwtype: %x\n", arphdr.hwtype);
    printf("  protype: %x\n", arphdr.protype);
    printf("  hwsize: %u\n", arphdr.hwsize);
    printf("  prosize: %u\n", arphdr.prosize);
    printf("  opcode: %u\n", arphdr.opcode);
}

void arp_ipv4_print(arp_ipv4 arpdata)
{
    printf("  raw:");
    int i = 0;
    for (; i < 20; ++i)
    {
        unsigned char* buf = (unsigned char*)&arpdata;
        printf(" %x", buf[i]);
    }
    printf("\n");
    printf("  smac: %02x:%02x:%02x:%02x:%02x:%02x\n", arpdata.smac[0], arpdata.smac[1], arpdata.smac[2], arpdata.smac[3], arpdata.smac[4], arpdata.smac[5]);

    printf("  sip: %02x:%02x:%02x:%02x (0x%0x)\n", ((arpdata.sip >> 24) & 0x000000FF), ((arpdata.sip >> 16) & 0x000000FF), ((arpdata.sip >> 8) & 0x000000FF), (arpdata.sip & 0x000000FF), arpdata.sip);
    printf("  dmac: %02x:%02x:%02x:%02x:%02x:%02x\n", arpdata.dmac[0], arpdata.dmac[1], arpdata.dmac[2], arpdata.dmac[3], arpdata.dmac[4], arpdata.dmac[5]);
    printf("  dip: %02x:%02x:%02x:%02x (0x%0x)\n", ((arpdata.dip >> 24) & 0x000000FF), ((arpdata.dip >> 16) & 0x000000FF), ((arpdata.dip >> 8) & 0x000000FF), (arpdata.dip & 0x000000FF), arpdata.dip);
}
