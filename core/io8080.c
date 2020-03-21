//
// Created by Simon Kharmatsky on 3/21/20.
//

#include "io8080.h"

void set_port(struct io_8080 *io, int port, uint8_t value) {
    io->ports[port] = value;
}

uint8_t get_port(struct io_8080 *io, int port) {
    return io->ports[port];
}