#include "../include/rtl8139.h"

void rtl8139_handler (registers_t *regs)
{
  uint32_t io_base = 49152;
  uint16_t status = port_word_in ((uint16_t)(io_base + 0x3e));
  if(status & (1<<2)) {
    kprint("Packet sent\n");
  }
  if (status & (1<<0)) {
    kprint("Received packet\n");
  }
}

void rtl8139_init ()
{
  uint16_t bus;
  uint8_t device;
  uint32_t io_base;
  uint32_t rx_buffer[1024];
  kprint ("initing rtl8139\n");
  //find device
  pci_get_device ((uint16_t)VENDOR_ID, (uint16_t)DEVICE_ID, &bus, &device);
  
  //enabling PCI bus mastering (allow nic to perform DMA)
  uint32_t reg = pci_read_data (bus, device, 0x04);
  reg |= 0x04;
  pci_write_data (bus, device, 0x04, reg);
  kprint ("PCI bus mastering enable\n");

  //get io base address
  reg = pci_read_data (bus, device, 0x10);
  io_base = reg & ~(0x03);

  //turning on the rtl8139
  port_byte_out ((uint16_t)(io_base + 0x52), 0x0);

  //software reset
  port_byte_out ((uint16_t)(io_base + 0x37), 0x10);
  while ((port_byte_in ((uint16_t)(io_base + 0x37)) & 0x10) != 0){}

  //init / allocate receive buffer
  //obs.: We can use the heap at this point
  port_dword_out ((uint16_t)(io_base + 0x30), (uintptr_t)rx_buffer);

  // Sets the TOK and ROK bits high
  port_word_out ((uint16_t)(io_base + 0x3C), 0x0005);

  // (1 << 7) is the WRAP bit, 0xf is AB+AM+APM+AAP
  port_dword_out ((uint16_t)(io_base + 0x44), 0xf | (1 << 7));

  // Sets the RE and TE bits high
  port_byte_out ((uint16_t)(io_base + 0x37), 0x0C);

  // register and enable network interruptions
  reg = pci_read_data (bus, device, 0x3c);
  register_interrupt_handler (IRQ11, rtl8139_handler);
  kprint ("registered\n");

  //port_word_out ((uint16_t)(io_base + 0x3E), 0x1);
  

  char s[10];
  itoa (reg & 0xff, s);
  kprint ("interrupt line: ");
  kprint (s);
  kprint ("\n");
}
