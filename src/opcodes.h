#ifndef OPCODES_H
#define OPCODES_H

#include "cpu.h"

void increment_pc(struct cpu* cpu, int n);
void execute_opcode(uint16_t opcode, struct cpu* cpu);

#endif
