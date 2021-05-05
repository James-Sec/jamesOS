#include <network_handler_task.h>

void network_handler(uint8_t *packet_pointer, uint32_t packet_size)
{
  task_entry();
  kprintf("packet_pointer: %x\n", 1, packet_pointer);
  kprintf("packet_size: %x\n", 1, packet_size);
  uint8_t* frame = kmalloc_u (packet_size);
  memcpy (packet_pointer, frame, packet_size);

  l2_lower_interface (frame, packet_size, L2_PROTOCOL_ETHERNET2);

  kfree (frame, packet_size);
  task_termination();
}
