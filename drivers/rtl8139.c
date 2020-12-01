#include <rtl8139.h>


uint8_t TSAD_array[4] = {0x20, 0x24, 0x28, 0x2C};
uint8_t TSD_array[4] = {0x10, 0x14, 0x18, 0x1C};


static void rtl8139_receive_frame ()
  // specific implementation for test
{
  /*
  char s[10];
  int i;
  kprint_debug ("Data bytes: \n", LIGHT_BLUE);
  for (i = 0; i <= 256; i++)
  {
    itoa (*(rtl8139_device->rx_buffer + i), s);
    kprint_debug (s, LIGHT_BLUE);
    kprint_debug (" ", LIGHT_BLUE);
  }
  kprint_debug ("\n", LIGHT_BLUE);
  kprint_debug ("address: ", LIGHT_BLUE);
  itoa ((uint32_t) rtl8139_device->rx_buffer, s);
  kprint_debug (s, LIGHT_BLUE);
  kprint ("\n");
  */
  kprint_debug ("frame received, passed to ether_handler\n", LIGHT_BLUE);
  recv_ether_frame ((struct ether_frame*) ((uint8_t*)rtl8139_device->rx_buffer+4));
}

static void rtl8139_handler (registers_t *regs)
{
  uint16_t status = port_word_in ((uint16_t)(rtl8139_device->io_base + 0x3e));
  if(status & (1<<2))
  {
    kprint("Packet sent\n");
    port_word_out (rtl8139_device->io_base + 0x3E, 0x4);
  }
  if (status & (1<<0))
  {
    kprint("Packet received\n");
    rtl8139_receive_frame ();
    port_word_out ((uint16_t)(rtl8139_device->io_base + 0x3E), 0x1);
  }
}

void rtl8139_send_frame (struct ether_frame* frame)
{
  port_dword_out (rtl8139_device->io_base + TSAD_array[rtl8139_device->tx_cur], frame);
  port_dword_out (rtl8139_device->io_base + TSD_array[rtl8139_device->tx_cur++], frame->data_size + 14);
  if(rtl8139_device->tx_cur > 3)
    rtl8139_device->tx_cur = 0;
}

void rtl8139_init ()
{
  rtl8139_device = (struct rtl8139_dev*) kmalloc (sizeof (struct rtl8139_dev));
  uint16_t bus;
  uint8_t device;
  rtl8139_device->tx_cur = 0;
  // find device
  pci_get_device ((uint16_t)VENDOR_ID, (uint16_t)DEVICE_ID, &bus, &device);

  // enabling PCI bus mastering (allow nic to perform DMA)
  uint32_t reg = pci_read_data (bus, device, 0x04); 
  reg |= 0x04;
  pci_write_data (bus, device, 0x04, reg);

  // get io base address
  reg = pci_read_data (bus, device, 0x10); 
  rtl8139_device->io_base = reg & (~0x03);  

  // turning on the rtl8139
  port_byte_out ((uint16_t)(rtl8139_device->io_base + 0x52), 0x0); 

  // software reset
  port_byte_out ((uint16_t)(rtl8139_device->io_base + 0x37), 0x10);
  while (port_byte_in ((uint16_t)(rtl8139_device->io_base + 0x37)) & 0x10);

  // TODO FIX RTL8139 BUFFER 
  // TODO FIGURE OUT WHERE WE PASS THE SIZE FOR THE RTL8139
  rtl8139_device->rx_buffer = (uint8_t*) virtual2phys (kernel_directory, kmalloc_a (8192 + 0x1000));
  memset (rtl8139_device->rx_buffer, 0, 8192 + 0x1000);
  port_dword_out ((uint16_t)(rtl8139_device->io_base + 0x30), (uint32_t)rtl8139_device->rx_buffer);

  // Sets the TOK and ROK bits high
  port_word_out ((uint16_t)(rtl8139_device->io_base + 0x3C), 0x0005);

  // (1 << 7) is the WRAP bit, 0xf is AB+AM+APM+AAP
  port_dword_out ((uint16_t)(rtl8139_device->io_base + 0x44), 0xf | (1 << 7));

  // Sets the RE and TE bits high
  port_byte_out ((uint16_t)(rtl8139_device->io_base + 0x37), 0x0C);

  // register and enable network interruptions
  reg = pci_read_data (bus, device, 0x3c) & 0xff;
  register_interrupt_handler (reg + 32, rtl8139_handler);

  // get mac addr
  uint8_t i;
  for (i = 0; i < 6; i++)
    rtl8139_device->mac_addr[i] = port_byte_in (rtl8139_device->io_base + i);

  // set ip addr
  set_ip_addr (0x01020304);
}
