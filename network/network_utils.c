#include <network_utils.h>

void htonb (uint8_t* byte, uint8_t bits)
{
	uint8_t tmp = 0;

	uint8_t i = 0;
	for (; i < bits; i++)
		tmp += (((*byte) >> (7 - i)) & 1) << ((bits - 1) - i);

	i = 0;
	for (; i < (8 - bits); i++)
		tmp += (((*byte) >> i) & 1) << ((bits) + i);

	*byte = tmp;
}

void ntohb (uint8_t* byte, uint8_t bits)
{
	htonb (byte, 8 - bits);
}

void htons (uint16_t* shrt)
{
	uint16_t tmp = 0;

	tmp += (0xff & *shrt) << 8;
	tmp += (0xff00 & *shrt) >> 8;

	*shrt = tmp;
}

void ntohs (uint16_t* shrt)
{
  htons (shrt);
}

void htonl (uint32_t* lng)
{
	uint32_t tmp = 0;

	tmp += (0xff000000 & *lng) >> 24;
	tmp += (0x00ff0000 & *lng) >> 8;
	tmp += (0x0000ff00 & *lng) << 8;
	tmp += (0x000000ff & *lng) << 24;

	kprintf ("tmp: %x\n", 1, tmp);

	*lng = tmp;
}

void ntohl (uint32_t* lng)
{
  htonl (lng);
}

void set_bits_attr_value (uint8_t* attr, uint32_t offset, uint32_t size, uint32_t value)
{
	uint32_t i = offset;
	uint8_t cnt = size - 1;
	for (; i < (offset + size); i++) 
	{
    uint8_t byte = i / 8;
		uint8_t bit = 7 - (i % 8);

		if ((value >> cnt--) & 1)
			*(attr + byte) |= (1 << bit);
		else
			*(attr + byte) &= ~(1 << bit);
	}
}

uint32_t get_bits_attr_value (uint8_t* attr, uint32_t offset, uint32_t size)
{
  uint32_t ret = 0;

	uint32_t i = offset;
	uint8_t cnt = size - 1;
	for (; i < (offset + size); i++)
	{
		uint8_t byte = i / 8;
		uint8_t bit = 7 - (i % 8);

    if ((*(attr + byte) >> bit) & 1)
      ret |= (1 << cnt--);
    else
      cnt--;
	}
  return ret;
}

void get_bytes_attr_value (uint8_t* attr, uint32_t offset, uint32_t size, uint8_t* ret)
{
  memcpy (attr + offset, ret, size);
}

void set_bytes_attr_value (uint8_t* attr, uint32_t offset, uint32_t size, uint8_t* value)
{
  memcpy (value, attr + offset, size);
}

uint16_t internet_checksum (uint8_t* header, uint32_t header_size, uint8_t *data, uint32_t data_size)
{
  uint32_t i = 0;
  uint16_t *header_ptr = (uint16_t*) header;
  uint32_t sum = 0;
  uint16_t word = 0;
  for (i = 0 ; i < (header_size / 2); i++)
  {
    word = *(header_ptr + i);
    ntohs (&word);
    sum += word;
  }

  uint16_t *data_ptr = (uint16_t*) data;
  for (i = 0 ; i < (data_size / 2); i++)
  {
    word = *(data_ptr + i);
    ntohs (&word);
    sum += word;
  }
  if (data_size % 2)
  {
    word = data_ptr[data_size / 2] & 0xff;
    ntohs (&word);
    sum += word;
  }
  uint16_t carry_out = 0;
  do
  {
    carry_out = (sum >> 16) & 0xffff;
    sum &= 0xffff;
    sum += carry_out;
  }
  while (carry_out);

  return ~sum;
}
