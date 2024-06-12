#ifndef PCI_H
#define PCI_H

#define PCI_CONFIG_ADDR 0xcf8
#define PCI_CONFIG_DATA 0xcfc

#include <ports.h>
#include <vga.h>
#include <string.h>

uint32_t pci_read_data (uint8_t bus, uint8_t device, uint8_t offset);
void pci_write_data (uint8_t bus, uint8_t device, uint8_t offset, uint32_t data);
uint32_t pci_gen_address (uint8_t bus, uint8_t device, uint8_t offset);
void pci_get_device (uint16_t vendor_id, uint16_t device_id, uint16_t *bus, uint8_t *device);
void pci_brute ();

#endif
