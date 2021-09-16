#ifndef L5_INTERFACE_H
#define L5_INTERFACE_H

#define L5_PROTOCOL_JNP 0

#include <stdint.h>
#include <stdarg.h>

void l5_upper_interface(uint16_t port, uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t protocol, ...);

#endif
