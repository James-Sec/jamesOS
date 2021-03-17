#include <ip.h>

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

struct ipv4_packet* build_ipv4_packet (struct ipv4_packet *ip, uint8_t version, uint8_t ihl, uint8_t dscp, uint8_t ecn, uint16_t total_length, uint16_t identification, uint8_t flags, uint16_t fragment_offset, uint8_t time_to_live, uint8_t protocol, uint32_t source_ip, uint32_t destination_ip, uint8_t* data)
{
	//setting on network order
	set_bits_attr_value (ip->header, IPv4_VERSION_OFFSET, IPv4_VERSION_SIZE, version);
	set_bits_attr_value (ip->header, IPv4_IHL_OFFSET, IPv4_IHL_SIZE, ihl);
	set_bits_attr_value (ip->header, IPv4_DSCP_OFFSET, IPv4_DSCP_SIZE, dscp);
	set_bits_attr_value (ip->header, IPv4_ECN_OFFSET, IPv4_ECN_SIZE, ecn);
	set_bits_attr_value (ip->header, IPv4_TOTAL_LENGTH_OFFSET, IPv4_TOTAL_LENGTH_SIZE, total_length);
	set_bits_attr_value (ip->header, IPv4_IDENTIFICATION_OFFSET, IPv4_IDENTIFICATION_SIZE, identification);
	set_bits_attr_value (ip->header, IPv4_FLAGS_OFFSET, IPv4_FLAGS_SIZE, flags);
	set_bits_attr_value (ip->header, IPv4_FRAGMENT_OFFSET_OFFSET, IPv4_FRAGMENT_OFFSET_SIZE, fragment_offset);
	set_bits_attr_value (ip->header, IPv4_TIME_TO_LIVE_OFFSET, IPv4_TIME_TO_LIVE_SIZE, time_to_live);
	set_bits_attr_value (ip->header, IPv4_PROTOCOL_OFFSET, IPv4_PROTOCOL_SIZE, protocol);
	set_bits_attr_value (ip->header, IPv4_SOURCE_IP_ADDRESS_OFFSET, IPv4_SOURCE_IP_ADDRESS_SIZE, source_ip);
	set_bits_attr_value (ip->header, IPv4_DESTINATION_IP_ADDRESS_OFFSET, IPv4_DESTINATION_IP_ADDRESS_SIZE, destination_ip);
	set_bits_attr_value (ip->header, IPv4_HEADER_CHECKSUM_OFFSET, IPv4_HEADER_CHECKSUM_SIZE, 0x0);

  uint16_t checksum = internet_checksum (ip->header, IPv4_HEADER_SIZE, 0, 0);
	set_bits_attr_value (ip->header, IPv4_HEADER_CHECKSUM_OFFSET, IPv4_HEADER_CHECKSUM_SIZE, checksum);
  ip->data = data;
	return ip;
}

void recv_ipv4_packet (uint8_t mac[6], uint8_t *data, uint32_t size)
{
  struct ipv4_packet *packet = kmalloc_u (sizeof (struct ipv4_packet));
  packet = array_to_ipv4 (packet, data, size);
  switch (get_bits_attr_value (packet, IPv4_PROTOCOL_OFFSET, IPv4_PROTOCOL_SIZE))
  {
    case IPv4_PROTOCOL_ICMP4:
      kprint ("ICMP RECEIVED\n");
      kprint ("ICMP RECEIVED\n");
      kprint ("ICMP RECEIVED\n");
      kprint ("ICMP RECEIVED\n");
      kprint ("ICMP RECEIVED\n");
      //recv_icmp_packet();
      break;
  }

}

void send_ipv4_packet (uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t dscp, uint8_t ecn, uint8_t protocol)
{
  struct ipv4_packet *packet = kmalloc_u (sizeof (struct ipv4_packet));
  uint16_t identification = 0;
  uint8_t flags = 0;
  uint16_t fragment_offset = 0;
  build_ipv4_packet (packet, IPv4_VERSION, IPv4_IHL, dscp, ecn, data_size + IPv4_HEADER_SIZE, identification, flags, fragment_offset, IPv4_TTL, protocol, rtl8139_device->ip_addr, ip, data);

  uint8_t *array = ipv4_to_array (packet, data_size);
  l2_upper_interface (mac, array, IPv4_HEADER_SIZE + data_size, L2_PROTOCOL_ETHERNET2, ETHER_TYPE_IPv4);
}

void set_ip_addr (uint32_t ip)
{
  rtl8139_device->ip_addr = ip;
}

uint8_t* ipv4_to_array (struct ipv4_packet *ip_packet, uint32_t data_size)
{
  uint8_t *array = kmalloc_u (data_size + IPv4_HEADER_SIZE);
  memcpy (ip_packet, array,IPv4_HEADER_SIZE);
  memcpy (ip_packet->data, array + IPv4_HEADER_SIZE, data_size);
  
  return array;
}

struct ipv4_packet* array_to_ipv4 (struct ipv4_packet* ip_packet, uint8_t* array, uint32_t size)
{
  memcpy (array, ip_packet, IPv4_HEADER_SIZE);
  ip_packet->data = kmalloc_u (size - IPv4_HEADER_SIZE);
  memcpy (array + IPv4_HEADER_SIZE, ip_packet->data, size - IPv4_HEADER_SIZE);

  return ip_packet;
}
