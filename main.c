#include "cpu.h"
#include "io.h"
#include <stdio.h>


void dump_memory(struct cpu* cpu);
uint16_t get_byte(uint8_t *memory, uint8_t pointer_to_memory);

int main()
{

  char step = 0;
  int count = 0;
  struct cpu cpu;

  init_cpu(&cpu);

#ifdef DEBUG
  dump_memory(&cpu);
#endif

  for(count = 0; count < 5 ; count ++)
    {
      if(step)
        getchar();
      uint16_t opcode = get_byte(cpu.memory, cpu.pc);
#ifdef DEBUG
      printf("opcode %d    = %0x4\n", count, opcode);
      printf("  current pc = %d\n", cpu.pc);
#endif
      execute_opcode(opcode, &cpu);

      process_input(&cpu);
    }

}

void dump_memory(struct cpu* cpu)
{
  FILE *log = fopen("memory_dump.log", "w");
  for(int i = 0; i < (int)sizeof(cpu->V); i++)
    fprintf(log, "Register %4.d: %c\n", i, *(cpu->V + i));
  fprintf(log, "\n===============================\n\n");
  for(int i = 0; i < (int)sizeof(cpu->memory); i += 2 ){
    uint16_t byte = get_byte(cpu->memory, i);
    fprintf(log, "Mem byte %4.d : %04x\n", i, byte);
  }
}

uint16_t get_byte(uint8_t *memory, uint8_t pointer_to_memory)
{
  return *(memory + pointer_to_memory) << 8 | *(memory + pointer_to_memory + 1);
}
