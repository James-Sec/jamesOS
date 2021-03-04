#ifndef ETHERNET_H
#define ETHERNET_H

#include <network_utils.h>
#include <stdint.h>
#include <rtl8139.h>
#include <ip.h>
#include <l1_interface.h>

// network byte order
#define ETHER_TYPE_IPv4 0x0008
#define ETHER_TYPE_ARP 0x0608
#define ETHER_TYPE_WOL 0x4208
#define ETHER_TYPE_AVTP 0xF022
#define ETHER_TYPE_TRILL 0xF322
#define ETHER_TYPE_SRP 0xEA22
#define ETHER_TYPE_DEC_MOP_RC 0x0260
#define ETHER_TYPE_DECNET 0x0360
#define ETHER_TYPE_DEC_LAT 0x0460
#define ETHER_TYPE_RARP 0x3580
#define ETHER_TYPE_APPLETALK 0x9B80
#define ETHER_TYPE_AARP 0xF380
#define ETHER_TYPE_VLAN_TAGGED_FRAME 0x0081
#define ETHER_TYPE_SLPP 0x0281
#define ETHER_TYPE_VLACP 0x0381
#define ETHER_TYPE_IPX 0x3781
#define ETHER_TYPE_QNX_QNET 0x0482
#define ETHER_TYPE_IPV6 0xDD86
#define ETHER_TYPE_ETHER_FLOW_CONTROL 0x0888
#define ETHER_TYPE_LACP 0x0988
#define ETHER_TYPE_COBRA_NET 0x1988
#define ETHER_TYPE_MPLS_UNICAST 0x4788
#define ETHER_TYPE_MPLS_MULTICAST 0x4888
#define ETHER_TYPE_PPPOE_DISCOVERY_STAGE 0x6388
#define ETHER_TYPE_PPPOE_SESSION_STAGE 0x6488
#define ETHER_TYPE_HOME_PLUG 0x7B88
#define ETHER_TYPE_EAP_OVER_LAN 0x8E88
#define ETHER_TYPE_PROFINET_PROTOCOL 0x9288
#define ETHER_TYPE_SCSI_OVER_ETHERNET 0x9A88
#define ETHER_TYPE_ATA_OVER_ETHERNET 0xA288
#define ETHER_TYPE_ETHER_CAT_PROTOCOL 0xA488
#define ETHER_TYPE_802_1AD 0xA888
#define ETHER_TYPE_ETHERNET_POWERLINK 0xAB88
#define ETHER_TYPE_GOOSE 0xB888
#define ETHER_TYPE_GSE 0xB988
#define ETHER_TYPE_SV 0xBA88
#define ETHER_TYPE_MIKROTIK 0xBF88
#define ETHER_TYPE_LLDP 0xCC88
#define ETHER_TYPE_SERCOS_III 0xCD88
#define ETHER_TYPE_MEDIA_REDUNDANCY_PROTOCOL 0xE388
#define ETHER_TYPE_MAC_SECURETY 0xE588
#define ETHER_TYPE_PBB 0xE788
#define ETHER_TYPE_PTP 0xF788
#define ETHER_TYPE_NC_SI 0xF888
#define ETHER_TYPE_PRP 0xFB88
#define ETHER_TYPE_802_1AG 0x0289
#define ETHER_TYPE_FCOE 0x0689
#define ETHER_TYPE_FCOE_INITIALIZATION_PROTOCOL 0x1489
#define ETHER_TYPE_RCOE 0x1589
#define ETHER_TYPE_TTE 0x1D89
#define ETHER_TYPE_1905_1_IEEE 0x3a89
#define ETHER_TYPE_HSR 0x2F89
#define ETHER_TYPE_ECTP 0x0090
#define ETHER_TYPE_802_1Q 0x0091
#define ETHER_TYPE_802_1CB 0xC1F1

#define ETHER_DEST_MAC_SIZE 6
#define ETHER_SOURCE_MAC_SIZE 6
#define ETHER_TYPE_SIZE 2

#define ETHER_DEST_MAC_OFFSET 0
#define ETHER_SOURCE_MAC_OFFSET 6
#define ETHER_TYPE_OFFSET 12

#define ETHER_HEADER_SIZE 14

struct ether_frame
{
  uint8_t header [14];
  uint8_t *data;
} __attribute__((packed));

struct ether_frame* build_ether_frame (struct ether_frame* frame, uint8_t dest [6], uint16_t type, uint8_t *data, uint32_t data_size);

void send_ether_frame (struct ether_frame* frame);
void recv_ethernet_frame (uint8_t *data, uint32_t size);
uint8_t* ethernet_to_array (struct ether_frame *frame, uint32_t data_size);
struct ether_frame * array_to_ethernet (struct ether_frame* ether, uint8_t* array, uint32_t size);

void ethernet_get_attr_value (uint8_t* attr, uint32_t offset, uint32_t size, uint8_t* ret);
void ethernet_set_attr_value (uint8_t* attr, uint32_t offset, uint32_t size, uint8_t* value);

#endif
