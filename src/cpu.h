#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include <string.h>

struct stack
{ uint16_t stack[0x10];
  int8_t stack_pointer;};

int stack_pop(struct stack*);
int stack_push(struct stack*, uint16_t);

struct cpu
{
  uint8_t memory[0x1000];        // 4096
  uint8_t V[0x10];               // 16
  struct stack stack;
  uint8_t graphics[0x20 * 0x40]; // 32 * 64
  uint8_t keyboard[0x10];        // 16

  uint16_t I;
  uint16_t pc;
  uint8_t delay_timer;
  uint8_t sound_timer;
};



/* Memory Map */
/* ========= */
// todo


void init_cpu(struct cpu* cpu);
void execute_opcode(uint16_t opcode, struct cpu* cpu);

#endif
