#include <stdio.h> // printf
#include <errno.h> // errno
#include <stdlib.h> // calloc
#include <string.h> // strerror

#include "tuntap.h" // TUN_init, TUN_read
#include "arp.h" // ARP_handle_frame (types.h: eth_hdr, dev)
#include "dev.h" // init_dev
#include "utils.h" // print_err, print_debug, print_raw, print_eth

#define BUFLEN 100
#define DEV_LEN 10 // Max length of device (linux '/dev' file) name

static char* dev_name; // Holds the name of tun/tap device in linux '/dev' folder

int main(void)
{
    unsigned char buf[BUFLEN];
    dev_name = calloc(DEV_LEN, 1); // Like malloc(), but also sets all elements to zero

    TUN_init(dev_name);
    dev d = init_dev("10.0.0.4", "00:0c:29:6d:50:25");
    ARP_init();

    while (1)
    {
        if (TUN_read(buf, BUFLEN) < 0) {
            print_err("ERR: Read from tun_fd: %s\n", strerror(errno));
            exit(1);
        }
        print_debug("#############################################################\n");
        print_debug("TUN_read (print buffer):");
        print_raw(buf, BUFLEN);

        // Parsing ethernet frame
        eth_hdr *ethhdr = (eth_hdr *) buf;
        print_eth(*ethhdr);

        ARP_handle_frame(d, ethhdr);
    }

    return 0;
}