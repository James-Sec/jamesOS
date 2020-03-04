#ifndef PCI_H
#define PCI_H

#include "ports.h"
#include "screen.h"
#include "../libc/string.h"

uint32_t pci_read_data (uint8_t bus, uint8_t device, uint8_t offset);
void pci_write_data (uint32_t address, uint32_t data);
uint32_t pci_gen_address (uint8_t bus, uint8_t device, uint8_t offset);
void pci_brute ();

#endif
