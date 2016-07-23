#include <stdio.h>
#include "cpu.h"
#include "debug.h"

void dump_memory(struct cpu* cpu)
{
  FILE *log = fopen("memory_dump.log", "w");

  fprintf(log, "Current Opcode: %x\n\n", cpu->current_opcode);
  fprintf(log, "Stack\n===========================\n");
  for(int i = 0; i < (int)sizeof(cpu->stack.stack); i += 2){
    fprintf(log, " %3.d: %04x", i, (cpu->stack.stack[i]) << 8 | cpu->stack.stack[i+1]);
    if(cpu->stack.stack_pointer == i)
      fprintf(log, " <------ Current Stack Pointer");
    fprintf(log, "\n");
  }
  fprintf(log, "Registers\n===========================\n");
  for(int i = 0; i < (int)sizeof(cpu->V); i++)
    fprintf(log, "  %4.d: %04x\n", i, *(cpu->V + i));
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
  for(int i = 0; i < (int)sizeof(cpu->memory); i += 2 ){
    uint16_t byte = get_byte(cpu->memory, (uint16_t)i);
    fprintf(log, "Mem byte %4.d : %04x\n", i, byte);
  }
}
