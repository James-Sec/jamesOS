#include "../include/rtl8139.h"

rtl8139_dev_t rtl8139_device;

uint8_t TSAD_array[4] = {0x20, 0x24, 0x28, 0x2C};
uint8_t TSD_array[4] = {0x10, 0x14, 0x18, 0x1C};


void rtl8139_handler (registers_t *regs)
{
  uint16_t status = port_word_in ((uint16_t)(rtl8139_device.io_base + 0x3e));
  if(status & (1<<2))
  {
    kprint("Packet sent\n");
    port_word_out (rtl8139_device.io_base + 0x3E, 0x5);
  }
  if (status & (1<<0))
  {
    kprint("Received packet\n");
    char s[10];
    itoa (*(rtl8139_device.rx_buffer+=18), s);// data start
    kprint ("first data byte: ");
    kprint (s);
    kprint ("\n");
    port_word_out ((uint16_t)(rtl8139_device.io_base + 0x3E), 0x1);
  }
}

void rtl8139_send_packet (uint8_t *data, uint32_t len)
{
  port_dword_out (rtl8139_device.io_base + TSAD_array[rtl8139_device.tx_cur], (uint32_t)data);
  port_dword_out (rtl8139_device.io_base + TSD_array[rtl8139_device.tx_cur++], len);
  if(rtl8139_device.tx_cur > 3)
    rtl8139_device.tx_cur = 0;
}

void rtl8139_init ()
{
  uint16_t bus;
  uint8_t device;
  rtl8139_device.tx_cur = 0;
  kprint ("initing rtl8139\n");
  // find device
  pci_get_device ((uint16_t)VENDOR_ID, (uint16_t)DEVICE_ID, &bus, &device);
  
  // enabling PCI bus mastering (allow nic to perform DMA)
  uint32_t reg = pci_read_data (bus, device, 0x04);
  reg |= 0x04;
  pci_write_data (bus, device, 0x04, reg);
  kprint ("PCI bus mastering enable\n");

  // get io base address
  reg = pci_read_data (bus, device, 0x10);
  rtl8139_device.io_base = reg & (~0x03);

  // turning on the rtl8139
  port_byte_out ((uint16_t)(rtl8139_device.io_base + 0x52), 0x0);

  // software reset
  port_byte_out ((uint16_t)(rtl8139_device.io_base + 0x37), 0x10);
  while (port_byte_in ((uint16_t)(rtl8139_device.io_base + 0x37)) & 0x10);

  // init / allocate receive buffer
  // obs.: We can use the heap at this point
  rtl8139_device.rx_buffer = (uint8_t*)kmalloc_a (8192 + 0x1000);//recomended (8192 + 16)
  memset (rtl8139_device.rx_buffer, 0, 8192 + 0x1000);
  port_dword_out ((uint16_t)(rtl8139_device.io_base + 0x30), (uint32_t)rtl8139_device.rx_buffer);

  // Sets the TOK and ROK bits high
  port_word_out ((uint16_t)(rtl8139_device.io_base + 0x3C), 0x0005);

  // (1 << 7) is the WRAP bit, 0xf is AB+AM+APM+AAP
  port_dword_out ((uint16_t)(rtl8139_device.io_base + 0x44), 0x2 | (1 << 7));

  // Sets the RE and TE bits high
  port_byte_out ((uint16_t)(rtl8139_device.io_base + 0x37), 0x0C);

  // register and enable network interruptions
  reg = pci_read_data (bus, device, 0x3c);
  register_interrupt_handler (IRQ11, rtl8139_handler);
  kprint ("registered\n");
  char s[10];
  itoa (*rtl8139_device.rx_buffer, s);
  kprint ("first mac byte: ");
  kprint (s);
  kprint ("\n");
}
