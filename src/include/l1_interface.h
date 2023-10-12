#ifndef L1_INTERFACE_H
#define L1_INTERFACE_H

#define L1_RTL8139_ID 0x10ec8139

#include <rtl8139.h>
#include <stdint.h>

void l1_upper_interface (uint8_t *data, uint32_t data_size, uint32_t driver_id);

#endif

