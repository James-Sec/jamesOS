#include <l1_interface.h>

void l1_upper_interface (uint8_t *data, uint32_t data_size, uint32_t driver_id)
{
  switch (driver_id)
  {
    case L1_RTL8139_ID:
      rtl8139_send_frame (data, data_size);
      break;
  }
}
