#include <linux/if.h> // ifreq, IFNAMSIZ
#include <linux/if_tun.h> // IFF_TAP, IFF_NO_PI, TUNSETIFF
#include <errno.h> // errno
#include <fcntl.h> // open
#include <stdlib.h> // exit
#include <string.h> // memset
#include <sys/ioctl.h> // ioctl
#include <unistd.h> // read, write, close

#include "tuntap.h"
#include "utils.h" // run_cmd

static int tun_fd; // tun/tap file descriptor

char *tapaddr = "10.0.0.5";
char *taproute = "10.0.0.0/24";

void tun_open(char *dev_name);

/***************************************************************/

/*
 * Taken from Kernel Documentation/networking/tuntap.txt
 */

// Returns file descriptor that can be used to read and write data
// to the virtual device's ethernet buffer.

void tun_open(char *dev_name)
{
    struct ifreq ifr;
    int err;

    if( (tun_fd = open("/dev/net/tun", O_RDWR)) < 0 ) {
        print_err("Cannot open TUN/TAP dev");
        exit(1);
    }

    //CLEAR(ifr);
    memset(&ifr, 0, sizeof(ifr));

    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *
     *        IFF_NO_PI - Do not provide packet information
     */
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    if( *dev_name ) {
        strncpy(ifr.ifr_name, dev_name, IFNAMSIZ);
    }

    if( (err = ioctl(tun_fd, TUNSETIFF, (void *) &ifr)) < 0 ){
        print_err("ERR: Could not ioctl tun: %s\n", strerror(errno));
        close(tun_fd);
    }

    strcpy(dev_name, ifr.ifr_name);
}

/***************************************************************/

void TUN_init(char *dev_name)
{
    tun_open(dev_name);

    if ( run_cmd("ip link set dev %s up", dev_name) != 0)
    {
        print_err("ERROR when setting up if\n");
    }

    if ( run_cmd("ip route add dev %s %s", dev_name, taproute) != 0)
    {
        print_err("ERROR when setting route for if\n");
    }

    if ( run_cmd("ip address add dev %s local %s", dev_name, tapaddr) != 0)
    {
        print_err("ERROR when setting addr for if\n");
    }
}

int TUN_read(char* buf, int len)
{
    return read(tun_fd, buf, len);
}

int TUN_write(char* buf, int len)
{
    return write(tun_fd, buf, len);
}
