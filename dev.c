#include <stdio.h> // sscanf
#include "ip.h" // IP_parse
#include "dev.h"

dev init_dev(char* addr, char* hwaddr)
{
    dev d;
    d.addr = IP_parse(addr);
    // Initialize Hardware/MAC -address
    sscanf(hwaddr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &d.hwaddr[0],
                                                    &d.hwaddr[1],
                                                    &d.hwaddr[2],
                                                    &d.hwaddr[3],
                                                    &d.hwaddr[4],
                                                    &d.hwaddr[5]);

    return d;
}
