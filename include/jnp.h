#ifndef JNP
#define JNP

#include <stdint.h>

void jnp_send_message (uint16_t port, uint32_t ip, uint8_t mac[6], uint8_t *data, uint32_t data_size, uint8_t l4_protocol);


#endif
