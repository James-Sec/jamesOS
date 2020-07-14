#ifndef RTL8139_H
#define RTL8139_H

#include <stdint.h>
#include "isr.h"
#include "pci.h"
#include "kheap.h"
#include "paging.h"

#define VENDOR_ID 0x10EC 
#define DEVICE_ID 0x8139

extern page_directory_t* kernel_directory;

void rtl8139_init ();
void rtl8139_send_frame (uint8_t *data, uint32_t len);

typedef struct rtl8139_dev
{
  uint8_t bar_type;
  uint32_t io_base;
  uint32_t mem_base;
  uint8_t mac_addr[6];
  uint8_t *rx_buffer;
  uint8_t tx_cur;
} rtl8139_dev_t;
#endif
