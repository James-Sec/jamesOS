#include <network_handler_task.h>

void network_handler (uint32_t argc, uint8_t *argp)
{
  task_entry();

  l2_lower_interface (argp, argc, L2_PROTOCOL_ETHERNET2);
  
  task_termination(argc, argp);
}
