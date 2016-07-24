#include "cpu.h"
#include "debug.h"
#include "io.h"
#include "opcodes.h"
#include <time.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  struct cpu cpu;

  init_cpu(&cpu);
  load_rom(&cpu, argc, argv);

  WINDOW* window = setup_ncurses();

  /* initscr(); */
  noecho();
  cbreak();
  /* nodelay(window, 1); */
  curs_set(0);
  /* refresh(); */

#ifdef DEBUG
  FILE* running_log = fopen("logs/running_log", "wa");
  int count = 0;
#endif

  cpu.pc = 0x200;
#ifdef DEBUG
  fprintf(running_log, "starting\n");
#endif
  for(;;)
    {
      process_keyboard_input(&cpu, window);
      uint16_t opcode = get_byte(cpu.memory, cpu.pc);
      execute_opcode(opcode, &cpu);
      output_display(&cpu, window);
      decrease_timers(&cpu);
      /* usleep(10); */

#ifdef DEBUG
      dump_memory(&cpu);
      fprintf(running_log, "------------------\n");
      count ++;
      fprintf(running_log, "opcode %d    = %0x\n", count, opcode);
      fprintf(running_log, "  current pc = %d\n", cpu.pc);
      fprintf(running_log, "  Stepping enabled? = %d\n", cpu.stepping.stepping);
      fprintf(running_log, "  I = %d\n", cpu.I);
      fflush(running_log);
#endif
    }
  end_ncurses();
}
