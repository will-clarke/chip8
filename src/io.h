#ifndef IO_H
#define IO_H
#include "cpu.h"

#define DISPLAY_H 32
#define DISPLAY_W 64


 /*  --------------- */
 /* |(0,0) 	(63,0) | */
 /* |(0,31)	(63,31)| */
 /*  --------------- */

void process_input(struct cpu*);
void clear_screen();

#endif
