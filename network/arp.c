#include <arp.h>

static void update_arp_table (struct arp_t* arp) {
  uint32_t i;
  for (i = 0; i < arp_table_size; i++)
  {
    if (!memcmp (arp_table[i].mac_addr, &arp->header[ARP_SHA_OFFSET], 6))
    {
      memcpy (&arp->header [ARP_SPA_OFFSET], &arp_table [i].ip_addr, 4);
      return ;
    }
  }
  
	memcpy (&arp->header [ARP_SPA_OFFSET], &arp_table [arp_table_size].ip_addr, 4);
	memcpy (&arp->header [ARP_SHA_OFFSET], arp_table [arp_table_size].mac_addr, 6);
	arp_table_size++;
}

static void recv_arp_reply (struct arp_t* arp)
{
  update_arp_table (arp);
}

static void send_arp_reply (struct arp_t* arp)
{
  struct arp_t* new_arp = kmalloc_u (sizeof (struct arp_t));

  build_arp_packet (new_arp, 0x0100, 0x0008, 6, 4, 0x0200, &rtl8139_device->mac_addr, &arp->header[ARP_TPA_OFFSET], &arp->header[ARP_SHA_OFFSET], &arp->header[ARP_SPA_OFFSET]);

  send_ethernet_frame (&new_arp->header[ARP_THA_OFFSET], new_arp, ARP_HEADER_SIZE, ETHER_TYPE_ARP);
  kfree ((uint8_t*)new_arp, sizeof (struct arp_t));
}

void send_arp_request (uint32_t ip)
{
  uint8_t dip[4];
  dip[0] = (ip >> 24) & 0xff;
  dip[1] = (ip >> 16) & 0xff;
  dip[2] = (ip >> 8) & 0xff;
  dip[3] = ip & 0xff;
  uint8_t sip[4];
  sip[0] = (rtl8139_device->ip_addr >> 24) & 0xff;
  sip[1] = (rtl8139_device->ip_addr >> 16) & 0xff;
  sip[2] = (rtl8139_device->ip_addr >> 8) & 0xff;
  sip[3] = rtl8139_device->ip_addr & 0xff;

  struct arp_t* new_arp = kmalloc_u (sizeof (struct arp_t));
  build_arp_packet (new_arp, 0x0100, 0x0800, 6, 4, 0x0200, rtl8139_device->mac_addr,sip, 0, dip);

  send_ethernet_frame (&new_arp->header[ARP_THA_OFFSET], new_arp, ARP_HEADER_SIZE, ETHER_TYPE_ARP);
  kfree ((uint8_t*)new_arp, sizeof (struct arp_t));
}

static void recv_arp_request (struct arp_t* arp)
{
  uint16_t arp_ptype = arp->header[ARP_PTYPE_OFFSET] << 8;
  arp_ptype += arp->header[ARP_PTYPE_OFFSET + 1];

  if (arp_ptype == 0x0800)
  {
    uint8_t i;
    uint32_t target_ip = 0;
    for (i = 0 ; i < 4 ; i++)
      target_ip += arp->header[ARP_TPA_OFFSET + i] << (24 - (i * 8));

    if (target_ip == rtl8139_device->ip_addr)
    {
      update_arp_table (arp);
      send_arp_reply (arp);
    }
  }
  return;
}


void recv_arp_handler (struct arp_t* arp)
{
  if (arp->header[ARP_OPER_OFFSET + 1] == 1) 
    recv_arp_request (arp);
  else if (arp->header[ARP_OPER_OFFSET + 1] == 2)
    recv_arp_reply (arp);
  return;
}

struct arp_t* build_arp_packet (struct arp_t* arp, uint16_t htype, uint16_t ptype, uint8_t hlen, uint8_t plen, uint16_t oper, uint8_t sha [6], uint8_t* spa, uint8_t tha[6], uint8_t* tpa)
{
  set_bytes_attr_value (arp->header, ARP_HTYPE_OFFSET, ARP_HTYPE_SIZE, &htype);
  set_bytes_attr_value (arp->header, ARP_PTYPE_OFFSET, ARP_PTYPE_SIZE, &ptype);
  set_bytes_attr_value (arp->header, ARP_HLEN_OFFSET, ARP_HLEN_SIZE, &hlen);
  set_bytes_attr_value (arp->header, ARP_PLEN_OFFSET, ARP_PLEN_SIZE, &plen);
  set_bytes_attr_value (arp->header, ARP_OPER_OFFSET, ARP_OPER_SIZE, &oper);
  set_bytes_attr_value (arp->header, ARP_SHA_OFFSET, ARP_SHA_SIZE, sha);
  set_bytes_attr_value (arp->header, ARP_SPA_OFFSET, ARP_SPA_SIZE, spa);
  set_bytes_attr_value (arp->header, ARP_THA_OFFSET, ARP_THA_SIZE, tha);
  set_bytes_attr_value (arp->header, ARP_TPA_OFFSET, ARP_TPA_SIZE, tpa);
}
