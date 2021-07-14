#ifndef L4_INTERFACE_H
#define L4_INTERFACE_H

#define L4_PROTOCOL_UDP 0
#include <stdint.h>
#include <stdarg.h>

void l4_upper_interface (uint16_t port, uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t protocol, ...);

void l4_lower_interface (uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t protocol);
#endif
