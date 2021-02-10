#include <network_utils.h>

void htonb (uint8_t* byte, uint8_t bits)
{
	uint8_t tmp = 0;

	uint8_t i = 0;
	for (; i < bits; i++)
		tmp += (((*byte) >> (7 - i)) & 1) << ((bits - 1) - i);

	i = 0;
	for (; i < (8 - bits); i++)
		tmp += (((*byte) >> i) & 1) << ((bits) + i);

	*byte = tmp;
}

void ntohb (uint8_t* byte, uint8_t bits)
{
	htonb (byte, 8 - bits);
}

void htons (uint16_t* shrt)
{
	uint16_t tmp = 0;

	tmp += (0xff & *shrt) << 8;
	tmp += (0xff00 & *shrt) >> 8;

	*shrt = tmp;
}

void ntohs (uint16_t* shrt)
{
  htons (shrt);
}

void htonl (uint32_t* lng)
{
	uint32_t tmp = 0;

	tmp += (0xff000000 & *lng) >> 24;
	tmp += (0x00ff0000 & *lng) >> 8;
	tmp += (0x0000ff00 & *lng) << 8;
	tmp += (0x000000ff & *lng) << 24;

	kprintf ("tmp: %x\n", 1, tmp);

	*lng = tmp;
}

void ntohl (uint32_t* lng)
{
  htonl (lng);
}

uint8_t* l3_interface_send (uint8_t* header, uint32_t header_size, uint8_t* data, uint32_t data_size)
{
  uint8_t* packet = kmalloc_u (header_size + data_size);

  memcpy (header, packet, header_size);
  memcpy (data, packet + header_size, data_size);

  return packet;
}

struct ip_packet* l3_interface_recv (uint8_t* packet)
{
  struct ip_packet *ip = kmalloc_u (sizeof (struct ip_packet));
  memcpy (packet, ip->header.ipv4.ipv4, IPv4_HEADER_SIZE);
  uint32_t size = ip_get_attr_value (ip->header.ipv4.ipv4, IPv4_TOTAL_LENGTH_OFFSET, IPv4_TOTAL_LENGTH_SIZE) - IPv4_HEADER_SIZE;
  memcpy (packet + IPv4_HEADER_SIZE, ip->data, size);

  return ip;
}

struct icmp4* l3_interface_recv_icmp4 (uint8_t* packet, uint32_t size)
{
  struct icmp4 *icmp4_packet = kmalloc (sizeof (struct icmp4));
  memcpy (packet, icmp4_packet, ICMP4_HEADER_SIZE);

  icmp4_packet->data = kmalloc (sizeof (size - ICMP4_HEADER_SIZE));
  memcpy (packet + ICMP4_HEADER_SIZE, icmp4_packet->data, size - ICMP4_HEADER_SIZE);

  return icmp4_packet;
}

struct ether_frame* l2_interface_recv_ethernet2 (uint8_t* packet, uint32_t size)
{
  /*
  struct ether_frame* ether = kmalloc_u (sizeof (struct ether_frame));
  memcpy (packet, (uint8_t*)ether, size);

  ether->data_size = size;

  return ether;
  */
  return 0;
}

struct ether_frame* l2_interface_send_ethernet2 (struct ip_packet* ip, uint8_t* mac_dest_addr)
{
  /*
	uint16_t ip_size = 0;
	ip_size = ip->header.ipv4.ipv4 [2] << 8;
	ip_size += ip->header.ipv4.ipv4 [3];
  
  struct ether_frame* ether = build_ether_frame (mac_dest_addr, ETHER_TYPE_IPV4, ip, IPv4_HEADER_SIZE);
  // ip_packet->data is a pointer, so it is needed to be copied separatetly
  memcpy (ip->data, ether->data + IPv4_HEADER_SIZE, ip_size - IPv4_HEADER_SIZE);

  return ether;
  */
  return 0;
}

uint8_t* l1_interface_send_rtl8139 (struct ether_frame* ether)
{
  return (uint8_t*)ether;
}
