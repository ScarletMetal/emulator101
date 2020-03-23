//
// Created by Simon Kharmatsky on 3/21/20.
//

#include "io8080.h"

void io8080_write_port(struct io_8080 *io, int port, uint8_t value) {
    io->ports[port] = value;
}

uint8_t io8080_read_port(struct io_8080 *io, int port) {
    return io->ports[port];
}

struct io_8080 *make_io(size_t size) {
    struct io_8080 *io = malloc(sizeof(struct io_8080));
    io->ports = calloc(size, sizeof(uint8_t));
    return io;
}