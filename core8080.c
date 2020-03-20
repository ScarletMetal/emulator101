#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "core8080.h"
#include "disassembler.h"

// cpu instruction abstractions
void add(struct state_8080 *state, uint8_t value);

void adc(struct state_8080 *state, uint8_t value);

void sub(struct state_8080 *state, uint8_t value);

void sbb(struct state_8080 *state, uint8_t value);

void call(struct state_8080 *state, uint16_t addr);

void ret(struct state_8080 *state);

void push(struct state_8080 *state, uint8_t hb, uint8_t lb);

uint16_t pop(struct state_8080 *state);

void jump(struct state_8080 *state, uint16_t addr);

void cmp(struct state_8080 *state, uint8_t value);

void and(struct state_8080 *state, uint8_t value);

void or(struct state_8080 *state, uint8_t value);

void xor(struct state_8080 *state, uint8_t value);

void write_byte(struct state_8080 *state, uint16_t offset, uint8_t value);

void write_word(struct state_8080 *state, uint16_t offset, uint16_t value);

uint8_t read_byte(struct state_8080 *state, uint16_t offset);

uint16_t read_word(struct state_8080 *state, uint16_t offset);

void io_read(struct state_8080 *state, int port);
void io_write(struct state_8080 *state, int port);

// util functions 
uint16_t make_word(uint8_t hb, uint8_t lb);

uint8_t get_low_byte(uint16_t word);

