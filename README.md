
# Summary

This project involved creating a L2 and L3 TCP/IP Ethernet stack
using TUN/TAP devices to communicate with the system.
We are doing this while following this guide: https://www.saminiir.com/lets-code-tcp-ip-stack-1-ethernet-arp/

# What is TUN and TAP?

TUN = network TUNneling
(https://en.wikipedia.org/wiki/Tunneling_protocol)
TAP = network TAP (tap as in tapping/monitoring a phone line)
(https://en.wikipedia.org/wiki/Network_tap)

Wiki: https://en.wikipedia.org/wiki/TUN/TAP

Following shows where tun and tap fits in the 7 layer osi model

L7 Application
L6 Presentation
L5 Session
L4 Transport
L3 Network <--- TUN (carries IP packets)
L2 Data link <--- TAP (carries Ethernet frames)
L1 Physical

# Address Resolution Protocol (ARP)

Used for dynamically mapping a 48-bit ethernet address (MAC address)
to a protocol address (e.g. IPv4 address).
The usual case is that you know the IP address of some service in your LAN, but to establish actual communications, also the hardware address (MAC) needs to be known. Hence, ARP is used to broadcast and query the
network, asking the owner of the IP address to report its hardware
address.

In summary:
ARP broadcasts and queries the network, asking owner of a IP address to
report its hardware address (MAC address).

# How to run

1. Run the ethernet stack ```sudo ./a.out``` (Most likely requires sudo)
2. Start arpping ```sudo arpping -I tap0 10.0.0.4``` (Most likely requires sudo)
    * Make sure the ip address passed in matches the ip-address that is set/hardcoded as the local ip-address in the ethernet stack.
    * 'arpping' is a tool for pinging with arp-packets which you might need to install
    if you don't have it
3. Observe as arpping successfully receive replies from our ethernet stack as it tries to ping it!
