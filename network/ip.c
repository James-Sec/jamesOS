#include <ip.h>

static uint16_t checksum (uint8_t* ip)
{
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

void print_bit_ipv4 (uint8_t* header)
{
  uint8_t *i = header;
  for (; i < (header + 20); i++)
  {
    int8_t j;
    for (j = 7 ; j >= 0 ; j--)
    {
      if (((uint8_t)*i >> j) & 1)
        kprint_debug ("1", CYAN);
      else
        kprint_debug ("0", CYAN);
    }
		kprint_debug (" ", CYAN);
  }
	kprint ("\n");
}

static void set_attr_value (uint8_t* attr, uint32_t offset, uint32_t size, uint32_t value)
{
	uint32_t i = offset;
	uint8_t cnt = size - 1;
	for (; i < (offset + size); i++) 
	{
		uint8_t byte = i / 8;
		uint8_t bit = 7 - (i % 8);

		if ((value >> cnt--) & 1)
			*(attr + byte) |= (1 << bit);
		else
			*(attr + byte) &= ~(1 << bit);
	}
}

struct ip_packet* build_ipv4_packet (uint32_t source_ip, uint32_t destination_ip, uint8_t* data, uint16_t data_len)
{
	return _build_ipv4_packet (4, 5, 127, 0, 20 + data_len, 0x1337, 0, 0, 128, 6, source_ip, destination_ip, data, data_len);
}

struct ip_packet* _build_ipv4_packet (uint8_t version, uint8_t ihl, uint8_t dscp, uint8_t ecn, uint16_t total_length, uint16_t identification, uint8_t flags, uint16_t fragment_offset, uint8_t time_to_live, uint8_t protocol, uint32_t source_ip, uint32_t destination_ip, uint8_t* data, uint16_t data_len)
{
	struct ip_packet* ip = (struct ip_packet*)kmalloc (sizeof (struct ip_packet));
	//setting on network order
	set_attr_value (ip->header.ipv4.ipv4, IPv4_VERSION_OFFSET, IPv4_VERSION_SIZE, version);
	set_attr_value (ip->header.ipv4.ipv4, IPv4_IHL_OFFSET, IPv4_IHL_SIZE, ihl);
	set_attr_value (ip->header.ipv4.ipv4, IPv4_DSCP_OFFSET, IPv4_DSCP_SIZE, dscp);
	set_attr_value (ip->header.ipv4.ipv4, IPv4_ECN_OFFSET, IPv4_ECN_SIZE, ecn);
	set_attr_value (ip->header.ipv4.ipv4, IPv4_TOTAL_LENGTH_OFFSET, IPv4_TOTAL_LENGTH_SIZE, 20 + data_len);
	set_attr_value (ip->header.ipv4.ipv4, IPv4_IDENTIFICATION_OFFSET, IPv4_IDENTIFICATION_SIZE, identification);
	set_attr_value (ip->header.ipv4.ipv4, IPv4_FLAGS_OFFSET, IPv4_FLAGS_SIZE, flags);
	set_attr_value (ip->header.ipv4.ipv4, IPv4_FRAGMENT_OFFSET_OFFSET, IPv4_FRAGMENT_OFFSET_SIZE, fragment_offset);
	set_attr_value (ip->header.ipv4.ipv4, IPv4_TIME_TO_LIVE_OFFSET, IPv4_TIME_TO_LIVE_SIZE, time_to_live);
	set_attr_value (ip->header.ipv4.ipv4, IPv4_PROTOCOL_OFFSET, IPv4_PROTOCOL_SIZE, protocol);
	set_attr_value (ip->header.ipv4.ipv4, IPv4_SOURCE_IP_ADDRESS_OFFSET, IPv4_SOURCE_IP_ADDRESS_SIZE, source_ip);
	set_attr_value (ip->header.ipv4.ipv4, IPv4_DESTINATION_IP_ADDRESS_OFFSET, IPv4_DESTINATION_IP_ADDRESS_SIZE, destination_ip);
	set_attr_value (ip->header.ipv4.ipv4, IPv4_HEADER_CHECKSUM_OFFSET, IPv4_HEADER_CHECKSUM_SIZE, 0x0);
	set_attr_value (ip->header.ipv4.ipv4, IPv4_HEADER_CHECKSUM_OFFSET, IPv4_HEADER_CHECKSUM_SIZE, checksum (ip->header.ipv4.ipv4));

	// -20, look at union struct it allocs the bigger type
	memcpy (data, ip->data - 20, data_len);
	return ip;
}

void recv_ipv4_handler (struct ip_packet* ip)
{
  kprintf ("ipv4_destination_addr: %d.%d.%d.%d\n", 4, ip->header.ipv4.ipv4 [12], ip->header.ipv4.ipv4 [13], ip->header.ipv4.ipv4 [14], ip->header.ipv4.ipv4 [15]);

  kprintf ("ipv4_source_addr: %d.%d.%d.%d\n", 4, ip->header.ipv4.ipv4 [16], ip->header.ipv4.ipv4 [17], ip->header.ipv4.ipv4 [18], ip->header.ipv4.ipv4 [19]);
}

void send_ipv4_packet (struct ip_packet* ip, uint8_t* mac_dest_addr)
{
	uint16_t ip_size = 0;
	ip_size = ip->header.ipv4.ipv4 [2] << 8;
	ip_size += ip->header.ipv4.ipv4 [3];

  struct ether_frame* frame = build_ether_frame (mac_dest_addr, ETHER_TYPE_IPV4, ip, ip_size);
  send_ether_frame (frame);
}
