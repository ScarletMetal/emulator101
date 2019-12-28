cycle(State8080 *state) {
    unsigned char *opcode = &state->memory[state->pc];

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
            uint16_t offset = 256U * ((uint16_t) state->h) + (uint16_t) state->l;
            state->b = state->memory[offset];
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
            uint16_t offset = 256U * ((uint16_t) state->h) + (uint16_t) state->l;
            state->c = state->memory[offset];
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
            uint16_t offset = 256U * ((uint16_t) state->h) + (uint16_t) state->l;
            state->d = state->memory[offset];
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
            uint16_t offset = 256U * ((uint16_t) state->h) + (uint16_t) state->l;
            state->e = state->memory[offset];
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
            state->h = state->e
            break;
        case 0x64: // mov H, H
            break;
        case 0x65: // mov H, L
            state->h = state->l;
            break;
        case 0x66: // mov H, M == mov H, [hl]
            uint16_t offset = 256U * ((uint16_t) state->h) + (uint16_t) state->l;
            state->h = state->memory[offset];
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
            state->l = state->e
            break;
        case 0x6C: // mov L, H
            state->l = state->h;
            break;
        case 0x6D: // mov L, L
            break;
        case 0x6E: // mov L, M == mov L, [hl]
            uint16_t offset = 256U * ((uint16_t) state->h) + (uint16_t) state->l;
            state->l = state->memory[offset];
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
            state->a = state->e
            break;
        case 0x7C: // mov A, H
            state->a = state->h;
            break;
        case 0x7D: // mov A, A
            break;
        case 0x7E: // mov A, M == mov A, [hl]
            uint16_t offset = 256U * ((uint16_t) state->h) + (uint16_t) state->l;
            state->a = state->memory[offset];
            break;
        case 0x7E: // mov A, A
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
            uint16_t offset = 256U * ((uint16_t) state->h) + (uint16_t) state->l;
            uint8_t value = state->memory[offset];
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
            uint16_t offset = 256U * ((uint16_t) state->h) + state->l;
            uint8_t value = state->memory[offset];
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
            uint16_t offset = 256U * ((uint16_t) state->h) + (uint16_t) state->l;
            uint8_t value = state->memory[offset];
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
        case 0x9e: // sbb M == sbb [bc]
            uint16_t offset = 256U * ((uint16_t) state->h) + (uint16_t) state->l;
            uint8_t value = state->memory[offset];
            sbb(state, value);
            break;
        case 0x9f: // sbb A
            sbb(state, state->a);
            break;
        default:
            unimplemented_instruction(state);
    }
    state->pc += 1;
}

void add(State8080 *state, uint8_t value) {
    uint16_t sum = (uint16_t) state->a + (uint16_t) value;
    state->cc.z = ((sum & 0xff) == 0);
    state->cc.s = ((sum & 0x80) != 0);
    state->cc.cy = (sum > 0xff);
    state->a = sum & 0xff;
}

void adc(State8080 *state, uint8_t value) {
    uint16_t sum = (uint16_t) state->a + (uint16_t) value + (uint16_t) state->cc.cy;
    state->cc.z = ((sum & 0xff) == 0);
    state->cc.s = ((sum & 0x80) != 0);
    state->cc.cy = (sum > 0xff);
    state->a = sum & 0xff;
}

void sub(State8080 *state, uint8_t value) {
    uint16_t diff = (uint16_t) state->a - (uint16_t) value;
    state->cc.z = ((diff & 0xff) == 0);
    state->cc.s = ((diff & 0x80) != 0);
    state->cc.cy = (diff > 0xff);
    state->a = diff & 0xff;
}

void sbb(State8080 *state, uint8_t value) {
    uint16_t diff = (uint16_t) state->a - (uint16_t) value - (uint16_t) state->cc.cy;
    state->cc.z = ((diff & 0xff) == 0);
    state->cc.s = ((diff & 0x80) != 0);
    state->cc.cy = (diff > 0xff);
    state->a = diff & 0xff;
}
