#include <stdio.h>
#include "cpu.h"
#include "debug.h"

void dump_memory(struct cpu* cpu)
{
  FILE *log = fopen("logs/memory_dump", "w");

  fprintf(log, "Current Opcode: %x\n\n", cpu->current_opcode);
  fprintf(log, "Stack\n===========================\n");
  for(int i = 0; i < (int)sizeof(cpu->stack.stack); i += 2){
    fprintf(log, " %x: %04x", i / 2, (cpu->stack.stack[i]) << 8 | cpu->stack.stack[i+1]);
    if(cpu->stack.stack_pointer == i)
      fprintf(log, " <------ Current Stack Pointer");
    fprintf(log, "\n");
  }

  fprintf(log, "\nRegisters\n===========================\n");
  fprintf(log, "  I: %d\n", cpu->I);
  for(int i = 0; i < (int)sizeof(cpu->V); i++)
    fprintf(log, "  %4.d: %04x\n", i, *(cpu->V + i));

  fprintf(log, "\n\nKeyboard\n---------\n\n");
  for (int i = 0; i < 0xF; i++) { // rows
    fprintf(log, "  key %d: %d\n", i, cpu->keyboard[i]);
  }

  fprintf(log, "\n\nDisplay\n---------\n\n");
  for (int j = 0; j < 32; j++) {
    for (int i = 0; i < 64; i++)
      fprintf(log, "%c", cpu->display[(64 * j) + i] ? '#' : ' ');
    fprintf(log, "\n");
  }

  /* for(int i = 0; i < (int)sizeof(cpu->display); i += 32){ */
  /*   /\* fprintf(log, "\n - %d / %d\n", i, ((int)sizeof(cpu->display) / 64); *\/ */
  /*   for(int j = 0; j < 64; j++){ */
  /*     /\* fprintf(log, "%d / %d\n", (i + j, (int)sizeof(cpu->display)); *\/ */
  /*     fprintf(log, "%d", cpu->display[i + j]); */
  /*   } */
  /*   fprintf(log, "\n"); */
  /* } */

  fprintf(log, "\n\n\nMemory\n===============================\n\n");
  for(int i = 0; i < (int)sizeof(cpu->memory); i += 2 ){
    uint16_t byte = get_opcode(cpu->memory, (uint16_t)i);
    fprintf(log, " byte # %4.d : %04x\n", i, byte);
  }

  fflush(log);
  fclose(log);
}
