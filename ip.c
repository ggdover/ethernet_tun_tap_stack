#include <arpa/inet.h> // inet_pton, ntohl, AF_INET
#include <stdlib.h> // exit
#include <stdio.h> // perror

#include "ip.h"

uint32_t IP_parse(char* addr)
{
    uint32_t dst = 0;

    // inet_pton = Convert IPv4 and IPv6 addresses from text to binary form
    // AF_INET (AF = Address Family) = IPv4 Internet protocol
    //     - (Source: https://man7.org/linux/man-pages/man2/socket.2.html)
    if (inet_pton(AF_INET, addr, &dst) != 1) {
        perror("ERR: Parsing inet address failed");
        exit(1);
    }

    // htonl, htons, ntohl, ntohs = System call functions for converting between host and network byte order
    //
    // ntohl
    //     - Converts the unsigned integer 'netlong' from network byte order to host byte order
    //     - ntohl -> (n = network, to = to, h = host, l = long (netlong)) 
    return ntohl(dst);
}
