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

void recv_icmp4_packet (uint8_t *packet, uint32_t size, uint32_t ip)
{
  struct icmp4* icmp4_packet = l3_interface_recv_icmp4 (packet, size);
  
  if (((uint8_t*)icmp4_packet + ICMP4_TYPE_OFFSET) == ICMP4_ECHO_REQUEST_TYPE)
  {
    icmp4_echo_request_handler ();
  }
  else if (((uint8_t*)icmp4_packet + ICMP4_TYPE_OFFSET) == ICMP4_ECHO_REPLY_TYPE)
  {
    kprintf ("echo reply received from: %x\n", 1, ip);
  }
  else
  {
    kprintf ("undefined icmp4 packet type: %x\n", 1, (uint8_t*)icmp4_packet + ICMP4_TYPE_OFFSET)
  }
}

static void icmp4_echo_request_handler (struct icmp4 *echo_request_packet, uint32_t ip)
{
  struct icmp4 *icmp4_echo_reply_packet = build_icmp4_packet (ICMP4_ECHO_REPLY_TYPE, ICMP4_ECHO_REPLY_CODE, 0x1337, *((uint32_t*)(echo_request_packet->header + ICMP4_REST_OF_HEADER_OFFSET)));
  icmp4_echo_reply_packet->data = "jaaaames";
  uint8_t mac_dest_addr [] = {0x12, 0xa3, 0xab, 0x41, 0x6e, 0x12};
  send_icmp4_packet (icmp4_echo_reply_packet, ICMP4_HEADER_SIZE + 8, ip, mac_dest_addr);
  // TODO we stopped here

  
}
