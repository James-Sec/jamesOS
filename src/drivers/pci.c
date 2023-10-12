#include <pci.h>

uint32_t pci_read_data (uint8_t bus, uint8_t device, uint8_t offset)
{
  uint32_t address = pci_gen_address (bus, device, offset);
  port_dword_out ((uint16_t) PCI_CONFIG_ADDR, address);
  uint32_t tmp = (uint32_t) (port_dword_in ((int16_t) PCI_CONFIG_DATA));
  return tmp;
}

void pci_write_data (uint8_t bus, uint8_t device, uint8_t offset, uint32_t data)
{
  uint32_t address = pci_gen_address (bus, device, offset);
  port_dword_out ((uint16_t)PCI_CONFIG_ADDR, address);
  port_dword_out ((uint16_t)PCI_CONFIG_DATA, data);
}

uint32_t pci_gen_address (uint8_t bus, uint8_t device, uint8_t offset)
{
  uint32_t address;
  uint32_t lbus  = (uint32_t)bus;
  uint32_t ldevice = (uint32_t)device;
  uint32_t lfunc = 0;
  uint32_t tmp = 0;

  address = (uint32_t)((lbus << 16) | (ldevice << 11) |
      (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

  return address;
}

void pci_get_device (uint16_t vendor_id, uint16_t device_id, uint16_t *bus, uint8_t *device) 
{
  int i, j;
  for (i = 0; i < 256; i++)
  {
    for (j = 0; j < 32; j++)
    {
      uint32_t data = pci_read_data (i, j, 0);
      uint32_t reg = (device_id << 16) | vendor_id;
      if (data == reg)
      {
        *bus = i;
        *device = j;
        return;
      }
    }
  }
}

void pci_brute () 
{
  int i, j;
  for (i = 0; i < 256; i++) {
    for (j = 0; j < 32; j++) {
      uint32_t data = pci_read_data (i, j, 0x8);
      if (((data >> 24) & 0x000000ff) == 0x02)
      {
        char str [10];
        itoa (data, str);
        kprint (str);
        kprint ("\n");

        data = pci_read_data (i, j, 0x0);
        itoa (data, str);
        kprint (str);
        kprint ("\n");
      }
    }
  }
}
