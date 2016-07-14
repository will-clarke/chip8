#include "cpu.h"
#include <stdio.h>

int main()
{

  struct cpu cpu;

  init_cpu(&cpu);

#ifdef DEBUG

  FILE *log = fopen("memory_dump.log", "w");
  for(int i = 0; i < sizeof(cpu.V); i++)
    fprintf(log, "register %.4d: %c\n", i, *(cpu.V + i));

  fprintf(log, "\n===============================\n\n");

  for(int i = 0; i < sizeof(cpu.memory); i += 2 ){
    uint16_t byte = *(cpu.memory + i) << 8 | *(cpu.memory + i + 1);
    fprintf(log, "mem byte: %4.d : %04x\n", i, byte);
  }

#endif
}
