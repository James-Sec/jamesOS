#ifndef L2_INTERFACE_H
#define L2_INTERFACE_H

#define L2_PROTOCOL_ETHERNET2 0

#include <ethernet.h>
#include <rtl8139.h>
#include <stdarg.h>
#include <l3_interface.h>
#include <stdint.h>


void l2_upper_interface (uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t protocol, ...);

void l2_lower_interface (uint8_t *data, uint32_t data_size);


#endif
