#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "core8080.h"
#include "disassembler.h"

// cpu instruction abstractions
void core8080_add(struct state_8080 *state, uint8_t value);

void core808_adc(struct state_8080 *state, uint8_t value);

void core8080_sub(struct state_8080 *state, uint8_t value);

void core8080_sbb(struct state_8080 *state, uint8_t value);

void core8080_call(struct state_8080 *state, uint16_t addr);

void core8080_ret(struct state_8080 *state);

void core8080_push(struct state_8080 *state, uint8_t hb, uint8_t lb);

uint16_t core8080_pop(struct state_8080 *state);

void core8080_jump(struct state_8080 *state, uint16_t addr);

void core8080_cmp(struct state_8080 *state, uint8_t value);

void core8080_and(struct state_8080 *state, uint8_t value);

void core8080_or(struct state_8080 *state, uint8_t value);

void core8080_xor(struct state_8080 *state, uint8_t value);

void core8080_write_byte(struct state_8080 *state, uint16_t offset, uint8_t value);

uint8_t core8080_read_byte(struct state_8080 *state, uint16_t offset);

void core8080_io_read(struct state_8080 *state, int port);
void core8080_io_write(struct state_8080 *state, int port);

// util functions

uint8_t pack_flags(struct state_8080 *state);

void unpack_flags(struct state_8080 *state, uint8_t psw);
void print_state(struct state_8080 *state);

void update_flags(struct state_8080 *state, uint16_t value);

int parity(int x, int size);

