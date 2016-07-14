#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include <string.h>

struct cpu
{
  uint8_t memory[0x1000];        // 4096
  uint8_t V[0xF];                // 16
  uint8_t stack[0xF];            // 16
  uint8_t graphics[0x20 * 0x40]; // 32 * 64
  uint8_t keyboard[0xF];

  uint16_t I;
  uint8_t pc;
  uint8_t delay_timer;
  uint8_t sound_timer;
};



/* Memory Map */
/* ========= */
// todo


void init_cpu(struct cpu* cpu);
void execute_opcode(uint16_t opcode);

#endif
