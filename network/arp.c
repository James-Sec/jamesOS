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

	kprint ("======printing arp table:======\n");
	for (i = 0; i < arp_table_size; i++)
		kprintf ("ip: %x, mac: %x:%x:%x:%x:%x:%x\n", 7, arp_table [i].ip_addr, arp_table [i].mac_addr [0], arp_table [i].mac_addr [1], arp_table [i].mac_addr [2], arp_table [i].mac_addr [3], arp_table [i].mac_addr [4], arp_table [i].mac_addr [5]);

	kprint ("====== END ARP TABLE ======\n");
}

static void recv_arp_reply (struct arp_t* arp)
{
  kprintf ("%d.%d.%d.%d\n", 4, arp->header[ARP_SPA_OFFSET], arp->header[ARP_SPA_OFFSET + 1], arp->header[ARP_SPA_OFFSET + 2], arp->header[ARP_SPA_OFFSET + 3]);
  update_arp_table (arp);
}

static void send_arp_reply (struct arp_t* arp)
{
  //kprintf ("mac: %x\n", 1, rtl8139_device->mac_addr);
  //kprintf ("&mac: %x\n", 1, &rtl8139_device->mac_addr);
  //struct arp_t* new_arp = build_arp_packet (1, 0x0800, 6, 4, 2, &rtl8139_device->mac_addr, &arp->header[ARP_TPA_OFFSET], &arp->header[ARP_SHA_OFFSET], &arp->header[ARP_SPA_OFFSET]);

  send_ethernet_frame (&arp->header[ARP_SHA_OFFSET], arp, ARP_SIZE, ETHER_TYPE_ARP);
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
  kprintf ("%x\n", 1, rtl8139_device->mac_addr[0]);
  kprintf ("sha: %x\n", 1, rtl8139_device->mac_addr);
  //struct arp_t* new_arp = build_arp_packet (1, 0x0800, 6, 4, 1, rtl8139_device->mac_addr,sip, 0, dip);

  //send_ether_frame (build_ether_frame (&new_arp->header[ARP_THA_OFFSET], ETHER_TYPE_ARP, new_arp, ARP_SIZE));
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
      kprint ("ip checked... sending arp reply\n");

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

struct arp_t* build_arp_packet (uint16_t htype, uint16_t ptype, uint8_t hlen, uint8_t plen, uint16_t oper, uint8_t* sha, uint8_t* spa, uint8_t* tha, uint8_t* tpa)
{
  uint8_t i;

  struct arp_t* arp = (struct arp_t*) kmalloc_u (sizeof (struct arp_t));
  arp->header[ARP_HTYPE_OFFSET] = (htype >> 8) & 0xff;
  arp->header[ARP_HTYPE_OFFSET + 1] = htype & 0xff;
  
  arp->header[ARP_PTYPE_OFFSET] = (ptype >> 8) & 0xff;
  arp->header[ARP_PTYPE_OFFSET + 1] = ptype & 0xff;

  arp->header[ARP_HLEN_OFFSET] = hlen;
  arp->header[ARP_PLEN_OFFSET] = plen;

  arp->header[ARP_OPER_OFFSET] = (oper >> 8) & 0xff;
  arp->header[ARP_OPER_OFFSET + 1] = oper & 0xff;

  for (i = 0 ; i < hlen; i++)
    arp->header[ARP_SHA_OFFSET + i] = sha[i];
  kprintf ("sha: %x\n", 1, sha);

  for (i = 0 ; i < plen ; i++)
    arp->header[ARP_SPA_OFFSET + i] = spa[i];

  for (i = 0 ; i < hlen; i++)
    if (tha)
      arp->header[ARP_THA_OFFSET + i] = tha[i];
    else
      arp->header[ARP_THA_OFFSET + i] = 0xff;

  for (i = 0 ; i < plen ; i++)
    arp->header[ARP_TPA_OFFSET + i] = tpa[i];
  return arp;
}
