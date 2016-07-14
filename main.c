#include "cpu.h"
#include <stdio.h>
void dump_memory(struct cpu *);

int main()
{

  struct cpu cpu;

  init_cpu(&cpu);

#ifdef DEBUG
  dump_memory(&cpu);
#endif
}

void dump_memory(struct cpu* cpu)
{
  FILE *log = fopen("memory_dump.log", "w");
  for(int i = 0; i < (int)sizeof(cpu->V); i++)
    fprintf(log, "Register %4.d: %c\n", i, *(cpu->V + i));
  fprintf(log, "\n===============================\n\n");
  for(int i = 0; i < (int)sizeof(cpu->memory); i += 2 ){
    uint16_t byte = *(cpu->memory + i) << 8 | *(cpu->memory + i + 1);
    fprintf(log, "Mem byte %4.d : %04x\n", i, byte);
  }
}