int cpu_update(struct state_8080 *state) {
    unsigned char *opcode = &state->memory[state->pc];
    uint16_t offset, w;
    uint8_t value, b1, b2;
    int addr;

    switch (*opcode) {
        case 0x00: //NOP
            break;
        case 0x01: // LXI B, D16
            b1 = core8080_read_byte(state, opcode[1]);
            b2 = core8080_read_byte(state, opcode[2]);
            state->b = b2;
            state->c = b1;
            state->pc += 2;
            break;
        case 0x02: // STAX B
            offset = make_word(state->b, state->c);
            core8080_write_byte(state, offset, state->a);
            break;
        case 0x03: // INX B
            w = make_word(state->b, state->c);
            w += 1;
            state->b = get_high_byte(w);
            state->c = get_low_byte(w);
            update_flags(state, w);
            break;
        case 0x04: // INC B
            state->b += 1;
            update_flags(state, state->b);
            break;
        case 0x05: // DCR B
            state->b -= 1;
            update_flags(state, state->b);
            break;
        case 0x06: // MVI B, D8
            b1 = opcode[1];
            state->b = b1;
            state->pc += 1;
            break;
        case 0x07: // RLC
            b1 = (state->a & 0x80) != 0;
            state->a = state->a << 1 | b1;
            state->flags.cy = b1;
            break;
        case 0x0b: // DCX B
            w = make_word(state->b, state->c);
            w -= 1;
            state->b = get_high_byte(w);
            state->c = get_low_byte(w);
            update_flags(state, w);
            break;
        case 0x0c: // INR C
            state->c += 1;
            update_flags(state, state->c);
            break;
        case 0x0d: // DCR C
            state->c -= 1;
            update_flags(state, state->c);
            break;
        case 0x0f: // RRC
            b1 = (state->a & 0x1);
            state->a = (state->a >> 1) | (0b1 << b1);
            state->flags.cy = b1;
            break;
        case 0x0e: // MVI C, D8
            b1 = opcode[1];
            state->c = b1;
            state->pc += 1;
            break;
        case 0x13: // INX D
            w = make_word(state->d, state->e);
            w += 1;
            state->d = get_high_byte(w);
            state->e = get_low_byte(w);
            update_flags(state, w);
            break;
        case 0x14: // INR D
            state->d += 1;
            update_flags(state, state->d);
            break;
        case 0x15: // DCR D
            state->d -= 1;
            update_flags(state, state->d);
            break;
        case 0x16: // MVI D, D8
            b1 = opcode[1];
            state->d = b1;
            state->pc += 1;
            break;
        case 0x1b: // DCX D
            w = make_word(state->d, state->e);
            w -= 1;
            state->d = get_high_byte(w);
            state->e = get_low_byte(w);
            update_flags(state, w);
            break;
        case 0x1c: // INR E
            state->e += 1;
            update_flags(state, state->e);
            break;
        case 0x1d: // DCR E
            state->e -= 1;
            update_flags(state, state->e);
            break;
        case 0x1e: // MVI E, D8
            b1 = opcode[1];
            state->e = b1;
            state->pc += 1;
            break;
        case 0x23: // INX H
            w = make_word(state->h, state->l);
            w += 1;
            state->h = get_high_byte(w);
            state->l = get_low_byte(w);
            update_flags(state, w);
            break;
        case 0x24: // INR H
            state->h += 1;
            update_flags(state, state->h);
            break;
        case 0x25: // DCR H
            state->h -= 1;
            update_flags(state, state->h);
            break;
        case 0x26: // MVI H, D8
            b1 = opcode[1];
            state->h = b1;
            state->pc += 1;
            break;
        case 0x2b: // DCX H
            w = make_word(state->h, state->l);
            w -= 1;
            state->h = get_high_byte(w);
            state->l = get_low_byte(w);
            update_flags(state, w);
            break;
        case 0x2c: // INR L
            state->l += 1;
            update_flags(state, state->l);
            break;
        case 0x2d: // DCR L
            state->l -= 1;
            update_flags(state, state->l);
            break;

        case 0x2e: // MVI L, D8
            b1 = opcode[1];
            state->l = b1;
            state->pc += 1;
            break;
        case 0x2f: // CMA
            state->a = ~state->a;
            break;
        case 0x32: // STA adr
            offset = make_word(opcode[1], opcode[2]);
            core8080_write_byte(state, offset, state->a);
            break;
        case 0x33: // INX SP
            state->sp += 1;
            break;
        case 0x34: // INR M
            offset = make_word(state->h, state->l);
            b1 = core8080_read_byte(state, offset) + 1;
            core8080_write_byte(state, offset, b1);
            update_flags(state, b1);
            break;
        case 0x35: // DCR M
            offset = make_word(state->h, state->l);
            b1 = core8080_read_byte(state, offset) - 1;
            core8080_write_byte(state, offset, b1);
            update_flags(state, b1);
            break;
        case 0x3b: // DCX SP
            state->sp -= 1;
            break;
        case 0x3c: // INR A
            state->a += 1;
            update_flags(state, state->a);
            break;
        case 0x3d: // DCR A
            state->a -= 1;
            update_flags(state, state->a);
            break;
        case 0x36: // MVI M, D8
            offset = make_word(state->h, state->l);
            core8080_write_byte(state, offset, opcode[1]);
            break;
        case 0x39: // STC
            state->flags.cy = 1;
            break;
        case 0x3a: // LDA D8
            offset = make_word(opcode[1], opcode[2]);
            state->a = core8080_read_byte(state, offset);
            break;
        case 0x3e: // MVI, A, D8
            b1 = opcode[1];
            state->a = b1;
            state->pc += 1;
            break;
        case 0x3f: // CMC
            state->flags.cy = ~state->flags.cy;
            break;
        case 0x40: // MOV B, B
            break;
        case 0x41: // MOV B, C
            state->b = state->c;
            break;
        case 0x42: // MOV B, D
            state->b = state->d;
            break;
        case 0x43: // MOV B, E
            state->b = state->e;
            break;
        case 0x44: // MOV B, H
            state->b = state->h;
            break;
        case 0x45: // MOV B, L
            state->b = state->l;
            break;
        case 0x46: // MOV B, M == MOV B, [hl]
            offset = make_word(state->h, state->l);
            state->b = core8080_read_byte(state, offset);
            break;
        case 0x47: // MOV B, A
            state->b = state->a;
            break;
        case 0x48: // MOV C, B
            state->c = state->b;
            break;
        case 0x49: // MOV C, C
            break;
        case 0x4A: // MOV C, D
            state->c = state->d;
            break;
        case 0x4B: // MOV C, E
            state->c = state->e;
            break;
        case 0x4C: // MOV C, H
            state->c = state->h;
            break;
        case 0x4D: // MOV C, L
            state->c = state->l;
            break;
        case 0x4E: // MOV C, M == MOV C, [hl]
            offset = make_word(state->h, state->l);
            state->c = core8080_read_byte(state, offset);
            break;
        case 0x4f: // MOV C, A
            state->c = state->a;
            break;
        case 0x50: // MOV D, B
            state->d = state->b;
            break;
        case 0x51: // MOV D, C
            state->d = state->c;
            break;
        case 0x52: // MOV D, d
            break;
        case 0x53: // MOV D, E
            state->d = state->e;
            break;
        case 0x54: // MOV D, H
            state->d = state->h;
            break;
        case 0x55: // MOV D, L
            state->d = state->l;
            break;
        case 0x56: // MOV D, M == MOV D, [hl]
            offset = make_word(state->h, state->l);
            state->d = core8080_read_byte(state, offset);
            break;
        case 0x57: // MOV D, A
            state->d = state->a;
            break;
        case 0x58: // MOV e, B
            state->e = state->b;
            break;
        case 0x59: // MOV e, C
            state->e = state->c;
            break;
        case 0x5A: // MOV E, D
            state->e = state->d;
            break;
        case 0x5B: // MOV E, E
            break;
        case 0x5C: // MOV E, H
            state->e = state->h;
            break;
        case 0x5D: // MOV E, L
            state->e = state->l;
            break;
        case 0x5E: // MOV E, M == MOV E, [hl]
            offset = make_word(state->h, state->l);
            state->e = core8080_read_byte(state, offset);
            break;
        case 0x5F: // MOV E, A
            state->e = state->a;
            break;
        case 0x60: // MOV H, B
            state->h = state->b;
            break;
        case 0x61: // MOV H, C
            state->h = state->c;
            break;
        case 0x62: // MOV H, D
            state->h = state->d;
            break;
        case 0x63: // MOV H, E
            state->h = state->e;
            break;
        case 0x64: // MOV H, H
            break;
        case 0x65: // MOV H, L
            state->h = state->l;
            break;
        case 0x66: // MOV H, M == MOV H, [hl]
            offset = make_word(state->h, state->l);
            state->h = core8080_read_byte(state, offset);
            break;
        case 0x67: // MOV h, A
            state->h = state->a;
            break;
        case 0x68: // MOV L, B
            state->l = state->b;
            break;
        case 0x69: // MOV L, C
            state->l = state->c;
            break;
        case 0x6A: // MOV L, D
            state->l = state->d;
            break;
        case 0x6B: // MOV H, E
            state->l = state->e;
            break;
        case 0x6C: // MOV L, H
            state->l = state->h;
            break;
        case 0x6D: // MOV L, L
            break;
        case 0x6E: // MOV L, M == MOV L, [hl]
            offset = make_word(state->h, state->l);
            state->l = core8080_read_byte(state, offset);
            break;
        case 0x6F: // MOV L, A
            state->l = state->a;
            break;
        case 0x70: // MOV M, B
            offset = make_word(state->h, state->l);
            core8080_write_byte(state, offset, state->b);
            break;
        case 0x71: // MOV M, C
            offset = make_word(state->h, state->l);
            core8080_write_byte(state, offset, state->c);
            break;
        case 0x72: // MOV M, D
            offset = make_word(state->h, state->l);
            core8080_write_byte(state, offset, state->d);
            break;

        case 0x73: // MOV M, E
            offset = make_word(state->h, state->l);
            core8080_write_byte(state, offset, state->e);
            break;
        case 0x74: // MOV M, H
            offset = make_word(state->h, state->l);
            core8080_write_byte(state, offset, state->h);
            break;
        case 0x75: // MOV M, L
            offset = make_word(state->h, state->l);
            core8080_write_byte(state, offset, state->c);
            break;
        case 0x76: // HLT
            return 1;
            break;
        case 0x77: // MOV M, A
            offset = make_word(state->h, state->l);
            core8080_write_byte(state, offset, state->a);
            break;
        case 0x78: // MOV A, B
            state->a = state->b;
            break;
        case 0x79: // MOV A, C
            state->a = state->c;
            break;
        case 0x7A: // MOV A, D
            state->a = state->d;
            break;
        case 0x7B: // MOV H, E
            state->a = state->e;
            break;
        case 0x7C: // MOV A, H
            state->a = state->h;
            break;
        case 0x7D: // MOV A, A
            break;
        case 0x7E: // MOV A, M == MOV A, [hl]
            offset = make_word(state->h, state->l);
            state->a = core8080_read_byte(state, offset);
            break;
        case 0x7F: // MOV A, A
            break;
        case 0x80: // ADD B
            core8080_add(state, state->b);
            break;
        case 0x81: // ADD C
            core8080_add(state, state->c);
            break;
        case 0x82: // ADD D
            core8080_add(state, state->d);
            break;
        case 0x83: // ADD E
            core8080_add(state, state->e);
            break;
        case 0x84: // ADD H
            core8080_add(state, state->h);
            break;
        case 0x85: // ADD L
            core8080_add(state, state->l);
            break;
        case 0x86: // ADD M == ADD [hl]
            offset = make_word(state->h, state->l);
            value = core8080_read_byte(state, offset);
            core8080_add(state, value);
            break;
        case 0x87: // ADD A
            core8080_add(state, state->a);
            break;
        case 0x88: // ADC B
            core808_adc(state, state->b);
            break;
        case 0x89: // ADC C
            core808_adc(state, state->c);
            break;
        case 0x8a: // ADC D
            core808_adc(state, state->d);
            break;
        case 0x8b: // ADC E
            core808_adc(state, state->e);
            break;
        case 0x8c: // ADC H
            core808_adc(state, state->h);
            break;
        case 0x8d: // ADC L
            core808_adc(state, state->l);
            break;
        case 0x8e: // ADC M == ADC [hl]
            offset = make_word(state->h, state->l);
            value = core8080_read_byte(state, offset);
            core808_adc(state, value);
            break;
        case 0x8f: // ADC A
            core808_adc(state, state->a);
            break;
        case 0x90: // SUB B
            core8080_sub(state, state->b);
            break;
        case 0x91: // SUB C
            core8080_sub(state, state->c);
            break;
        case 0x92: // SUB D
            core8080_sub(state, state->d);
            break;
        case 0x93: // SUB E
            core8080_sub(state, state->e);
            break;
        case 0x94: // SUB H
            core8080_sub(state, state->h);
            break;
        case 0x95: // SUB L
            core8080_sub(state, state->l);
            break;
        case 0x96: // SUB M == SUB [hl]
            offset = make_word(state->h, state->l);
            value = core8080_read_byte(state, offset);
            core8080_sub(state, value);
            break;
        case 0x97: // SUB A
            core8080_sub(state, state->a);
            break;
        case 0x98: // SBB B
            core8080_sbb(state, state->b);
            break;
        case 0x99: // SBB C
            core8080_sbb(state, state->c);
            break;
        case 0x9a: // SBB D
            core8080_sbb(state, state->d);
            break;
        case 0x9b: // SBB E
            core8080_sbb(state, state->e);
            break;
        case 0x9c: // SBB H
            core8080_sbb(state, state->h);
            break;
        case 0x9d: // SBB L
            core8080_sbb(state, state->l);
            break;
        case 0x9e: // SBB M == SBB [hl]
            offset = make_word(state->h, state->l);
            value = core8080_read_byte(state, offset);
            core8080_sbb(state, value);
            break;
        case 0x9f: // SBB A
            core8080_sbb(state, state->a);
            break;
        case 0xa0: // ANA B
            core8080_and(state, state->b);
            break;
        case 0xa1: // ANA C
            core8080_and(state, state->c);
            break;
        case 0xa2: // ANA D
            core8080_and(state, state->d);
            break;
        case 0xa3: // ANA E
            core8080_and(state, state->e);
            break;
        case 0xa4: // ANA H
            core8080_and(state, state->h);
            break;
        case 0xa5: // ANA L
            core8080_and(state, state->l);
            break;
        case 0xa6: // ANA M
            core8080_and(state, core8080_read_byte(state, make_word(state->h, state->l)));
            break;
        case 0xa7: // ANA A
            core8080_and(state, state->a);
            break;
        case 0xa8: // XRA B
            core8080_xor(state, state->b);
            break;
        case 0xa9: // XRA C
            core8080_xor(state, state->c);
            break;
        case 0xaa: // XRA D
            core8080_xor(state, state->d);
            break;
        case 0xab: // XRA E
            core8080_xor(state, state->e);
            break;
        case 0xac: // XRA H
            core8080_xor(state, state->h);
            break;
        case 0xad: // XRA L
            core8080_xor(state, state->l);
            break;
        case 0xae: // XRA M
            core8080_xor(state, core8080_read_byte(state, make_word(state->h, state->l)));
            break;
        case 0xaf: // XRA A
            core8080_xor(state, state->a);
            break;
        case 0xb0: // ORA B
            core8080_or(state, state->b);
            break;
        case 0xb1: // ORA C
            core8080_or(state, state->c);
            break;
        case 0xb2: // ORA D
            core8080_or(state, state->d);
            break;
        case 0xb3: // ORA E
            core8080_or(state, state->e);
            break;
        case 0xb4: // ORA H
            core8080_or(state, state->h);
            break;
        case 0xb5: // ORA L
            core8080_or(state, state->l);
            break;
        case 0xb6: // ORA M
            core8080_or(state, core8080_read_byte(state, make_word(state->h, state->l)));
            break;
        case 0xb7: // ORA A
            core8080_or(state, state->a);
            break;
        case 0xc1: // POP B
            w = core8080_pop(state);
            state->b = get_high_byte(w);
            state->c = get_low_byte(w);
            break;
        case 0xc2: // jnz adr
            if (!state->flags.z) {
                core8080_jump(state, make_word(opcode[2], opcode[1]));
                return 0;
            } else state->pc += 2;
            break;
        case 0xc3: // jmp adr
            core8080_jump(state, make_word(opcode[2], opcode[1]));
            return 0;
        case 0xc4: // cnz adr
            if (!state->flags.z) {
                core8080_call(state, make_word(opcode[2], opcode[1]));
                return 0;
            } else state->pc += 2;
            break;
        case 0xc5: // PUSH B
            core8080_push(state, state->b, state->c);
            break;

        case 0xc6: // ADI D8
            b1 = opcode[1];
            core8080_add(state, b1);
            break;
        case 0xc8: // rz
            if (state->flags.z) {
                core8080_ret(state);
                return 0;
            }
            break;
        case 0xc9: // ret
            core8080_ret(state);
            return 0;
        case 0xca: // jz adr
            if (state->flags.z) {
                core8080_jump(state, make_word(opcode[2], opcode[1]));
                return 0;
            } else state->pc += 2;
            break;
        case 0xd1: // POP D
            w = core8080_pop(state);
            state->d = get_high_byte(w);
            state->e = get_low_byte(w);
            break;
        case 0xd2: // jnc adr
            if (state->flags.cy) {
                core8080_jump(state, make_word(opcode[2], opcode[1]));
                return 0;
            }
            break;
        case 0xd3: // OUT D8
            b1 = opcode[1];
            core8080_io_write(state, b1);
            state->pc += 1;
            break;
        case 0xd5: // PUSH D
            core8080_push(state, state->d, state->e);
            break;
        case 0xda: // jc adr
            if (!state->flags.cy) {
                core8080_jump(state, make_word(opcode[2], opcode[1]));
                return 0;
            } else state->pc += 2;
            break;
        case 0xdb: // IN D8
            b1 = opcode[1];
            core8080_io_read(state, b1);
            state->pc += 1;
            break;
        case 0xdc: //
            if (!state->flags.cy) {
                core8080_call(state, make_word(opcode[2], opcode[1]));
                return 0;
            }
            break;
        case 0xcd: // call adr
            core8080_call(state, make_word(opcode[2], opcode[1]));
            return 0;
        case 0xe1: // POP H
            w = core8080_pop(state);
            state->h = get_high_byte(w);
            state->l = get_low_byte(w);
            break;
        case 0xe2: // jpo
            if (state->flags.p) {
                core8080_jump(state, make_word(opcode[2], opcode[1]));
                return 0;
            } else state->pc += 2;
            break;
        case 0xe3: // XTHL
            w = core8080_pop(state);
            state->h = get_high_byte(w);
            state->l = get_low_byte(w);
            break;
        case 0xe5: // PUSH H
            core8080_push(state, state->h, state->l);
            break;
        case 0xe6: // ANI D8
            b1 = opcode[1];
            state->a = state->a & b1;
            state->pc += 1;
            break;
        case 0xeb: // XCHG
            b1 = state->h;
            b2 = state->l;
            state->h = state->d;
            state->l = state->e;
            state->d = b1;
            state->e = b2;
            break;
        case 0xf1: // POP PSW
            w = core8080_pop(state);
            state->a = get_high_byte(w);
            unpack_flags(state, get_low_byte(w));
            break;
        case 0xf5: // PUSH PSW
            core8080_push(state, state->a, pack_flags(state));
            break;
        case 0xf9: // SPHL
            state->sp = make_word(state->h, state->l);
            break;
        case 0xfe: // CPI
            b1 = opcode[1];
            core8080_cmp(state, b1);
            state->pc += 1;
            break;
        default:
            printf("Panic! Unknown Instruction %x", opcode[0]);
            exit(1);
    }
    state->pc += 1;
    return 0;
}

