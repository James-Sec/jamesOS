#include <icmp.h>

struct icmp4* build_icmp4 (struct icmp4* icmp, uint8_t type, uint8_t code, uint16_t checksum, uint32_t rest_of_header, uint8_t *data, uint32_t data_size)
{
  icmp->data = kmalloc_u (data_size);
  memcpy (data, icmp->data, data_size);

  set_bytes_attr_value (icmp->header, ICMP4_TYPE_OFFSET, ICMP4_TYPE_SIZE, &type);
  set_bytes_attr_value (icmp->header, ICMP4_CODE_OFFSET, ICMP4_CODE_SIZE, &code);
  set_bytes_attr_value (icmp->header, ICMP4_CHECKSUM_OFFSET, ICMP4_CHECKSUM_SIZE, &checksum);
  set_bytes_attr_value (icmp->header, ICMP4_REST_OF_HEADER_OFFSET, ICMP4_REST_OF_HEADER_SIZE, &rest_of_header);

  return icmp;
}

void send_icmp4_packet (uint32_t ip, uint8_t mac[6], uint8_t type, uint8_t code, uint32_t rest_of_header, uint8_t *data, uint32_t data_size)
{
  struct icmp4 *icmp = kmalloc_u (sizeof (struct icmp4));
  build_icmp4 (icmp, type, code, 0x760d, rest_of_header, data, data_size);

  uint8_t *array = icmp4_to_array (icmp, data_size);

  send_ipv4_packet (ip, mac, array, data_size + ICMP4_HEADER_SIZE, IPv4_DSCP_DF, 0, IPv4_PROTOCOL_ICMP4);
}

uint8_t* icmp4_to_array (struct icmp4 *icmp, uint32_t data_size)
{
  uint8_t *array = kmalloc_u (data_size + ICMP4_HEADER_SIZE);
  memcpy (icmp->header, array, ICMP4_HEADER_SIZE);
  memcpy (icmp->data, array + ICMP4_HEADER_SIZE, data_size);
  
  return array;
}

struct icmp4* array_to_icmp4 (struct icmp4* icmp, uint8_t* array, uint32_t size)
{
  memcpy (array, icmp, ICMP4_HEADER_SIZE);
  icmp->data = kmalloc_u (size - ICMP4_HEADER_SIZE);
  memcpy (array + ICMP4_HEADER_SIZE, icmp->data, size - ICMP4_HEADER_SIZE);

  return icmp;
}
