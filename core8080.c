#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Flags {
    uint8_t z:1;
    uint8_t s:1;
    uint8_t cy:1;
    uint8_t ac:1;
    uint8_t p:1;
    uint8_t pad:3;
} Flags;

typedef struct State8080 {
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;

    uint16_t sp;
    uint16_t pc;

    uint8_t *memory;
    struct Flags flags;

    uint8_t int_enable;
} State8080;

// cpu instruction abstractions
void add(State8080 *state, uint8_t value);

void adc(State8080 *state, uint8_t value);

void sub(State8080 *state, uint8_t value);

void sbb(State8080 *state, uint8_t value);

void call(State8080 *state, uint16_t addr);

void ret(State8080 *state);

void jump(State8080 *state, uint16_t addr);

void mem_write(State8080 *state, uint16_t offset, uint8_t value);

uint8_t mem_read(State8080 *state, uint16_t offset);

// util functions 
uint16_t make_word(uint8_t hb, uint8_t lb);

int parity(int x, int size);

void cycle(State8080 *state) {
    unsigned char *opcode = &state->memory[state->pc];
    uint16_t offset;
    uint8_t value;

    int addr;
    switch (*opcode) {
        case 0x00: //NOP
            break;
        case 0x40: // mov B, B
            break;
        case 0x41: // mov B, C
            state->b = state->c;
            break;
        case 0x42: // mov B, D
            state->b = state->d;
            break;
        case 0x43: // mov B, E
            state->b = state->e;
            break;
        case 0x44: // mov B, H
            state->b = state->h;
            break;
        case 0x45: // mov B, L
            state->b = state->l;
            break;
        case 0x46: // mov B, M == mov B, [hl]
            offset = make_word(state->h, state->l);
            state->b = mem_read(state, offset); 
            break;
        case 0x47: // mov B, A
            state->b = state->a;
            break;
        case 0x48: // mov C, B
            state->c = state->b;
            break;
        case 0x49: // mov C, C
            break;
        case 0x4A: // mov C, D
            state->c = state->d;
            break;
        case 0x4B: // mov C, E
            state->c = state->e;
            break;
        case 0x4C: // mov C, H
            state->c = state->h;
            break;
        case 0x4D: // mov C, L
            state->c = state->l;
            break;
        case 0x4E: // mov C, M == mov C, [hl]
            offset = make_word(state->h, state->l);
            state->c = mem_read(state, offset); 
            break;
        case 0x4f: // mov C, A
            state->c = state->a;
            break;
        case 0x50: // mov D, B
            state->d = state->b;
            break;
        case 0x51: // mov D, C
            state->d = state->c;
            break;
        case 0x52: // mov D, d
            break;
        case 0x53: // mov D, E
            state->d = state->e;
            break;
        case 0x54: // mov D, H
            state->d = state->h;
            break;
        case 0x55: // mov D, L
            state->d = state->l;
            break;
        case 0x56: // mov D, M == mov D, [hl]
            offset = make_word(state->h, state->l);
            state->d = mem_read(state, offset); 
            break;
        case 0x57: // mov D, A
            state->d = state->a;
            break;
        case 0x58: // mov e, B
            state->e = state->b;
            break;
        case 0x59: // mov e, C
            state->e = state->c;
            break;
        case 0x5A: // mov E, D
            state->e = state->d;
            break;
        case 0x5B: // mov E, E
            break;
        case 0x5C: // mov E, H
            state->e = state->h;
            break;
        case 0x5D: // mov E, L
            state->e = state->l;
            break;
        case 0x5E: // mov E, M == mov E, [hl]
            offset = make_word(state->h, state->l);
            state->e = mem_read(state, offset); 
            break;
        case 0x5F: // mov E, A
            state->e = state->a;
            break;
        case 0x60: // mov H, B
            state->h = state->b;
            break;
        case 0x61: // mov H, C
            state->h = state->c;
            break;
        case 0x62: // mov H, D
            state->h = state->d;
            break;
        case 0x63: // mov H, E
            state->h = state->e;
            break;
        case 0x64: // mov H, H
            break;
        case 0x65: // mov H, L
            state->h = state->l;
            break;
        case 0x66: // mov H, M == mov H, [hl]
            offset = make_word(state->h, state->l);
            state->h = mem_read(state, offset); 
            break;
        case 0x67: // mov h, A
            state->h = state->a;
            break;
        case 0x68: // mov L, B
            state->l = state->b;
            break;
        case 0x69: // mov L, C
            state->l = state->c;
            break;
        case 0x6A: // mov L, D
            state->l = state->d;
            break;
        case 0x6B: // mov H, E
            state->l = state->e;
            break;
        case 0x6C: // mov L, H
            state->l = state->h;
            break;
        case 0x6D: // mov L, L
            break;
        case 0x6E: // mov L, M == mov L, [hl]
            offset = make_word(state->h, state->l);
            state->l = mem_read(state, offset); 
            break;
        case 0x6F: // mov L, A
            state->l = state->a;
            break;

        case 0x76: // HLT
            break;
        case 0x78: // mov A, B
            state->a = state->b;
            break;
        case 0x79: // mov A, C
            state->a = state->c;
            break;
        case 0x7A: // mov A, D
            state->a = state->d;
            break;
        case 0x7B: // mov H, E
            state->a = state->e;
            break;
        case 0x7C: // mov A, H
            state->a = state->h;
            break;
        case 0x7D: // mov A, A
            break;
        case 0x7E: // mov A, M == mov A, [hl]
            offset = make_word(state->h, state->l);
            state->a = mem_read(state, offset); 
            break;
        case 0x7F: // mov A, A
            break;
        case 0x80: // add B
            add(state, state->b);
            break;
        case 0x81: // add C
            add(state, state->c);
            break;
        case 0x82: // add D
            add(state, state->d);
            break;
        case 0x83: // add E
            add(state, state->e);
            break;
        case 0x84: // add H
            add(state, state->h);
            break;
        case 0x85: // add L
            add(state, state->l);
            break;
        case 0x86: // add M == add [hl]
            offset = make_word(state->h, state->l);
            value = mem_read(state, offset); 
            add(state, value);
            break;
        case 0x87: // add A
            add(state, state->a);
            break;
        case 0x88: // adc B
            adc(state, state->b);
            break;
        case 0x89: // adc C
            adc(state, state->c);
            break;
        case 0x8a: // adc D
            adc(state, state->d);
            break;
        case 0x8b: // adc E
            adc(state, state->e);
            break;
        case 0x8c: // adc H
            adc(state, state->h);
            break;
        case 0x8d: // adc L
            adc(state, state->l);
            break;
        case 0x8e: // adc M == adc [hl]
            offset = make_word(state->h, state->l);
            value = mem_read(state, offset); 
            adc(state, value);
            break;
        case 0x8f: // adc A
            adc(state, state->a);
            break;
        case 0x90: // sub B
            sub(state, state->b);
            break;
        case 0x91: // sub C
            sub(state, state->c);
            break;
        case 0x92: // sub D
            sub(state, state->d);
            break;
        case 0x93: // sub E
            sub(state, state->e);
            break;
        case 0x94: // sub H
            sub(state, state->h);
            break;
        case 0x95: // sub L
            sub(state, state->l);
            break;
        case 0x96: // sub M == sub [hl]
            offset = make_word(state->h, state->l);
            value = mem_read(state, offset); 
            sub(state, value);
            break;
        case 0x97: // sub A
            sub(state, state->a);
            break;
        case 0x98: // sbb B
            sbb(state, state->b);
            break;
        case 0x99: // sbb C
            sbb(state, state->c);
            break;
        case 0x9a: // sbb D
            sbb(state, state->d);
            break;
        case 0x9b: // sbb E
            sbb(state, state->e);
            break;
        case 0x9c: // sbb H
            sbb(state, state->h);
            break;
        case 0x9d: // sbb L
            sbb(state, state->l);
            break;
        case 0x9e: // sbb M == sbb [hl]
            offset = make_word(state->h, state->l);
            value = mem_read(state, offset); 
            sbb(state, value);
            break;
        case 0x9f: // sbb A
            sbb(state, state->a);
            break;
        case 0xc2: // jnz adr
            if (!state->flags.z) jump(state, make_word(opcode[1], opcode[2]));
            else state->pc += 2;
            break;
        case 0xc3: // jmp adr
            jump(state, make_word(opcode[1], opcode[2]));
            break;
        case 0xc4: // cnz adr
            if (!state->flags.z) call(state, make_word(opcode[1], opcode[2]));
            else state->pc += 2;
            break;
        case 0xc8: // rz
            if (state->flags.z) ret(state);
            break;
        case 0xc9: // ret
            ret(state);
            break;
        case 0xca: // jz adr
            if (state->flags.z) jump(state, make_word(opcode[1], opcode[2]));
            else state->pc += 2;
            break;
        case 0xd2: // jnc adr
            if (state->flags.cy) jump(state, make_word(opcode[1], opcode[2]));
            break;
        case 0xda: // jc adr
            if (!state->flags.cy) jump(state, make_word(opcode[1], opcode[2]));
            else state->pc += 2;
            break;
        case 0xdc: //
            if (!state->flags.cy) call(state, make_word(opcode[1], opcode[2]));
            break;
        case 0xcd: // call adr
            call(state, make_word(opcode[1], opcode[2]));
            break;
        case 0xe2: // jpo
            if (state->flags.p) jump(state, make_word(opcode[1], opcode[2]));
            else state->pc += 2;
            break;
        default:
            printf("Panic! Unknown Instruction %s", opcode);
            exit(1);
    }
    state->pc += 1;
}

