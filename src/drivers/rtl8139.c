#include <rtl8139.h>

uint8_t TSAD_array[4] = {0x20, 0x24, 0x28, 0x2C};
uint8_t TSD_array[4] = {0x10, 0x14, 0x18, 0x1C};

static void rtl8139_reset ();

// specific implementation for test
static void rtl8139_receive_frame ()
{

  while (!((port_byte_in ((uint16_t)(rtl8139_device->io_base + 0x37))) & 0x1))
  {
    // getting packet size
    uint8_t *pckt_ptr = (uint8_t*)(rtl8139_device->rx_buffer + rtl8139_device->rx_cur);
    uint16_t pckt_sz = *(uint16_t*)(pckt_ptr + 2);
    uint16_t STATUS = *((uint16_t*)pckt_ptr);

    if (STATUS == 0xffff)
    {
      rtl8139_reset ();
      goto rx_error;
    }
    
    pckt_ptr += 4;
    pckt_sz -= 4;

    uint8_t *new_frame = kmalloc_u (pckt_sz);
    memcpy (pckt_ptr, new_frame, pckt_sz);

    nht_enqueue_recv_frame (new_frame, pckt_sz);

    kfree(new_frame, pckt_sz);

    /*
    char name[10] = "NETWORK";
    uint8_t *argp = kmalloc_u (pckt_sz);
    memcpy (pckt_ptr, argp, pckt_sz);
    l2_lower_interface (argp, pckt_sz, L2_PROTOCOL_ETHERNET2);
    */


rx_error:
    // updating the packet offset
    pckt_sz += 4;
    rtl8139_device->rx_cur = (rtl8139_device->rx_cur + pckt_sz + 4 + 3) & (~3);
    rtl8139_device->rx_cur %= RX_BUFFER_SIZE - 0x10;

    // 0x38 == CAPR(CURRENT ADDRESS PACKET READ) PORT
    port_word_out (rtl8139_device->io_base + 0x38, rtl8139_device->rx_cur - 0x10);
    
  }

}

static void rtl8139_handler (registers_t *regs)
{
  uint16_t status = port_word_in ((uint16_t)(rtl8139_device->io_base + 0x3e));
  if(status & (1<<2))
  {
    port_word_out (rtl8139_device->io_base + 0x3E, 0x4);
  }
  if (status & (1<<0))
  {
    rtl8139_receive_frame ();
    port_word_out ((uint16_t)(rtl8139_device->io_base + 0x3E), 0x1);
  }
}

void rtl8139_send_frame (uint8_t* frame, uint32_t size)
{
  port_dword_out (rtl8139_device->io_base + TSAD_array[rtl8139_device->tx_cur], frame);
  port_dword_out (rtl8139_device->io_base + TSD_array[rtl8139_device->tx_cur++], size);

  if(rtl8139_device->tx_cur > 3){
    rtl8139_device->tx_cur = 0;
  }
}

void rtl8139_init ()
{
  rtl8139_device = (struct rtl8139_dev*) kmalloc_u (sizeof (struct rtl8139_dev));
  rtl8139_device->tx_cur = 0;
  rtl8139_device->rx_cur = 0;

  // find device
  pci_get_device ((uint16_t)VENDOR_ID, (uint16_t)DEVICE_ID, &RTL8139_BUS, &RTL8139_DEVICE);

  // enabling PCI bus mastering (allow nic to perform DMA)
  uint32_t reg = pci_read_data (RTL8139_BUS, RTL8139_DEVICE, 0x04); 
  reg |= 0x04;
  pci_write_data (RTL8139_BUS, RTL8139_DEVICE, 0x04, reg);

  // get io base address
  reg = pci_read_data (RTL8139_BUS, RTL8139_DEVICE, 0x10); 
  rtl8139_device->io_base = reg & (~0x03);  

  rtl8139_device->rx_buffer = (uint8_t*) virtual2phys (kernel_directory, kmalloc (RX_BUFFER_SIZE));

  rtl8139_reset();

  // register and enable network interruptions
  reg = pci_read_data (RTL8139_BUS, RTL8139_DEVICE, 0x3c) & 0xff;
  register_interrupt_handler (reg + 32, rtl8139_handler);

  // get mac addr
  uint8_t i;
  for (i = 0; i < 6; i++)
    rtl8139_device->mac_addr[i] = port_byte_in (rtl8139_device->io_base + i);

  // set ip addr
  set_ip_addr (0x01020304);
}

static void rtl8139_reset ()
{
  rtl8139_device->tx_cur = 0;
  rtl8139_device->rx_cur = 0;

  // turning on the rtl8139
  port_byte_out ((uint16_t)(rtl8139_device->io_base + 0x52), 0x0); 

  // software reset
  port_byte_out ((uint16_t)(rtl8139_device->io_base + 0x37), 0x10);
  while (port_byte_in ((uint16_t)(rtl8139_device->io_base + 0x37)) & 0x10);

  memset (rtl8139_device->rx_buffer, 0xff, RX_BUFFER_SIZE);
  port_dword_out ((uint16_t)(rtl8139_device->io_base + 0x30), (uint32_t)rtl8139_device->rx_buffer);

  // Sets the TOK and ROK bits high
  port_word_out ((uint16_t)(rtl8139_device->io_base + 0x3C), 0x0005);

  // (0 << 7) is the WRAP bit, 0xf is AB+AM+APM+AAP
  port_dword_out ((uint16_t)(rtl8139_device->io_base + 0x44), 0xf | (0 << 7));

  // Sets the RE and TE bits high
  port_byte_out ((uint16_t)(rtl8139_device->io_base + 0x37), 0x0C);
}