int gpu_update(struct state_8080 *state) {

	    for (int i = 0; i < 256 * 224 / 8; i++) {
        const int y = i * 8 / 256;
        const int base_x = (i * 8) % 256;
        const uint8_t cur_byte = state->memory[VRAM_ADDRESS + i];

        for (uint8_t bit = 0; bit < 8; bit++) {
            int px = base_x + bit;
            int py = y;
            const uint8_t is_pixel_lit = (cur_byte >> bit) & 1;
            uint8_t r = 0, g = 0, b = 0;


            if (is_pixel_lit) {
                if (px < 16) {
                    if (py < 16 || py > 118 + 16) {
                        r = 255; g = 255; b = 255;
                    }
                    else {
                        g = 255;
                    }
                }
                else if (px >= 16 && px <= 16 + 56) {
                    g = 255;
                }
                else if (px >= 16 + 56 + 120 && px < 16 + 56 + 120 + 32) {
                    r = 255;
                }
                else {
                    r = 255; g = 255; b = 255;
                }
            }

            // space invaders' screen is rotated 90 degrees anti-clockwise
            // so we invert the coordinates:
            const int temp_x = px;
            px = py;
            py = -temp_x + SCREEN_HEIGHT - 1;

            state->screen_buffer[py][px][0] = r;
            state->screen_buffer[py][px][1] = g;
            state->screen_buffer[py][px][2] = b;
        }
    }

    state->update_screen(state);
	return 0;
}

