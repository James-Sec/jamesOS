#include <icmp.h>

struct icmp4* build_icmp4 (struct icmp4* icmp, uint8_t type, uint8_t code, uint32_t rest_of_header, uint8_t *data, uint32_t data_size)
{
  icmp->data = kmalloc_u (data_size);
  memcpy (data, icmp->data, data_size);

  set_bytes_attr_value (icmp->header, ICMP4_TYPE_OFFSET, ICMP4_TYPE_SIZE, &type);
  set_bytes_attr_value (icmp->header, ICMP4_CODE_OFFSET, ICMP4_CODE_SIZE, &code);
  set_bytes_attr_value (icmp->header, ICMP4_REST_OF_HEADER_OFFSET, ICMP4_REST_OF_HEADER_SIZE, &rest_of_header);

  uint16_t checksum = 0;
  set_bytes_attr_value (icmp->header, ICMP4_CHECKSUM_OFFSET, ICMP4_CHECKSUM_SIZE, &checksum);
  checksum = internet_checksum (icmp->header, ICMP4_HEADER_SIZE, icmp->data, data_size);
  set_bytes_attr_value (icmp->header, ICMP4_CHECKSUM_OFFSET, ICMP4_CHECKSUM_SIZE, &checksum);

  return icmp;
}

void send_icmp4_packet (uint32_t ip, uint8_t mac[6], uint8_t type, uint8_t code, uint32_t rest_of_header, uint8_t *data, uint32_t data_size)
{
  struct icmp4 *icmp = kmalloc_u (sizeof (struct icmp4));
  icmp->data = kmalloc_u (data_size);
  build_icmp4 (icmp, type, code, rest_of_header, data, data_size);

  uint8_t *array = icmp4_to_array (icmp, data_size);

  send_ipv4_packet (ip, mac, array, data_size + ICMP4_HEADER_SIZE, IPv4_DSCP_DF, 0, IPv4_PROTOCOL_ICMP4);

  kfree (icmp->data, data_size);
  kfree (icmp, sizeof (struct icmp4));
  kfree (array, ICMP4_HEADER_SIZE + data_size);
}

void recv_icmp4_packet (uint8_t mac[6], uint32_t ip, struct icmp4* icmp, uint32_t size)
{
  uint8_t type;
  get_bytes_attr_value (icmp->header, ICMP4_TYPE_OFFSET, ICMP4_TYPE_SIZE, &type);
  uint32_t rest_of_header = 0;
  uint32_t data_size = size - ICMP4_HEADER_SIZE;
  uint8_t* data = kmalloc_u (data_size);

  kprint("header: ");
  for (int i = 0; i < ICMP4_HEADER_SIZE; i++)
    kprintf ("%x ", 1, icmp->header[i]);
  kprint("\ndata: ");
  for (int i = ICMP4_HEADER_SIZE; i < size; i++)
    kprintf ("%x ", 1, icmp->data[i]);
  kprint("\n");

  switch (type)
  {
    case ICMP4_ECHO_REQUEST_TYPE:
      kprint ("sending echo reply...\n");
      get_bytes_attr_value (icmp->header, ICMP4_REST_OF_HEADER_OFFSET, ICMP4_REST_OF_HEADER_SIZE, &rest_of_header);
      memcpy (icmp->data, data, data_size);

      send_icmp4_packet (ip, mac, ICMP4_ECHO_REPLY_TYPE, ICMP4_ECHO_REPLY_CODE, rest_of_header, data, size);
      break;
  }
  kfree (icmp->data, data_size);
  //kfree (icmp, sizeof (struct icmp4));
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
