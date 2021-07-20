#include <ethernet.h>

struct ether_frame* build_ether_frame (struct ether_frame *frame, uint8_t dest [6], uint16_t type, uint8_t *data, uint32_t data_size)
{
  set_bytes_attr_value (frame->header, ETHER_DEST_MAC_OFFSET, ETHER_DEST_MAC_SIZE, dest);
  set_bytes_attr_value (frame->header, ETHER_SOURCE_MAC_OFFSET, ETHER_SOURCE_MAC_SIZE, rtl8139_device->mac_addr);
  set_bytes_attr_value (frame->header, ETHER_TYPE_OFFSET, ETHER_TYPE_SIZE, &type);

  memcpy (data, frame->data, data_size);

  return frame;
}

void send_ethernet_frame (uint8_t mac[6], uint8_t *data, uint32_t data_size, uint16_t type)
{
  struct ether_frame* ether = kmalloc_u (sizeof (struct ether_frame));
  ether->data = kmalloc_u (data_size);
  ether = build_ether_frame (ether, mac, type, data, data_size);

  uint8_t* ethernet_array = ethernet_to_array (ether, data_size);
  l1_upper_interface (ethernet_array, data_size + ETHER_HEADER_SIZE, L1_RTL8139_ID);

  kfree (ether->data, data_size);
  kfree (ether, sizeof (struct ether_frame));
  kfree (ethernet_array, ETHER_HEADER_SIZE + data_size);
}

void recv_ethernet_frame (uint8_t *data, uint32_t size)
{
  struct ether_frame *frame = kmalloc_u (sizeof (struct ether_frame));
  frame = array_to_ethernet (frame, data, size);
  uint16_t ether_type;
  get_bytes_attr_value (frame->header, ETHER_TYPE_OFFSET, ETHER_TYPE_SIZE, &ether_type);

  switch (ether_type)
  {
    case ETHER_TYPE_IPv4:
      ;uint8_t source_addr[6];
      get_bytes_attr_value (frame, ETHER_SOURCE_MAC_OFFSET, ETHER_SOURCE_MAC_SIZE, source_addr);
      l3_lower_interface (source_addr, frame->data, size - ETHER_HEADER_SIZE, L3_PROTOCOL_IPv4);
      break;
    case ETHER_TYPE_ARP:
      recv_arp_handler ((struct arp_t*) frame->data);
      break;
    default:
      kprint ("undefined protocol\n");
      break;
  }

  kfree (frame->data, size - ETHER_HEADER_SIZE);
  kfree (frame, sizeof (struct ether_frame));
}

uint8_t* ethernet_to_array (struct ether_frame *frame, uint32_t data_size)
{
  uint8_t *array = kmalloc_u (data_size + ETHER_HEADER_SIZE);
  memcpy (frame, array, ETHER_HEADER_SIZE);
  memcpy (frame->data, array + ETHER_HEADER_SIZE, data_size);
  
  return array;
}

struct ether_frame* array_to_ethernet (struct ether_frame* frame, uint8_t* array, uint32_t size)
{
  memcpy (array, frame, ETHER_HEADER_SIZE);
  frame->data = kmalloc_u (size - ETHER_HEADER_SIZE);
  memcpy (array + ETHER_HEADER_SIZE, frame->data, size - ETHER_HEADER_SIZE);

  return frame;
}