void core8080_add(struct state_8080 *state, uint8_t value) {
  uint16_t sum = (uint16_t) state->a + (uint16_t) value;
	update_flags(state, sum);
  state->a = sum & 0xff;
}

void core808_adc(struct state_8080 *state, uint8_t value) {
  uint16_t sum = (uint16_t) state->a + (uint16_t) value + (uint16_t) state->flags.cy;
	update_flags(state, sum);
  state->a = sum & 0xff;
}

void core8080_sub(struct state_8080 *state, uint8_t value) {
  uint16_t diff = (uint16_t) state->a - (uint16_t) value;
	update_flags(state, diff);
  state->a = diff & 0xff;
}

void core8080_sbb(struct state_8080 *state, uint8_t value) {
  uint16_t diff = (uint16_t) state->a - (uint16_t) value - (uint16_t) state->flags.cy;
	update_flags(state, diff);	
  state->a = diff & 0xff;
}

void update_flags(struct state_8080 *state, uint16_t value) {
	state->flags.z = ((value & 0xff) == 0);
	state->flags.s = ((value & 0x80) != 0);
	state->flags.cy = (value > 0xff);
	state->flags.p = parity(value & 0xff, 8);
}

void core8080_call(struct state_8080 *state, uint16_t addr) {
  uint16_t offset = state->pc + 2;
  state->memory[state->sp - 1] = (offset >> 8) & 0xff;
  state->memory[state->sp - 2] = offset & 0xff;
  state->sp = state->sp - 2;
  state->pc = addr;
}

