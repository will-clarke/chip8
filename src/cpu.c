#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "cpu.h"
#include "io.h"
#include "debug.h"

void init_cpu(struct cpu* cpu)
{
  srand(time(0));
  memset( cpu->memory, 0, sizeof(cpu->memory) );
  memset( cpu->V, 0, sizeof(cpu->V) );
  memset( cpu->stack.stack, 0, sizeof(cpu->stack) );
  cpu->stack.stack_pointer = 0;
  memset( cpu->display, 0, sizeof(cpu->display) );
  memset( cpu->keyboard, 0, sizeof(cpu->keyboard) );
  cpu->I = 0x0;
  cpu->pc = 0x200;
  cpu->delay_timer = 0;
  cpu->sound_timer = 0;
  uint8_t fonts[80] = { 0xF0, 0x90, 0x90, 0x90, 0xF0, //0
                        0x20, 0x60, 0x20, 0x20, 0x70, //1
                        0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
                        0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
                        0x90, 0x90, 0xF0, 0x10, 0x10, //4
                        0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
                        0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
                        0xF0, 0x10, 0x20, 0x40, 0x40, //7
                        0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
                        0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
                        0xF0, 0x90, 0xF0, 0x90, 0x90, //A
                        0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
                        0xF0, 0x80, 0x80, 0x80, 0xF0, //C
                        0xE0, 0x90, 0x90, 0x90, 0xE0, //D
                        0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
                        0xF0, 0x80, 0xF0, 0x80, 0x80  //F
  };
  memcpy(cpu->memory, fonts, sizeof(fonts) * sizeof(fonts[0]));
}

/* 11111111       0xF0        <= example font for 0: 0xF0, 0x90, 0x90, 0x90, 0xF0 */
/* 10000001       0x90  */
/* 10000001       0x90  */
/* 10000001       0x90  */
/* 11111111       0xF0  */




int stack_pop(struct stack* stack){
  if(stack->stack_pointer > 0)
    return *(stack->stack + --stack->stack_pointer);
  else {
    printf("Empty Stack Popped\n");
    exit(1);
  }
}

int stack_push(struct stack* stack, uint16_t n){
  int stack_elems_count = (int)sizeof(stack->stack) / sizeof(stack->stack[0]);
  if(stack->stack_pointer < stack_elems_count) {
    stack->stack[stack->stack_pointer++] = n;
    return 1;
  }
  else {
    printf("Stack Overflow!\n");
    exit(1);
  }
}

uint16_t get_byte(uint8_t *memory, uint16_t pointer_to_memory)
{
  return *(memory + pointer_to_memory) << 8 | *(memory + pointer_to_memory + 1);
}

void decrease_timers(struct cpu* cpu){
  if(cpu->delay_timer > 0)
    (cpu->delay_timer)--;
  if(cpu->sound_timer > 0){
    (cpu->sound_timer)--;
    if(cpu->sound_timer == 0)
      printf("\a");
    // BEEP!
  }
}
