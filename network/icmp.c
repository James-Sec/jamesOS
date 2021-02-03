#include <icmp.h>

struct icmp4* build_icmp4_packet (uint8_t type, uint8_t code, uint16_t checksum, uint32_t rest_of_header)
{
  struct icmp4* icmp = (struct icmp4*)kmalloc_u (sizeof (struct icmp4));
  memcpy (&type, ((uint8_t*)icmp) + ICMP4_TYPE_OFFSET, ICMP4_TYPE_SIZE);
  memcpy (&code, ((uint8_t*)icmp) + ICMP4_CODE_OFFSET, ICMP4_CODE_SIZE);
  memcpy (&checksum, ((uint8_t*)icmp) + ICMP4_CHECKSUM_OFFSET, ICMP4_CHECKSUM_SIZE);
  memcpy (&rest_of_header, ((uint8_t*)icmp) + ICMP4_REST_OF_HEADER_OFFSET, ICMP4_REST_OF_HEADER_SIZE);

  return icmp;
}

void send_icmp4_packet (struct icmp4* icmp, uint32_t icmp_size, uint32_t dest_ip, uint8_t dest_mac[6])
{
  uint8_t* packet = l3_interface_send (icmp->header, ICMP4_HEADER_SIZE, icmp->data, icmp_size - ICMP4_HEADER_SIZE);
  struct ip_packet* ip = build_ipv4_packet (dest_ip, packet, icmp_size);
  send_ipv4_packet (ip, dest_mac);
}
