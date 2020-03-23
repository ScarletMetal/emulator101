#include <stdint.h>

uint16_t make_word(uint8_t hb, uint8_t lb);

uint8_t get_low_byte(uint16_t word);

uint8_t get_high_byte(uint16_t word);

int parity(int x, int size);