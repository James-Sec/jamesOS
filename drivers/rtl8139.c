#include "../include/rtl8139.h"

void rtl8139_init ()
{
  kprint ("initing rtl8139\n");
  //find device
  uint16_t bus;
  uint8_t device;
  pci_get_device ((uint16_t)VENDOR_ID, (uint16_t)DEVICE_ID, &bus, &device);
  
  //enabling PCI bus mastering (allow nic to perform DMA)
  uint32_t reg = pci_read_data (bus, device, 0x04);
  reg |= 0x04;
  pci_write_data (bus, device, 0x04, reg);
  kprint ("PCI bus mastering enable\n");

  //get io base address
  reg = pci_read_data (bus, device, 0x10);

  char s[10];
  itoa (reg, s);
  kprint ("base addr: ");
  kprint (s);
  kprint ("\n");
}
