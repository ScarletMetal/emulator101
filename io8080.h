#include <stdint.h>

#include "constants.h"

struct io_8080 {
    uint8_t *ports;
    uint8_t shift0;
    uint8_t shift1;
};

void set_port(struct io_8080 *io, int port, uint8_t value);
uint8_t get_port(struct io_8080 *io, int port);