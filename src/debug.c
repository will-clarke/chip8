#include <stdio.h>
#include "cpu.h"
#include "debug.h"

void dump_memory(struct cpu* cpu)
{
  FILE *log = fopen("memory_dump.log", "w");
  for(int i = 0; i < (int)sizeof(cpu->V); i++)
    fprintf(log, "Register %4.d: %04x\n", i, *(cpu->V + i));
  fprintf(log, "\n===============================\n\nDisplay\n---------\n\n");
  for(int i = 0; i < (int)sizeof(cpu->display); i += 64){
    /* fprintf(log, "\n - %d / %d\n", i, ((int)sizeof(cpu->display) / 64); */
    for(int j = 0; j < 32; j++){
      /* fprintf(log, "%d / %d\n", (i + j, (int)sizeof(cpu->display)); */
      fprintf(log, "%3d", cpu->display[i + j]);
    }
    fprintf(log, "\n");
  }
  fprintf(log, "\n===============================\n\n");
  fprintf(log, "\n===============================\n\n");
    for(int i = 0; i < (int)sizeof(cpu->display); i += 2 ){
      uint16_t byte = get_byte(cpu->display, (uint16_t)i);
      fprintf(log, "DISPLAY %4.d : %04x\n", i, byte);
    }
  fprintf(log, "\n===============================\n\n");
  for(int i = 0; i < (int)sizeof(cpu->memory); i += 2 ){
    uint16_t byte = get_byte(cpu->memory, (uint16_t)i);
    fprintf(log, "Mem byte %4.d : %04x\n", i, byte);
  }
}