void core8080_ret(struct state_8080 *state) {
  uint16_t offset = state->sp;
  state->pc = make_word(state->memory[offset], state->memory[offset + 1]);
  state->sp += 2;
}

void core8080_jump(struct state_8080 *state, uint16_t addr) {
  state->pc = addr;
}

void core8080_cmp(struct state_8080 *state, uint8_t value) {
	uint16_t diff = state->a - value;
	update_flags(state, diff);
}

void core8080_and(struct state_8080 *state, uint8_t value) {
	uint16_t and = state->a & value;
	state->a = and;
	update_flags(state, and);
}

void core8080_or(struct state_8080 *state, uint8_t value) {
	uint16_t or = state->a | value;
	state->a = or;
	update_flags(state, or);
}

void core8080_xor(struct state_8080 *state, uint8_t value) {
	uint16_t xor = state->a ^ value;
	state->a = xor;
	update_flags(state, xor);
}

void core8080_push(struct state_8080 *state, uint8_t hb, uint8_t lb) {
	uint16_t offset = state->sp;
	uint16_t w = make_word(hb, lb);
    core8080_write_byte(state, offset - 1, hb);
    core8080_write_byte(state, offset - 2, lb);
	state->sp -= 2;
}

uint16_t core8080_pop(struct state_8080 *state) {
	uint16_t offset = state->sp;
	uint8_t b1 = core8080_read_byte(state, offset + 1);
	uint8_t b2 = core8080_read_byte(state, offset);
	state->sp += 2;
	return make_word(b1, b2);
}

