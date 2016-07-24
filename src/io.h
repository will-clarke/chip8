#ifndef IO_H
#define IO_H
#include "cpu.h"
#include <ncurses.h>

#define DISPLAY_H 32
#define DISPLAY_W 64


 /*  --------------- */
 /* |(0,0) 	(63,0) | */
 /* |(0,31)	(63,31)| */
 /*  --------------- */

void clear_screen();
void load_rom(struct cpu* cpu, int argc, char *argv[]);
void process_keyboard_input(struct cpu* cpu);
WINDOW * setup_ncurses();
void end_ncurses();

uint16_t next_opcode(struct cpu* cpu, int n);

void output_display(struct cpu* cpu, WINDOW*);
#endif
