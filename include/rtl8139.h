#ifndef RTL8139_H
#define RTL8139_H

#include <stdint.h>
#include "pci.h"

#define VENDOR_ID 0x10EC 
#define DEVICE_ID 0x8139

void rtl8139_init ();
#endif