uint8_t get_high_byte(uint16_t word);

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

    disassemble_8080(state->memory, state->pc);
    switch (*opcode) {
        case 0x00: //NOP
            break;
        case 0x01: // LXI B, D16
            b1 = read_byte(state, opcode[1]);
            b2 = read_byte(state, opcode[2]);
            state->b = b2;
            state->c = b1;
            state->pc += 2;
            break;
        case 0x02: // STAX B
            offset = make_word(state->b, state->c);
            write_byte(state, offset, state->a);
            break;
        case 0x03: // INX B
            w = make_word(state->b, state->c);
            w += 1;
            state->b = get_high_byte(w);
            state->c = get_low_byte(w);
            break;
        case 0x06: // MVI B, D8
            b1 = opcode[1];
            state->b = b1;
            state->pc += 1;
            break;
        case 0x07: // RLC
            b1 = (state->a & 0x80) != 0;
            state->a = (state->a << 1) | b1;
            state->flags.cy = b1;
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
        case 0x16: // MVI D, D8
            b1 = opcode[1];
            state->d = b1;
            state->pc += 1;
            break;
        case 0x1e: // MVI E, D8
            b1 = opcode[1];
            state->e = b1;
            state->pc += 1;
            break;
        case 0x26: // MVI H, D8
            b1 = opcode[1];
            state->h = b1;
            state->pc += 1;
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
            write_byte(state, offset, state->a);
            break;
        case 0x36: // MVI M, D8
            offset = make_word(state->h, state->l);
            write_byte(state, offset, opcode[1]);
            break;
        case 0x39: // STC
            state->flags.cy = 1;
            break;
        case 0x3a: // LDA D8
            offset = make_word(opcode[1], opcode[2]);
            state->a = read_byte(state, offset);
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
            state->b = read_byte(state, offset);
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
            state->c = read_byte(state, offset);
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
            state->d = read_byte(state, offset);
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
            state->e = read_byte(state, offset);
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
            state->h = read_byte(state, offset);
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
            state->l = read_byte(state, offset);
            break;
        case 0x6F: // MOV L, A
            state->l = state->a;
            break;
        case 0x70: // MOV M, B
            offset = make_word(state->h, state->l);
            write_byte(state, offset, state->b);
            break;
        case 0x71: // MOV M, C
            offset = make_word(state->h, state->l);
            write_byte(state, offset, state->c);
            break;
        case 0x72: // MOV M, D
            offset = make_word(state->h, state->l);
            write_byte(state, offset, state->d);
            break;

        case 0x73: // MOV M, E
            offset = make_word(state->h, state->l);
            write_byte(state, offset, state->e);
            break;
        case 0x74: // MOV M, H
            offset = make_word(state->h, state->l);
            write_byte(state, offset, state->h);
            break;
        case 0x75: // MOV M, L
            offset = make_word(state->h, state->l);
            write_byte(state, offset, state->c);
            break;
        case 0x76: // HLT
            return 1;
            break;
        case 0x77: // MOV M, A
            offset = make_word(state->h, state->l);
            write_byte(state, offset, state->a);
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
            state->a = read_byte(state, offset);
            break;
        case 0x7F: // MOV A, A
            break;
        case 0x80: // ADD B
            add(state, state->b);
            break;
        case 0x81: // ADD C
            add(state, state->c);
            break;
        case 0x82: // ADD D
            add(state, state->d);
            break;
        case 0x83: // ADD E
            add(state, state->e);
            break;
        case 0x84: // ADD H
            add(state, state->h);
            break;
        case 0x85: // ADD L
            add(state, state->l);
            break;
        case 0x86: // ADD M == ADD [hl]
            offset = make_word(state->h, state->l);
            value = read_byte(state, offset);
            add(state, value);
            break;
        case 0x87: // ADD A
            add(state, state->a);
            break;
        case 0x88: // ADC B
            adc(state, state->b);
            break;
        case 0x89: // ADC C
            adc(state, state->c);
            break;
        case 0x8a: // ADC D
            adc(state, state->d);
            break;
        case 0x8b: // ADC E
            adc(state, state->e);
            break;
        case 0x8c: // ADC H
            adc(state, state->h);
            break;
        case 0x8d: // ADC L
            adc(state, state->l);
            break;
        case 0x8e: // ADC M == ADC [hl]
            offset = make_word(state->h, state->l);
            value = read_byte(state, offset);
            adc(state, value);
            break;
        case 0x8f: // ADC A
            adc(state, state->a);
            break;
        case 0x90: // SUB B
            sub(state, state->b);
            break;
        case 0x91: // SUB C
            sub(state, state->c);
            break;
        case 0x92: // SUB D
            sub(state, state->d);
            break;
        case 0x93: // SUB E
            sub(state, state->e);
            break;
        case 0x94: // SUB H
            sub(state, state->h);
            break;
        case 0x95: // SUB L
            sub(state, state->l);
            break;
        case 0x96: // SUB M == SUB [hl]
            offset = make_word(state->h, state->l);
            value = read_byte(state, offset);
            sub(state, value);
            break;
        case 0x97: // SUB A
            sub(state, state->a);
            break;
        case 0x98: // SBB B
            sbb(state, state->b);
            break;
        case 0x99: // SBB C
            sbb(state, state->c);
            break;
        case 0x9a: // SBB D
            sbb(state, state->d);
            break;
        case 0x9b: // SBB E
            sbb(state, state->e);
            break;
        case 0x9c: // SBB H
            sbb(state, state->h);
            break;
        case 0x9d: // SBB L
            sbb(state, state->l);
            break;
        case 0x9e: // SBB M == SBB [hl]
            offset = make_word(state->h, state->l);
            value = read_byte(state, offset);
            sbb(state, value);
            break;
        case 0x9f: // SBB A
            sbb(state, state->a);
            break;
        case 0xa0: // ANA B
            and(state, state->b);
            break;
        case 0xa1: // ANA C
            and(state, state->c);
            break;
        case 0xa2: // ANA D
            and(state, state->d);
            break;
        case 0xa3: // ANA E
            and(state, state->e);
            break;
        case 0xa4: // ANA H
            and(state, state->h);
            break;
        case 0xa5: // ANA L
            and(state, state->l);
            break;
        case 0xa6: // ANA M
            and(state, read_byte(state, make_word(state->h, state->l)));
            break;
        case 0xa7: // ANA A
            and(state, state->a);
            break;
        case 0xa8: // XRA B
            xor(state, state->b);
            break;
        case 0xa9: // XRA C
            xor(state, state->c);
            break;
        case 0xaa: // XRA D
            xor(state, state->d);
            break;
        case 0xab: // XRA E
            xor(state, state->e);
            break;
        case 0xac: // XRA H
            xor(state, state->h);
            break;
        case 0xad: // XRA L
            xor(state, state->l);
            break;
        case 0xae: // XRA M
            xor(state, read_byte(state, make_word(state->h, state->l)));
            break;
        case 0xaf: // XRA A
            xor(state, state->a);
            break;
        case 0xb0: // ORA B
            or(state, state->b);
            break;
        case 0xb1: // ORA C
            or(state, state->c);
            break;
        case 0xb2: // ORA D
            or(state, state->d);
            break;
        case 0xb3: // ORA E
            or(state, state->e);
            break;
        case 0xb4: // ORA H
            or(state, state->h);
            break;
        case 0xb5: // ORA L
            or(state, state->l);
            break;
        case 0xb6: // ORA M
            or(state, read_byte(state, make_word(state->h, state->l)));
            break;
        case 0xb7: // ORA A
            or(state, state->a);
            break;
        case 0xc1: // POP B
            w = pop(state);
            state->b = get_high_byte(w);
            state->c = get_low_byte(w);
            break;
        case 0xc2: // jnz adr
            if (!state->flags.z) {
                jump(state, make_word(opcode[2], opcode[1]));
                return 0;
            } else state->pc += 2;
            break;
        case 0xc3: // jmp adr
            jump(state, make_word(opcode[2], opcode[1]));
            return 0;
        case 0xc4: // cnz adr
            if (!state->flags.z) {
                call(state, make_word(opcode[2], opcode[1]));
                return 0;
            } else state->pc += 2;
            break;
        case 0xc5: // PUSH B
            push(state, state->b, state->c);
            break;

        case 0xc6: // ADI D8
            b1 = opcode[1];
            add(state, b1);
            break;
        case 0xc8: // rz
            if (state->flags.z) {
                ret(state);
                return 0;
            }
            break;
        case 0xc9: // ret
            ret(state);
            return 0;
        case 0xca: // jz adr
            if (state->flags.z) {
                jump(state, make_word(opcode[2], opcode[1]));
                return 0;
            } else state->pc += 2;
            break;
        case 0xd1: // POP D
            w = pop(state);
            state->d = get_high_byte(w);
            state->e = get_low_byte(w);
            break;
        case 0xd2: // jnc adr
            if (state->flags.cy) {
                jump(state, make_word(opcode[2], opcode[1]));
                return 0;
            }
            break;
        case 0xd5: // PUSH D
            push(state, state->d, state->e);
            break;
        case 0xda: // jc adr
            if (!state->flags.cy) {
                jump(state, make_word(opcode[2], opcode[1]));
                return 0;
            } else state->pc += 2;
            break;
        case 0xdc: //
            if (!state->flags.cy) {
                call(state, make_word(opcode[2], opcode[1]));
                return 0;
            }
            break;
        case 0xcd: // call adr
            call(state, make_word(opcode[2], opcode[1]));
            return 0;
        case 0xe1: // POP H
            w = pop(state);
            state->h = get_high_byte(w);
            state->l = get_low_byte(w);
            break;
        case 0xe2: // jpo
            if (state->flags.p) {
                jump(state, make_word(opcode[2], opcode[1]));
                return 0;
            } else state->pc += 2;
            break;
        case 0xe3: // XTHL
            w = pop(state);
            state->h = get_high_byte(w);
            state->l = get_low_byte(w);
            break;
        case 0xe5: // PUSH H
            push(state, state->h, state->l);
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
            w = pop(state);
            state->a = get_high_byte(w);
            unpack_flags(state, get_low_byte(w));
            break;
        case 0xf5: // PUSH PSW
            push(state, state->a, pack_flags(state));
            break;
        case 0xf9: // SPHL
            state->sp = make_word(state->h, state->l);
            break;
        case 0xfe: // CPI
            b1 = opcode[1];
            cmp(state, b1);
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

            state->io->screen_buffer[py][px][0] = r;
            state->io->screen_buffer[py][px][1] = g;
            state->io->screen_buffer[py][px][2] = b;
        }
    }

    state->update_screen(state);
	return 0;
}

