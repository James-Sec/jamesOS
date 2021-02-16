#ifndef L3_INTERFACE_H
#define L3_INTERFACE_H

#define L3_PROTOCOL_IPv4 0

#include <ethernet.h>
#include <rtl8139.h>
#include <stdarg.h>
#include <l2_interface.h>
#include <ip.h>


void l3_upper_interface (uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t protocol);

void l3_lower_interface (uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t protocol);

#endif
