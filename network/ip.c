#include "../include/ip.h"

uint16_t checksum (uint8_t* ip) {
	uint32_t sz = sizeof (struct ipv4_header) / 2;
	uint16_t* arr = (uint16_t*) ip;

	uint32_t i = 0;
	uint32_t ans = 0;
	for (; i < sz; i++)
		ans += (((*arr) & 0x00ff) << 8) | (((*arr) & 0xff00) >> 8);

	uint32_t tmp = ans >> 16;
	ans = ans & 0xffff;
	ans += tmp;
	return ~ans;
}

void aux (uint8_t* attr, uint32_t offset, uint32_t size, uint32_t value) {
	uint32_t i = offset;
	uint8_t cnt = 0;
	for (; i < (offset + size); i++) 
	{
		uint8_t byte = i / 8;
		uint8_t bit  = i % 8;

		if ((value >> cnt++) & 1)
			*(attr + byte) |= (1 << (bit)); 
		else
			*(attr + byte) &= ((1 << 8) - 1) - (1 << (bit)); 
	}
}

void htonb (uint8_t* byte, uint8_t bits) {
	uint8_t tmp = 0;

	uint8_t i = 0;
	for (; i < bits; i++)
		tmp += (((*byte) >> (7 - i)) & 1) << ((bits - 1) - i);

	i = 0;
	for (; i < (8 - bits); i++)
		tmp += (((*byte) >> i) & 1) << ((bits) + i);

	*byte = tmp;
}

void htons (uint16_t* shrt) {
	uint16_t tmp = 0;

	tmp += (0xff & *shrt) << 8;
	tmp += (0xff00 & *shrt) >> 8;

	*shrt = tmp;
}

void htonl (uint32_t* lng) {
	uint32_t tmp = 0;

	tmp += (0xff000000 & *lng) >> 24;
	tmp += (0x00ff0000 & *lng) >> 8;
	tmp += (0x0000ff00 & *lng) << 8;
	tmp += (0x000000ff & *lng) << 24;

	kprintf ("tmp: %x\n", 1, tmp);

	*lng = tmp;
}

struct ip_packet* build_ipv4_packet (uint8_t data [20], uint16_t len) {
	struct ip_packet* ip = (struct ip_packet*)kmalloc (sizeof (struct ip_packet));


	//setting to network order

	aux (ip->header.ipv4.ipv4, IPv4_VERSION_OFFSET, IPv4_VERSION_SIZE, 4);
	aux (ip->header.ipv4.ipv4, IPv4_IHL_OFFSET, IPv4_IHL_SIZE, 5);
	htonb (ip->header.ipv4.ipv4, 4);

	aux (ip->header.ipv4.ipv4, IPv4_DSCP_OFFSET, IPv4_DSCP_SIZE, 127);
	aux (ip->header.ipv4.ipv4, IPv4_ECN_OFFSET, IPv4_ECN_SIZE, 0);
	htonb (&ip->header.ipv4.ipv4 [1], 2);

	aux (ip->header.ipv4.ipv4, IPv4_TOTAL_LENGTH_OFFSET, IPv4_TOTAL_LENGTH_SIZE, 27);
	htons ((uint16_t*)&ip->header.ipv4.ipv4 [2]);

	aux (ip->header.ipv4.ipv4, IPv4_IDENTIFICATION_OFFSET, IPv4_IDENTIFICATION_SIZE, 37);
	htons ((uint16_t*)&ip->header.ipv4.ipv4 [4]);

	aux (ip->header.ipv4.ipv4, IPv4_FLAGS_OFFSET, IPv4_FLAGS_SIZE, 0);
	aux (ip->header.ipv4.ipv4, IPv4_FRAGMENT_OFFSET_OFFSET, IPv4_FRAGMENT_OFFSET_SIZE, 2);
	htonb (&ip->header.ipv4.ipv4 [6], 5);

	aux (ip->header.ipv4.ipv4, IPv4_TIME_TO_LIVE_OFFSET, IPv4_TIME_TO_LIVE_SIZE, 19);
	aux (ip->header.ipv4.ipv4, IPv4_PROTOCOL_OFFSET, IPv4_PROTOCOL_SIZE, 0x6);

	aux (ip->header.ipv4.ipv4, IPv4_SOURCE_IP_ADDRESS_OFFSET, IPv4_SOURCE_IP_ADDRESS_SIZE, 16909060);
	htonl ((uint32_t*)&ip->header.ipv4.ipv4 [12]);

	aux (ip->header.ipv4.ipv4, IPv4_DESTINATION_IP_ADDRESS_OFFSET, IPv4_DESTINATION_IP_ADDRESS_SIZE, 16909060);
	htonl ((uint32_t*)&ip->header.ipv4.ipv4 [16]);

	aux (ip->header.ipv4.ipv4, IPv4_HEADER_CHECKSUM_OFFSET, IPv4_HEADER_CHECKSUM_SIZE, 0x0);
	aux (ip->header.ipv4.ipv4, IPv4_HEADER_CHECKSUM_OFFSET, IPv4_HEADER_CHECKSUM_SIZE, checksum (ip->header.ipv4.ipv4));
	htons ((uint16_t*)&ip->header.ipv4.ipv4 [10]);

	// -20, look at union struct it allocs the bigger type
	memcpy (data, ip->data - 20, len);
	return ip;
}
