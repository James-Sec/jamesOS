#include <bitmap.h>

void bitmap_insert(uint8_t *bitmap, uint32_t position) {
  uint32_t byte = position / 8;
  uint32_t bit = position % 8;
  bitmap[byte] |= 1 << bit;
}

void bitmap_clear(uint8_t *bitmap, uint32_t position) {
  uint32_t byte = position / 8;
  uint32_t bit = position % 8;
  bitmap[byte] &= ~(1 << bit);
}

uint8_t bitmap_read(uint8_t *bitmap, uint32_t position) {
  uint32_t byte = position / 8;
  uint32_t bit = position % 8;
  return (bitmap[byte] >> bit) & 1;
}

void bitmap_fill(uint8_t *bitmap, uint32_t bitmap_size, uint32_t begin_bit, uint32_t bits_size, uint32_t bit) {
  for (int32_t i = 0; i < bits_size; i++)
  {
    uint32_t position = (begin_bit + i) % bitmap_size;
    if (bit)
      bitmap_insert(bitmap, position);
    else
      bitmap_clear(bitmap, position);
  }
}

uint32_t bitmap_find_sequence(uint8_t *bitmap, uint32_t bitmap_size, uint32_t begin_bit, uint32_t expected_sequence_bits_size, uint32_t expected_bit) {
  uint32_t current_sequence_size = 0;
  uint32_t return_position = -1;
  for (int i = 0; i < bitmap_size; i++)
  {
    uint32_t position = (begin_bit + i) % bitmap_size;
    uint8_t value = bitmap_read(bitmap, position);
    if (value == expected_bit)
    {
      current_sequence_size++;
      if (current_sequence_size == 1)
        return_position = position;
      if (current_sequence_size == expected_sequence_bits_size)
        return return_position;
    }
    else
      current_sequence_size = 0;
  }
  return -1;
}