void add(struct state_8080 *state, uint8_t value) {
  uint16_t sum = (uint16_t) state->a + (uint16_t) value;
	update_flags(state, sum);
  state->a = sum & 0xff;
}

void adc(struct state_8080 *state, uint8_t value) {
  uint16_t sum = (uint16_t) state->a + (uint16_t) value + (uint16_t) state->flags.cy;
	update_flags(state, sum);
  state->a = sum & 0xff;
}

void sub(struct state_8080 *state, uint8_t value) {
  uint16_t diff = (uint16_t) state->a - (uint16_t) value;
	update_flags(state, diff);
  state->a = diff & 0xff;
}

void sbb(struct state_8080 *state, uint8_t value) {
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

void call(struct state_8080 *state, uint16_t addr) {
  uint16_t offset = state->pc + 2;
  state->memory[state->sp - 1] = (offset >> 8) & 0xff;
  state->memory[state->sp - 2] = offset & 0xff;
  state->sp = state->sp - 2;
  state->pc = addr;
}

void ret(struct state_8080 *state) {
  uint16_t offset = state->sp;
  state->pc = make_word(state->memory[offset], state->memory[offset + 1]);
  state->sp += 2;
}

void jump(struct state_8080 *state, uint16_t addr) {
  state->pc = addr;
}

void cmp(struct state_8080 *state, uint8_t value) {
	uint16_t diff = state->a - value;
	update_flags(state, diff);
}

void and(struct state_8080 *state, uint8_t value) {
	uint16_t and = state->a & value;
	state->a = and;
	update_flags(state, and);
}

void or(struct state_8080 *state, uint8_t value) {
	uint16_t or = state->a | value;
	state->a = or;
	update_flags(state, or);
}

void xor(struct state_8080 *state, uint8_t value) {
	uint16_t xor = state->a ^ value;
	state->a = xor;
	update_flags(state, xor);
}

void push(struct state_8080 *state, uint8_t hb, uint8_t lb) {
	uint16_t offset = state->sp;
	uint16_t w = make_word(hb, lb);
	write_byte(state, offset-1, hb);
	write_byte(state, offset-2, lb);
	state->sp -= 2;
}

void pushWord(struct state_8080 *state, uint16_t word) {
	push(state, get_high_byte(word), get_low_byte(word));
}

uint16_t pop(struct state_8080 *state) {
	uint16_t offset = state->sp;
	uint8_t b1 = read_byte(state, offset+1);
	uint8_t b2 = read_byte(state, offset);
	state->sp += 2;
	return make_word(b1, b2);
}
uint16_t make_word(uint8_t hbyte, uint8_t lbyte) {
    uint16_t word = (hbyte << 8) | lbyte;
    return word;
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

void write_byte(struct state_8080 *state, uint16_t offset, uint8_t value) {
	if (state->ram_offset <= offset) state->memory[offset] = value;
	else printf("Cannot Write To Offset %x, Part Of ROM\n", offset); 
}

void write_word(struct state_8080 *state, uint16_t offset, uint16_t value) {
	write_byte(state, offset, get_high_byte(value));
	write_byte(state, offset+1, get_low_byte(value));
}

uint8_t read_byte(struct state_8080 *state, uint16_t offset) {
	if (state->ram_offset <= offset) return state->memory[offset];
	return 0;
}

uint16_t read_word(struct state_8080 *state, uint16_t offset) {
	return make_word(read_byte(state, offset+1), read_byte(state,offset));
}
uint8_t get_low_byte(uint16_t word) {
	return (uint8_t) word;
}

uint8_t get_high_byte(uint16_t word) {
	return word >> 8; 
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

void io_read(struct state_8080 *state, int port) {
	uint8_t byte = state->io->rp[port];
	state->a = byte;
}

void io_write(struct state_8080 *state, int port) {
	uint8_t byte = state->a;
	state->io->wp[port] = byte;
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
		write_byte(state, offset+i, mem[i]);	
	}

	return 0;
}

struct state_8080 *make_state(int mem_size, uint16_t ram_offset) {
	struct state_8080 *state = malloc(sizeof(struct state_8080));
	state->memory = calloc(mem_size, sizeof(uint8_t));
	state->ram_offset = ram_offset;
	return state;
}
