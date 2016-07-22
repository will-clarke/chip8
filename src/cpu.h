#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include <string.h>

struct stack
{ uint16_t stack[0x10];
  int8_t stack_pointer;};

int stack_pop(struct stack*);
int stack_push(struct stack*, uint16_t);

#ifdef DEBUG
struct step
{
  uint8_t stepping;
  uint8_t tries;
};
#endif

struct cpu
{
  uint8_t memory[0x1000];        // 4096
  uint8_t V[0x10];               // 16
  struct stack stack;
#ifdef DEBUG
  struct step stepping;          // Do we want to step through bit by bit?
#endif
  uint8_t display[0x20 * 0x40];  // 32 * 64
  uint8_t keyboard[0x10];        // 16

  uint16_t I;
  uint16_t pc;
  uint8_t delay_timer;
  uint8_t sound_timer;
};

void init_cpu(struct cpu* cpu);

uint16_t get_byte(uint8_t *memory, uint16_t pointer_to_memory);


void decrease_timers(struct cpu* cpu);
#endif
