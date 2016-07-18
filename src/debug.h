#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include "cpu.h"

uint16_t get_byte(uint8_t *memory, uint8_t pointer_to_memory);
void dump_memory(struct cpu* cpu);

#endif
