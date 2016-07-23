#ifdef DEBUG
#include "debug.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "io.h"


void increment_pc(struct cpu* cpu, int n){
  cpu->pc += n * 2;
}

/*   nnn or addr - A 12-bit value,
     the lowest 12 bits of the instruction */

/*   n or nibble - A 4-bit value,
     the lowest 4 bits of the instruction */

/*   x - A 4-bit value, the lower 4 bits
     of the high byte of the instruction */

/*   y - A 4-bit value, the upper 4 bits
     of the low byte of the instruction */

/*   kk or byte - An 8-bit value, the
     lowest 8 bits of the instruction */

void execute_opcode(uint16_t opcode, struct cpu* cpu)
{
  cpu->current_opcode = opcode;
#ifdef DEBUG
  dump_memory(cpu);
  FILE* running_log = fopen("running_log.log", "wa");
  fprintf(running_log, "* Executing %4x", opcode);
#endif
  switch(opcode & 0xF000)
    {
    case(0x0000):
      switch(opcode)
        {
          /* 0nnn - SYS addr */
          /* Jump to a machine code routine at nnn. */
          /* This instruction is only used on the old computers on which Chip-8 was originally implemented. It is ignored by modern interpreters. */

          /* 00E0 - CLS */
          /* Clear the display. */
        case(0x00E0):
          memset(cpu->display, 0, sizeof(cpu->display));
          // handled in io::output_display
          increment_pc(cpu, 1);
#ifdef DEBUG
          fprintf(running_log, "    clearing screen");
#endif
          break;

          /* 00EE - RET */
          /* Return from a subroutine. */
          /* The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer. */
        case(0x00EE):
          cpu->pc = stack_pop(&(cpu->stack));
          cpu->pc += 2; // to move past the CALL opcode
#ifdef DEBUG
          fprintf(running_log, "    returning from subroutine: stack popped");
#endif
          break;
        }
      break;
    case(0x1000): {
      /* 1nnn - JP addr */
      /* Jump to location nnn. */
      /* The interpreter sets the program counter to nnn. */
      uint16_t address = opcode & 0x0FFF;
      cpu->pc = address;
    }
      break;
    case(0x2000): {
      /* 2nnn - CALL addr */
      /* Call subroutine at nnn. */
      /* The interpreter increments the stack pointer,
         then puts the current PC on the top of the stack.
         The PC is then set to nnn. */
      uint16_t nnn = opcode & 0x0FFF;
      stack_push(&cpu->stack, cpu->pc);
      cpu->pc = nnn;
    }
      break;
    case(0x3000):{
      /* 3xkk - SE Vx, byte */
      /* Skip next instruction if Vx = kk. */
      /* The interpreter compares register
         Vx to kk, and if they are equal,
         increments the program counter by 2. */
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t kk = opcode & 0x00FF;
      char is_equal = cpu->V[x] == kk;
      if(is_equal)
        increment_pc(cpu, 2);
      else
        increment_pc(cpu, 1);
    }
      break;
    case(0x4000):{
      /* 4xkk - SNE Vx, byte */
      /* Skip next instruction if Vx != kk. */
      /* The interpreter compares register
         Vx to kk, and if they are not equal,
         increments the program counter by 2. */
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t kk = opcode & 0x00FF;
      char is_not_equal = cpu->V[x] != kk;
      if(is_not_equal)
        increment_pc(cpu, 2);
      else
        increment_pc(cpu, 1);
      break;
    }
    case(0x5000):{
      /* 5xy0 - SE Vx, Vy */
      /* Skip next instruction if Vx = Vy. */
      /* The interpreter compares
         register Vx to register Vy,
         and if they are equal, increments
         the program counter by 2. */
      uint8_t x, y;
      x = (opcode & 0x0F00) >> 8;
      y = (opcode & 0x00F0) >> 4;
      if(cpu->V[x] == cpu->V[y])
        increment_pc(cpu, 2);
      else
        increment_pc(cpu, 1);
      break;
    }
    case(0x6000):{
      /* 6xkk - LD Vx, byte */
      /* Set Vx = kk. */
      /* The interpreter puts the value
         kk into register Vx. */
      uint8_t _register = (opcode & 0x0F00) >> 8;
      uint8_t value = opcode & 0x00FF;
      cpu->V[_register] = value;
      increment_pc(cpu, 1);
      break;
    }
    case(0x7000):{
      /* 7xkk - ADD Vx, byte */
      /* Set Vx = Vx + kk. */
      /* Adds the value kk to the value of register Vx, then stores the result in Vx. */
      uint8_t _register = (opcode & 0x0F00) >> 8;
      uint8_t value = opcode & 0x00FF;
      cpu->V[_register] += value;
      increment_pc(cpu, 1);
      break;}
    case(0x8000):{
      switch(opcode & 0x000F){
      case(0x0): {
        /* 8xy0 - LD Vx, Vy */
        /* Set Vx = Vy. */
        /* Stores the value of register
           Vy in register Vx. */
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;
        cpu->V[x] = cpu->V[y];
        increment_pc(cpu, 1);
        break;
      }
      case(0x1): {
        /* 8xy1 - OR Vx, Vy */
        /* Set Vx = Vx OR Vy. */
        /* Performs a bitwise OR on the values of Vx and Vy,
           then stores the result in Vx. */
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;
        cpu->V[x] |= cpu->V[y];
        increment_pc(cpu, 1);
        break;
      }

      case(0x2): {
        /* 8xy2 - AND Vx, Vy */
        /* Set Vx = Vx AND Vy. */
        /* Performs a bitwise AND on the values of
           Vx and Vy, then stores the result in Vx.
           A bitwise AND compares the corrseponding bits
           from two values, and if both bits are 1, then
           the same bit in the result is also 1. Otherwise, it is 0. */
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;
        cpu->V[x] &= cpu->V[y];
        increment_pc(cpu, 1);
        break;
      }
      case(0x3): {
        /* 8xy3 - XOR Vx, Vy */
        /* Set Vx = Vx XOR Vy. */
        /* Performs a bitwise exclusive OR on the
           values of Vx and Vy, then stores the result in Vx.
           An exclusive OR compares the corrseponding bits from two values,
           and if the bits are not both the same, then the corresponding bit
           in the result is set to 1. Otherwise, it is 0. */

        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;
        cpu->V[x] ^= cpu->V[y];
        increment_pc(cpu, 1);
        break;
      }
      case(0x4): {
        /* 8xy4 - ADD Vx, Vy */
        /* Set Vx = Vx + Vy, set VF = carry. */
        /* The values of Vx and Vy are added together.
           If the result is greater than 8 bits (i.e., > 255,)
           VF is set to 1, otherwise 0.
           Only the lowest 8 bits of the result are kept,
           and stored in Vx. */

        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;
        cpu->V[x] += cpu->V[y];
        increment_pc(cpu, 1);
        break;
      }
      case(0x5): {
        /* 8xy5 - SUB Vx, Vy */
        /* Set Vx = Vx - Vy, set VF = NOT borrow. */
        /* If Vx > Vy, then VF is set to 1,
           otherwise 0. Then Vy is subtracted from Vx,
           and the results stored in Vx. */
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;
        cpu->V[x] -= cpu->V[y];
        increment_pc(cpu, 1);
        break;
      }
      case(0x6): {
        /* 8xy6 - SHR Vx {, Vy} */
        /* Set Vx = Vx SHR 1. */
        /* If the least-significant bit of Vx is 1,
           then VF is set to 1, otherwise 0.
           Then Vx is divided by 2. */
        uint8_t x = (opcode & 0x0F00) >> 8;
        /* uint8_t y = (opcode & 0x00F0) >> 4; */
        if (cpu->V[x] & 1)
          cpu->V[0xF] = 1;
        else
          cpu->V[0xF] = 0;
        cpu->V[x] /= 2;
        increment_pc(cpu, 1);
        break;
      }
      case(0x7): {
        /* 8xy7 - SUBN Vx, Vy */
        /* Set Vx = Vy - Vx, set VF = NOT borrow. */
        /* If Vy > Vx, then VF is set to 1, otherwise 0.
           Then Vx is subtracted from Vy, and
           the results stored in Vx. */
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;
        if(cpu->V[y] > cpu->V[x])
          cpu->V[0xF] = 1;
        else
          cpu->V[0xF] = 0;
        cpu->V[x] =  cpu->V[y] -  cpu->V[x];
        increment_pc(cpu, 1);
        break;
      }
      case(0xE): {
        uint8_t x = (opcode & 0x0F00) >> 8;
        /* 8xyE - SHL Vx {, Vy} */
        /* Set Vx = Vx SHL 1. */
        /* If the most-significant bit of Vx is 1,
           then VF is set to 1, otherwise to 0.
           Then Vx is multiplied by 2. */
        if(cpu->V[x] >> 3)
          cpu->V[0xF] = 1;
        else
          cpu->V[0xF] = 0;
        cpu->V[x] = (cpu->V[x] << 1) & 0xF;
        increment_pc(cpu, 1);
        break;
      }
      }
    }
      break;
    case(0x9000):
      {
        /* 9xy0 - SNE Vx, Vy */
        /* Skip next instruction if Vx != Vy. */
        /* The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2. */
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;
        if(cpu->V[x] != cpu->V[y])
        increment_pc(cpu, 2);
        else
        increment_pc(cpu, 1);
        break;
      }
    case(0xA000):
      {
        /* Annn - LD I, addr */
        /* Set I = nnn. */
        /* The value of register I is set to nnn. */
        uint16_t nnn = (opcode & 0x0FFF);
        cpu->I = nnn;
        increment_pc(cpu, 1);
        break;
      }
    case(0xB000):
      {
        /* Bnnn - JP V0, addr */
        /* Jump to location nnn + V0. */

        /* The program counter is set to nnn plus the value of V0. */
        uint16_t nnn = (opcode & 0x0FFF);
        cpu->pc = cpu->V[0] + nnn;
        break;
      }
    case(0xC000):
      {
        /* Cxkk - RND Vx, byte */
        /* Set Vx = random byte AND kk. */
        /* The interpreter generates a random number from 0 to 255,
           which is then ANDed with the value kk. The results are
           stored in Vx. See instruction 8xy2 for more information on AND. */
        uint8_t random_number = (uint8_t)rand();
        uint8_t v_register = (opcode & 0x0F00) >> 8;
        uint8_t kk = (opcode & 0x00FF);
        cpu->V[v_register] = random_number & kk;
        increment_pc(cpu, 1);
      }
    case(0xD000):
      {
        /* Dxyn - DRW Vx, Vy, nibble */
        /* Display n-byte sprite starting at memory
           location I at (Vx, Vy), set VF = collision. */
        /* The interpreter reads n bytes from memory, starting at the
           address stored in I. These bytes are then displayed as sprites
           on screen at coordinates (Vx, Vy).
           Sprites are XORed onto the existing screen. If this causes any
           pixels to be erased, VF is set to 1, otherwise it is set to 0.
           If the sprite is positioned so part of it is outside the coordinates
           of the display, it wraps around to the opposite side of the screen.*/


        // N.B These individual BITS from the mem[I] location should get put to display.
        // So we should do something funky with >>2, >>3, >>4, etc... to isolate the bits.
        uint8_t x = cpu->V[(opcode & 0x0F00) >> 8];
        uint8_t y = cpu->V[(opcode & 0x00F0) >> 4];
        uint8_t n = (opcode & 0x000F);
        uint8_t collision = 0;

        for(int i = 0; i < n; i++){
          uint16_t row = ((y * DISPLAY_W) + (i * DISPLAY_W)) % (DISPLAY_W * DISPLAY_H);
          uint16_t col = x % DISPLAY_W;
          uint16_t display_x_y = row + col % (DISPLAY_W * DISPLAY_H);
          uint16_t memory_for_display = cpu->memory[cpu->I + i];
          uint8_t the_actual_xor = cpu->display[display_x_y] ^= memory_for_display;
          uint8_t xor_collision = the_actual_xor != memory_for_display;
          if(xor_collision)
            collision = xor_collision;
        }
        cpu->V[0xF] = collision;
        increment_pc(cpu, 1);
        break;
      }
    case(0xE000):
      {
        switch(opcode & 0xE0FF){
        case(0xE09E):{
          /* Ex9E - SKP Vx */
          /* Skip next instruction if key with
             the value of Vx is pressed. */
          /* Checks the keyboard, and if the key
             corresponding to the value of Vx is
             currently in the down position,
             PC is increased by 2. */
          uint8_t x = (opcode & 0x0F00) >> 8;
          if (cpu->keyboard[x])
            // if key is pressed
            increment_pc(cpu, 2);
          else
            increment_pc(cpu, 1);
          break;
        }

        case(0xE0A1): {
          /* ExA1 - SKNP Vx */
          /* Skip next instruction if key with the value of Vx is not pressed. */
          /* Checks the keyboard, and if the key corresponding
             to the value of Vx is currently in the up position,
             PC is increased by 2. */
          uint8_t x = (opcode & 0x0F00) >> 8;
          if (cpu->keyboard[x])
            // if key is pressed
            increment_pc(cpu, 1);
          else
            increment_pc(cpu, 2);
          break;
        }
        }
        break;
      }
    case(0xF000):
      {

        switch(opcode & 0xF0FF){

        case(0xF007): {
          /* Fx07 - LD Vx, DT */
          /* Set Vx = delay timer value. */
          /* The value of DT is placed into Vx. */
          uint8_t x = (opcode & 0x0F00) >> 8;
          cpu->V[x] = cpu->delay_timer;
          increment_pc(cpu, 1);
          break;
        }
        case(0xF00A): {
          /* Fx0A - LD Vx, K */
          /* Wait for a key press, store the
             value of the key in Vx. */
          /* All execution stops until a key is pressed,
             then the value of that key is stored in Vx. */
          uint8_t x = (opcode & 0x0F00) >> 8;
          uint8_t key_number = 0;
          uint8_t key_value = 0;
          // dunno if I should be zeroing out the whole keyboard. Meh...
          for(int i = 0; i < 0x10; i++)
            cpu->keyboard[i] = 0;
          for(;;){
            if((key_value = cpu->keyboard[key_number])){
              cpu->V[x] = key_number;
              break;
            }
            key_number++;
            key_number %= 0xF;
            increment_pc(cpu, 1);
          }
          break;
        }
        case(0xF015): {
          /* Fx15 - LD DT, Vx */
          /* Set delay timer = Vx. */
          /* DT is set equal to the value of Vx. */
          uint8_t x = (opcode & 0x0F00) >> 8;
          cpu->delay_timer = cpu->V[x];
          increment_pc(cpu, 1);
          break;
        }
        case(0xF018): {
          /* Fx18 - LD ST, Vx */
          /* Set sound timer = Vx. */
          /* ST is set equal to the value of Vx. */
          uint8_t x = (opcode & 0x0F00) >> 8;
          cpu->sound_timer = cpu->V[x];
          increment_pc(cpu, 1);
          break;
        }
        case(0xF01E): {
          /* Fx1E - ADD I, Vx */
          /* Set I = I + Vx. */
          /* The values of I and Vx are added,
             and the results are stored in I. */
          uint8_t x = (opcode & 0x0F00) >> 8;
          cpu->I += x;
          increment_pc(cpu, 1);
          break;
        }
        case(0xF029): {
          /* Fx29 - LD F, Vx */
          /* Set I = location of sprite for digit Vx. */
          /* The value of I is set to the location
             for the hexadecimal sprite corresponding
             to the value of Vx. */
          uint8_t x = (opcode & 0x0F00) >> 8;
          /* cpu->I = (5 * 8) *  (cpu->V[x] - 1); */
          cpu->I = cpu->V[x];
          increment_pc(cpu, 1);
          break;
        }
        case(0xF033): {
          /* Fx33 - LD B, Vx */
          /* Store BCD representation of Vx in memory
             locations I, I+1, and I+2. */
          /* The interpreter takes the decimal value of Vx,
             and places the hundreds digit in memory at
             location in I, the tens digit at location I+1,
             and the ones digit at location I+2. */
          uint8_t x = (opcode & 0x0F00) >> 8;
          uint8_t digits = cpu->V[x];
          cpu->memory[cpu->I]     = (digits % 1000)  / 100;
          cpu->memory[cpu->I + 1] = (digits % 100)   / 10;
          cpu->memory[cpu->I + 2] = (digits % 10)    / 1;
          increment_pc(cpu, 1);
          break;
        }
        case(0xF055): {
          /* Fx55 - LD [I], Vx */
          /* Store registers V0 through Vx in memory
             starting at location I. */

          /* The interpreter copies the values of registers
             V0 through Vx into memory, starting at the
             address in I. */
          uint8_t x = (opcode & 0x0F00) >> 8;
          /* printf("x = %d\n", x); */
          for(int i = 0; i <= x; i++){
            cpu->memory[cpu->I + i] = cpu->V[i];
            /* printf("mem[%d] = %d\n", cpu->I + i, cpu->V[i]); */
          }
          increment_pc(cpu, 1);
          break;
        }
        case(0xF065): {
          /* Fx65 - LD Vx, [I] */
          /* Read registers V0 through Vx from memory
             starting at location I. */
          /* The interpreter reads values from memory
             starting at location I into registers V0
             through Vx. */
          uint8_t x = (opcode & 0x0F00) >> 8;
          for(int i = 0; i <= x; i++){
            cpu->V[i] = cpu->memory[cpu->I + i];
          }
          increment_pc(cpu, 1);
          break;
        }
        }
        break;
      }
    default:
      printf("Opcode not found. Soz mate.\n");
    }
}
