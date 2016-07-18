#include <stdio.h>
#include "cpu.h"
#include "debug.h"

void dump_memory(struct cpu* cpu)
{
  FILE *log = fopen("memory_dump.log", "w");
  for(int i = 0; i < (int)sizeof(cpu->V); i++)
    fprintf(log, "Register %4.d: %c\n", i, *(cpu->V + i));
  fprintf(log, "\n===============================\n\n");
  for(int i = 0; i < (int)sizeof(cpu->memory); i += 2 ){
    uint16_t byte = get_byte(cpu->memory, (uint16_t)i);
    fprintf(log, "Mem byte %4.d : %04x\n", i, byte);
  }
}

uint16_t get_byte(uint8_t *memory, uint8_t pointer_to_memory)
{
  return *(memory + pointer_to_memory) << 8 | *(memory + pointer_to_memory + 1);
}
