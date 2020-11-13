#include <arp.h>
static void recv_arp_reply (struct arp_t* arp)
{
  kprintf ("%d.%d.%d.%d", 4, arp->header[ARP_SPA], arp->header[ARP_SPA + 1], arp->header[ARP_SPA + 2], arp->header[ARP_SPA + 3]);
}

static void send_arp_reply (struct arp_t* arp)
{
  struct arp_t* new_arp = build_arp_packet (1, 0x0800, 6, 4, 2, &arp->header[ARP_THA], &arp->header[ARP_TPA], &arp->header[ARP_SHA], &arp->header[ARP_SPA]);

  send_ether_frame (build_ether_frame (&arp->header[ARP_SHA], ETHER_TYPE_ARP, new_arp, ARP_SIZE));
}

void send_arp_request (uint8_t* ip)
{
  uint8_t sip[4];
  sip[0] = (rtl8139_device->ip_addr >> 24) & 0xff;
  sip[1] = (rtl8139_device->ip_addr >> 16) & 0xff;
  sip[2] = (rtl8139_device->ip_addr >> 8) & 0xff;
  sip[3] = rtl8139_device->ip_addr & 0xff;
  kprintf ("%x\n", 1, rtl8139_device->mac_addr[0]);
  kprintf ("sha: %x\n", 1, rtl8139_device->mac_addr);
  struct arp_t* new_arp = build_arp_packet (1, 0x0800, 6, 4, 1, rtl8139_device->mac_addr,sip, 0, ip);

  send_ether_frame (build_ether_frame (&new_arp->header[ARP_THA], ETHER_TYPE_ARP, new_arp, ARP_SIZE));
}

static void recv_arp_request (struct arp_t* arp)
{
  uint16_t arp_ptype = arp->header[ARP_PTYPE] << 8;
  arp_ptype += arp->header[ARP_PTYPE + 1];

  if (arp_ptype == 0x0800)
  {
    uint8_t i;
    uint32_t target_ip = 0;
    for (i = 0 ; i < 4 ; i++)
      target_ip += arp->header[ARP_TPA + i] << (24 - (i * 8));
    if (target_ip == rtl8139_device->ip_addr)
    {
      kprint ("ip checked... sending arp reply\n");
      send_arp_reply (arp);
    }
  }
  return;
}


void recv_arp_handler (struct arp_t* arp)
{
  if (arp->header[ARP_OPER + 1] == 1) 
    recv_arp_request (arp);
  else if (arp->header[ARP_OPER + 1] == 2)
    recv_arp_reply (arp);
  return;
}

struct arp_t* build_arp_packet (uint16_t htype, uint16_t ptype, uint8_t hlen, uint8_t plen, uint16_t oper, uint8_t* sha, uint8_t* spa, uint8_t* tha, uint8_t* tpa)
{
  uint8_t i;

  struct arp_t* arp = (struct arp_t*) kmalloc (sizeof (struct arp_t));
  arp->header[ARP_HTYPE] = (htype >> 8) & 0xff;
  arp->header[ARP_HTYPE + 1] = htype & 0xff;
  
  arp->header[ARP_PTYPE] = (ptype >> 8) & 0xff;
  arp->header[ARP_PTYPE + 1] = ptype & 0xff;

  arp->header[ARP_HLEN] = hlen;
  arp->header[ARP_PLEN] = plen;

  arp->header[ARP_OPER] = (oper >> 8) & 0xff;
  arp->header[ARP_OPER + 1] = oper & 0xff;

  for (i = 0 ; i < hlen; i++)
    arp->header[ARP_SHA + i] = sha[i];
  kprintf ("sha: %x\n", 1, sha);

  for (i = 0 ; i < plen ; i++)
    arp->header[ARP_SPA + i] = spa[i];

  for (i = 0 ; i < hlen; i++)
    if (tha)
      arp->header[ARP_THA + i] = tha[i];
    else
      arp->header[ARP_THA + i] = 0xff;

  for (i = 0 ; i < plen ; i++)
    arp->header[ARP_TPA + i] = tpa[i];

  return arp;
}
