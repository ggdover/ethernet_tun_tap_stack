#include <string.h> // memcpy
#include <arpa/inet.h> // htons, htonl
#include <linux/if_ether.h> // ETH_P_ARP

#include "arp.h"
#include "ethernet.h" // ETH_transmit
#include "utils.h" // print_debug, print_arp, print_arp_ipv4

static arp_trans_table_entry arp_trans_table[ARP_TRANS_TABLE_LEN];

int arp_update_trans_table(arp_hdr arphdr, arp_ipv4 arpdata);
int arp_insert_trans_table(arp_hdr arphdr, arp_ipv4 arpdata);

/***************************************************************/

int arp_update_trans_table(arp_hdr arphdr, arp_ipv4 arpdata)
{
    arp_trans_table_entry* entry;

    for (int i = 0; i < ARP_TRANS_TABLE_LEN; ++i)
    {
        entry = &arp_trans_table[i];

        if (entry->state == ARP_STATE_FREE) continue;

        if (entry->hwtype == arphdr.hwtype && entry->sip == arpdata.sip)
        {
            memcpy(entry->smac, arpdata.smac, 6);
            return 1;
        }
    }

    return 0;
}

int arp_insert_trans_table(arp_hdr arphdr, arp_ipv4 arpdata)
{
    arp_trans_table_entry* entry;

    for (int i = 0; i < ARP_TRANS_TABLE_LEN; ++i)
    {
        entry = &arp_trans_table[i];

        if (entry->state == ARP_STATE_FREE)
        {
            entry->state = ARP_STATE_RESOLVED;

            entry->hwtype = arphdr.hwtype;
            entry->sip = arpdata.sip;
            memcpy(entry->smac, arpdata.smac, sizeof(entry->smac));

            return 0;
        }
    }

    return -1;
}

// Create and send a ARP reply as response to receiving a ARP request
// d [in] - Holds our local address (MAC and IP address)
// arphdr [in] - Copy of the ARP package that we have received and that we are
//               modifying and reusing to send a reply.
void arp_reply(dev d, arp_hdr* arphdr)
{
    arp_ipv4* arpdata = (arp_ipv4 *) arphdr->data; 

    print_debug("*** arp_reply, start  ***\n");
    print_arp(*arphdr);
    print_arp_ipv4(*arpdata);

    // Set destination address (MAC and IP address)
    // (Changed to the address (source address) that the arp package was received from)
    memcpy(arpdata->dmac, arpdata->smac, 6);
    arpdata->dip = arpdata->sip;
    // Set source address (MAC and IP address)
    // (Set to our local address)
    memcpy(arpdata->smac, d.hwaddr, 6);
    arpdata->sip = d.addr;

    // Change byte order from 'host' to 'network' on ip addresses
    arpdata->dip = htonl(arpdata->dip);
    arpdata->sip = htonl(arpdata->sip);

    print_debug("*** arp_reply, after new MAC and ip have been set ***\n");
    print_arp(*arphdr);
    print_arp_ipv4(*arpdata);

    arphdr->opcode = ARP_OP_REPLY;

    // Change byte order from 'host' to 'network' on opcode, hwtype, protype
    arphdr->opcode = htons(arphdr->opcode);
    arphdr->hwtype = htons(arphdr->hwtype);
    arphdr->protype = htons(arphdr->protype);

    printf("*** arp_reply, after byte order changed from host to network order ***\n");
    print_arp(*arphdr);
    print_arp_ipv4(*arpdata);

    int len = sizeof(arp_hdr) + sizeof(arp_ipv4);
    ETH_transmit(d, ETH_P_ARP, (unsigned char*)arphdr, len, arpdata->dmac);
}

/***************************************************************/

void ARP_init()
{
    memset(arp_trans_table, 0, ARP_TRANS_TABLE_LEN  * sizeof(arp_trans_table_entry));
}

void ARP_handle_frame(dev d, eth_hdr* ethhdr)
{
    arp_hdr* arphdr = (arp_hdr *) ethhdr->payload;

    // Convert fields from Network-byte-order to Host-byte-order.
    // * Network-byte-order is the order in which bytes are stored on the network
    // as it is being traveled between computers, and is the same for all computers,
    // often it is Big endianess aka MSB (Most-significant-byte first) order.
    // * Host-byte-order is the byte order on the machine/computer, and
    // might be different between computers on the same network.
    //
    // Now we are handling a incoming data packet (ethernet/arp frame)
    // and so we are converting from Network-byte-order to Host-byte-order.
    arphdr->hwtype = ntohs(arphdr->hwtype);
    arphdr->protype = ntohs(arphdr->protype);
    arphdr->opcode = ntohs(arphdr->opcode);

    arp_print(*arphdr);

    if (arphdr->hwtype != ARP_ETHERNET)
    {
        print_debug("wrong hw type\n");
        return;
    }

    // Optionally check hardware size/length
    //if (hdr.hwsize)

    if (arphdr->protype != ARP_IPV4)
    {
        print_debug("wrong proto type\n");
        return;
    }

    // Optionally check protocol size/length
    //if (hdr.prosize)

    arp_ipv4* arpdata = (arp_ipv4*) arphdr->data;

    print_debug("*** ARP_handle_frame, arp_ipv4 before network to host byte order swap ***\n");
    arp_ipv4_print(*arpdata);
    arpdata->sip = ntohl(arpdata->sip);
    arpdata->dip = ntohl(arpdata->dip);
    arp_ipv4_print(*arpdata);
    print_debug("*** ARP_handle_frame, arp_ipv4 after network to host byte order swap ***\n");

    int merge_flag = arp_update_trans_table(*arphdr, *arpdata);

    if (d.addr != arpdata->dip)
    {
        print_debug("this arp pack is not for us\n");
        return;
    }

    if (!merge_flag)
    {
        if (arp_insert_trans_table(*arphdr, *arpdata) != 0)
        {
            print_debug("failed to insert into arp translate table, table is full\n");
            return;
        }
    }

    if (arphdr->opcode == ARP_OP_REQUEST)
    {
        arp_reply(d, arphdr);
    }
    else
    {
        print_debug("opcode not supported\n");
    }
}
