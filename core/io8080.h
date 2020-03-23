#include <stdlib.h>
#include <stdint.h>


#include "constants.h"

struct io_8080 {
    uint8_t *ports;

    void (* notify_read) (int port);
    void (* notify_write) (int port);
};

void io8080_write_port(struct io_8080 *io, int port, uint8_t value);

uint8_t io8080_read_port(struct io_8080 *io, int port);

struct io_8080 *make_io(size_t size);