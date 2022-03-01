#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

void bitmap_insert(uint8_t *bitmap, uint32_t position);

uint8_t bitmap_read(uint8_t *bitmap, uint32_t position);

void bitmap_fill(uint8_t *bitmap, uint32_t bitmap_size, uint32_t begin_bit, uint32_t bits_size, uint32_t bit);

uint32_t bitmap_find_sequence(uint8_t *bitmap, uint32_t bitmap_size, uint32_t begin_bit, uint32_t expected_sequence_bits_size, uint32_t expected_bit);

#endif