void add(State8080 *state, uint8_t value) {
    uint16_t sum = (uint16_t) state->a + (uint16_t) value;
    state->flags.z = ((sum & 0xff) == 0);
    state->flags.s = ((sum & 0x80) != 0);
    state->flags.cy = (sum > 0xff);
    state->flags.p = parity(sum & 0xff, 8);
    state->a = sum & 0xff;
}

void adc(State8080 *state, uint8_t value) {
    uint16_t sum = (uint16_t) state->a + (uint16_t) value + (uint16_t) state->flags.cy;
    state->flags.z = ((sum & 0xff) == 0);
    state->flags.s = ((sum & 0x80) != 0);
    state->flags.cy = (sum > 0xff);
    state->flags.p = parity(sum & 0xff, 8);
    state->a = sum & 0xff;
}

void sub(State8080 *state, uint8_t value) {
    uint16_t diff = (uint16_t) state->a - (uint16_t) value;
    state->flags.z = ((diff & 0xff) == 0);
    state->flags.s = ((diff & 0x80) != 0);
    state->flags.cy = (diff > 0xff);
    state->flags.p = parity(diff & 0xff, 8);
    state->a = diff & 0xff;
}

void sbb(State8080 *state, uint8_t value) {
    uint16_t diff = (uint16_t) state->a - (uint16_t) value - (uint16_t) state->flags.cy;
    state->flags.z = ((diff & 0xff) == 0);
    state->flags.s = ((diff & 0x80) != 0);
    state->flags.cy = (diff > 0xff);
    state->flags.p = parity(diff & 0xff, 8);
    state->a = diff & 0xff;
}

void call(State8080 *state, uint16_t addr) {
    uint16_t offset = state->pc + 2;
    state->memory[state->sp - 1] = (offset >> 8) & 0xff;
    state->memory[state->sp - 2] = offset & 0xff;
    state->sp = state->sp - 2;
    state->pc = addr;
}

void ret(State8080 *state) {
    uint16_t offset = state->pc;
    state->pc = make_word(state->memory[offset], state->memory[offset + 1]);
    state->sp += 2;
}

void jump(State8080 *state, uint16_t addr) {
    state->pc = addr;
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

void mem_write(State8080 *state, uint16_t offset, uint8_t value) {
	state->memory[offset] = value;
}

uint8_t mem_read(State8080 *state, uint16_t offset) {
	return state->memory[offset];
}
void main() {}
