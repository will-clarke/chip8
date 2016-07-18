#include "cpu.h"
#include "debug.h"
#include "io.h"
#include <stdio.h>

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