void core8080_write_byte(struct state_8080 *state, uint16_t offset, uint8_t value) {
	if (state->ram_offset <= offset) state->memory[offset] = value;
	else printf("Cannot Write To Offset %x, Part Of ROM\n", offset); 
}

uint8_t core8080_read_byte(struct state_8080 *state, uint16_t offset) {
	if (state->ram_offset <= offset) return state->memory[offset];
	return 0;
}


void print_state(struct state_8080 *state) {
	printf("A: %x, B: %x, C: %x, D: %x, E: %x, H: %x, L: %x \n", state->a, state->b, state->c, state->d, state->e, state->h, state->l);
	printf("Z: %x, S: %x, CY: %x, AC: %x, P: %x \n\n", state->flags.z, state->flags.s, state->flags.cy, state->flags.ac, state->flags.p);
}

uint8_t pack_flags(struct state_8080 *state) {
	return (state->flags.z | state->flags.s << 1 | state->flags.p << 2 | state->flags.cy << 3 | state->flags.ac << 4);  
}

void unpack_flags(struct state_8080 *state, uint8_t psw) {
	state->flags.z = 0x01 == (psw & 0x01);
	state->flags.s = 0x02 == (psw & 0x02);
	state->flags.p = 0x04 == (psw & 0x04);
	state->flags.cy = 0x08 == (psw & 0x08);
	state->flags.ac = 0x10 == (psw & 0x10);
}

void core8080_io_read(struct state_8080 *state, int port) {
	uint8_t byte = state->io->ports[port];
	state->a = byte;
}

void core8080_io_write(struct state_8080 *state, int port) {
	uint8_t byte = state->a;
	state->io->ports[port] = byte;
}

int load_bin_file(struct state_8080 *state, int offset, char *file_name) {
	FILE *fd = fopen(file_name, "rb");

	fseek(fd, 0L, SEEK_END);
	int fsize = ftell(fd);
	fseek(fd, 0L, SEEK_SET);
	printf("fsize %d\n", fsize);
	uint8_t *mem = malloc(fsize);
	fread(mem, fsize, 1, fd);
	fclose(fd);
	for (int i = 0; i < fsize; i++) {
        core8080_write_byte(state, offset + i, mem[i]);
	}

	return 0;
}

struct state_8080 *make_state(int mem_size, uint16_t ram_offset) {
	struct state_8080 *state = malloc(sizeof(struct state_8080));
	state->memory = calloc(mem_size, sizeof(uint8_t));
	state->ram_offset = ram_offset;
	return state;
}
