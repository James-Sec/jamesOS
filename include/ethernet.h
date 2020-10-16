#ifndef ETHERNET.H
#define ETHERNET.H

#include <stdint.h>
#include "../include/rtl8139.h"

struct ether_frame
{
  uint8_t destination_addr [6];
  uint8_t source_addr [6];
  uint8_t ether_type [2];
  uint8_t data [1500];
  uint32_t data_size;
};

struct ether_frame* build_ether_frame (uint8_t dest [6], uint8_t type [2], uint8_t *data, uint32_t size);
#endif
