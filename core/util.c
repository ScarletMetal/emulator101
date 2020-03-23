#include <stdint.h>

#include "util.h"

uint16_t make_word(uint8_t hbyte, uint8_t lbyte) {
    uint16_t word = (hbyte << 8) | lbyte;
    return word;
}

uint8_t get_low_byte(uint16_t word) {
    return (uint8_t) word;
}

uint8_t get_high_byte(uint16_t word) {
    return word >> 8;
}

int parity(int x, int size) {
    int p = 0;
    x = (x & ((1 << size) - 1));
    for (int i = 0; i < size; i++) {
        if (x & 0x1) p++;
        x = x >> 1;
    }
    return ((p & 0x1) == 0);
}