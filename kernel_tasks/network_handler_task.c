#include <network_handler_task.h>

void network_handler(uint8_t *packet_pointer, uint32_t packet_size)
{
  task_entry();
  //l2_lower_interface(packet_pointer, packet_size, L2_PROTOCOL_ETHERNET2);
  task_termination();
}
