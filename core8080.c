#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "core8080.h"

// cpu instruction abstractions
void add(struct State8080 *state, uint8_t value);

void adc(struct State8080 *state, uint8_t value);

void sub(struct State8080 *state, uint8_t value);

void sbb(struct State8080 *state, uint8_t value);

void call(struct State8080 *state, uint16_t addr);

void ret(struct State8080 *state);

void jump(struct State8080 *state, uint16_t addr);

void mem_write(struct State8080 *state, uint16_t offset, uint8_t value);

uint8_t mem_read(struct State8080 *state, uint16_t offset);

// util functions 
uint16_t make_word(uint8_t hb, uint8_t lb);

uint8_t get_low_byte(uint16_t word);

uint8_t get_high_byte(uint16_t word);

void print_state(struct State8080 *state);

void update_flags(struct State8080 *state, uint16_t value);

int parity(int x, int size);

int execute(struct State8080 *state) {
    unsigned char *opcode = &state->memory[state->pc];
    uint16_t offset, w;
    uint8_t value, b1, b2;

    int addr;
    switch (*opcode) {
        case 0x00: //NOP
            break;
				case 0x01: // LXI B, D16
						b1 = mem_read(state, opcode[1]);
						b2 = mem_read(state, opcode[2]);
						state->b = b2;
						state->c = b1;
						state->pc += 2;
						break;
				case 0x02: // STAX B
						offset = make_word(state->b, state->c);
						mem_write(state, offset, state->a);
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
				case 0x08:
						return 1;
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
						state->h = b1;
						state->pc += 1;
						break;
				case 0x3e: // MVI, A, D8
						b1 = opcode[1];
						state->a = b1;
						state->pc += 1;
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
            state->b = mem_read(state, offset); 
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
            state->c = mem_read(state, offset); 
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
            state->d = mem_read(state, offset); 
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
            state->e = mem_read(state, offset); 
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
            state->h = mem_read(state, offset); 
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
            state->l = mem_read(state, offset); 
            break;
        case 0x6F: // MOV L, A
            state->l = state->a;
            break;

        case 0x76: // HLT
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
            state->a = mem_read(state, offset); 
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
            value = mem_read(state, offset); 
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
            value = mem_read(state, offset); 
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
            value = mem_read(state, offset); 
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
            value = mem_read(state, offset); 
            sbb(state, value);
            break;
        case 0x9f: // SBB A
            sbb(state, state->a);
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
        case 0xc8: // rz
            if (state->flags.z) {
							ret(state);
							return 0;
						} break;
        case 0xc9: // ret
            ret(state);
						return 0;
        case 0xca: // jz adr
            if (state->flags.z) {
							jump(state, make_word(opcode[2], opcode[1]));
							return 0;
						} else state->pc += 2;
            break;
        case 0xd2: // jnc adr
            if (state->flags.cy) {
							jump(state, make_word(opcode[2], opcode[1]));
							return 0;
						}
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
        case 0xe2: // jpo
            if (state->flags.p) {
							jump(state, make_word(opcode[2], opcode[1]));
							return 0;
						} else state->pc += 2;
            break;
        default:
            printf("Panic! Unknown Instruction %x", opcode[0]);
            exit(1);
    }
    state->pc += 1;
		return 0;
}

void add(struct State8080 *state, uint8_t value) {
  uint16_t sum = (uint16_t) state->a + (uint16_t) value;
	update_flags(state, sum);
  state->a = sum & 0xff;
}

void adc(struct State8080 *state, uint8_t value) {
  uint16_t sum = (uint16_t) state->a + (uint16_t) value + (uint16_t) state->flags.cy;
	update_flags(state, sum);
  state->a = sum & 0xff;
}

void sub(struct State8080 *state, uint8_t value) {
  uint16_t diff = (uint16_t) state->a - (uint16_t) value;
	update_flags(state, diff);
  state->a = diff & 0xff;
}

void sbb(struct State8080 *state, uint8_t value) {
  uint16_t diff = (uint16_t) state->a - (uint16_t) value - (uint16_t) state->flags.cy;
	update_flags(state, diff);	
  state->a = diff & 0xff;
}

void update_flags(struct State8080 *state, uint16_t value) {
	state->flags.z = ((value & 0xff) == 0);
	state->flags.s = ((value & 0x80) != 0);
	state->flags.cy = (value > 0xff);
	state->flags.p = parity(value & 0xff, 8);
}

void call(struct State8080 *state, uint16_t addr) {
    uint16_t offset = state->pc + 2;
    state->memory[state->sp - 1] = (offset >> 8) & 0xff;
    state->memory[state->sp - 2] = offset & 0xff;
    state->sp = state->sp - 2;
    state->pc = addr;
}

void ret(struct State8080 *state) {
    uint16_t offset = state->pc;
    state->pc = make_word(state->memory[offset], state->memory[offset + 1]);
    state->sp += 2;
}

void jump(struct State8080 *state, uint16_t addr) {
    state->pc = addr;
		printf("jumping to %x\n", addr);
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

void mem_write(struct State8080 *state, uint16_t offset, uint8_t value) {
	state->memory[offset] = value;
}

uint8_t mem_read(struct State8080 *state, uint16_t offset) {
	return state->memory[offset];
}

uint8_t get_low_byte(uint16_t word) {
	return (uint8_t) word;
}

uint8_t get_high_byte(uint16_t word) {
	return (uint8_t) word >> 8;
}

void print_state(struct State8080 *state) {
	printf("A: %x, B: %x, C: %x, D: %x, E: %x, H: %x, L: %x \n", state->a, state->b, state->c, state->d, state->e, state->h, state->l);
	printf("Z: %x, S: %x, CY: %x, AC: %x, P: %x \n\n", state->flags.z, state->flags.s, state->flags.cy, state->flags.ac, state->flags.p);
}

int load_bin_file(struct State8080 *state, int offset, char *file_name) {
	FILE *fd = fopen(file_name, "rb");

	fseek(fd, 0L, SEEK_END);
	int fsize = ftell(fd);
	fseek(fd, 0L, SEEK_SET);
	printf("fsize %d\n", fsize);
	uint8_t *mem = malloc(fsize);
	fread(mem, fsize, 1, fd);
	fclose(fd);
	for (int i = 0; i < fsize; i++) {
		mem_write(state, offset+i, mem[i]);	
	}
}

struct State8080 *make_state(int mem_size) {
	struct State8080 *state = malloc(sizeof(struct State8080));
	state->memory = calloc(mem_size, sizeof(uint8_t));
	return state;
}
