#include "cpu.h"
#include "debug.h"
#include "io.h"
#include "opcodes.h"
#include <time.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  char step = 0;
  int count = 0;
  struct cpu cpu;

  init_cpu(&cpu);
  load_rom(&cpu, argc, argv);

  /* initscr(); */
  /* noecho(); */
  /* cbreak(); */
  /* curs_set(0); */
  /* refresh(); */
  WINDOW* window = setup_ncurses();
#ifdef DEBUG
  FILE* running_log = fopen("running_log.log", "wa");
#endif

  cpu.pc = 0x200;
#ifdef DEBUG
  fprintf(running_log, "starting\n");
#endif
  for(;;) // (count = 0; count < 5 ; count ++)
    {
      count ++;
      if(step)
        getchar();
      /* 6a02 6b0c 6c3f 6d0c a2ea da */
      process_keyboard_input(&cpu);
      uint16_t opcode = get_byte(cpu.memory, cpu.pc);
#ifdef DEBUG
      dump_memory(&cpu);
      fprintf(running_log, "------------------\n");
      fprintf(running_log, "opcode %d    = %0x\n", count, opcode);
      fprintf(running_log, "  current pc = %d\n", cpu.pc);
      fprintf(running_log, "  Stepping enabled? = %d\n", cpu.stepping.stepping);
      fflush(running_log);
#endif
      execute_opcode(opcode, &cpu);
      output_display(window, &cpu);
      decrease_timers(&cpu);
      usleep(100);
    }
  end_ncurses();
}
