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

struct icmp4* l3_interface_recv_icmp4 (uint8_t* packet, uint32_t size)
{
  struct icmp4 *icmp4_packet = kmalloc (sizeof (struct icmp4));
  memcpy (packet, icmp4_packet, ICMP4_HEADER_SIZE);

  icmp4_packet->data = kmalloc (sizeof (size - ICMP4_HEADER_SIZE));
  memcpy (packet + ICMP4_HEADER_SIZE, icmp4_packet->data, size - ICMP4_HEADER_SIZE);

  return icmp4_packet;
}
