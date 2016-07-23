#include <gtest/gtest.h>
#include <limits.h>
// #include "../src/cpu.c"
// #include "../src/io.c"
#include "../src/io.h"
// #include "../src/opcodes.c"
// #include "../src/debug.c"

#include "../src/cpu.h"
#include "../src/io.h"
#include "../src/opcodes.h"
#include "../src/debug.h"

TEST(OpCodeTest, 0x00EE){
  /* Return from a subroutine. */
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.stack.stack_pointer = 5;
  cpu.stack.stack[4] = 2;
  execute_opcode(0x00EE, &cpu);
  EXPECT_EQ(cpu.stack.stack_pointer, 4);
  EXPECT_EQ(cpu.pc, 2);
}

TEST(OpCodeTest, 0x1000)
{
  struct cpu cpu;
  init_cpu(&cpu);
  execute_opcode(0x1987, &cpu);
  EXPECT_EQ(cpu.pc, 0x987);
}

TEST(OpCodeTest, 0x2000)
{
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.pc = 42;
  execute_opcode(0x2123, &cpu);
  EXPECT_EQ(cpu.stack.stack_pointer, 1);
  EXPECT_EQ(cpu.stack.stack[0], 42);
  EXPECT_EQ(cpu.pc, 0x123);
}

TEST(OpCodeTest, 0x3000)
{
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.pc = 0;
  execute_opcode(0x3323, &cpu);
  EXPECT_EQ(cpu.pc, 2);
  cpu.pc = 0;
  cpu.V[3] = 0x23;
  execute_opcode(0x3323, &cpu);
  EXPECT_EQ(cpu.pc, 4);
}

TEST(OpCodeTest, 0x4000)
{
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.pc = 0;
  execute_opcode(0x4123, &cpu);
  EXPECT_EQ(cpu.pc, 4); // Not equal so jump 2
  cpu.pc = 0;
  cpu.V[1] = 0x23;
  execute_opcode(0x4123, &cpu);
  EXPECT_EQ(cpu.pc, 2); // Equal so jump 1
}

TEST(OpCodeTest, 0x5xy0){
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.pc = 0;
  execute_opcode(0x5120, &cpu);
  // V[1] == v[2]
  EXPECT_EQ(cpu.pc, 4);
  cpu.pc = 0;
  cpu.V[2] = 99;
  execute_opcode(0x5120, &cpu);
  // V[1] != v[2]
  EXPECT_EQ(cpu.pc, 2);
}

TEST(OpCodeTest, 0x6xkk){
  struct cpu cpu;
  init_cpu(&cpu);
  execute_opcode(0x6720, &cpu);
  EXPECT_EQ(cpu.V[7], 0x20);
}

TEST(OpCodeTest, 0x7xkk){
  struct cpu cpu;
  init_cpu(&cpu);
  execute_opcode(0x7720, &cpu);
  EXPECT_EQ(cpu.V[7], 0x20);
  execute_opcode(0x7701, &cpu);
  EXPECT_EQ(cpu.V[7], 0x21);
}

TEST(OpCodeTest, 0x8xy0){
  struct cpu cpu;
  init_cpu(&cpu);
  execute_opcode(0x8B80, &cpu);
  EXPECT_EQ(cpu.V[0xB], cpu.V[8]);
  cpu.V[8] = 42;
  execute_opcode(0x8B80, &cpu);
  EXPECT_EQ(cpu.V[0xB], cpu.V[8]);
}

TEST(OpCodeTest, 0x8xy1){
  // AND
  struct cpu cpu;
  init_cpu(&cpu);
  execute_opcode(0x8B81, &cpu);
  EXPECT_EQ(cpu.V[0xB], cpu.V[8]);
  cpu.V[8] = 0b011001;
  execute_opcode(0x8B81, &cpu);
  EXPECT_EQ(cpu.V[0xB], cpu.V[8]);
  cpu.V[8]    =  0b011001;
  cpu.V[0xB]  =  0b101001;
  int expected = 0b111001;
  execute_opcode(0x8B81, &cpu);
  EXPECT_EQ(cpu.V[0xB], expected);
}

TEST(OpCodeTest, 0x8xy2){
  // OR
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.V[0xB] = 0;
  cpu.V[8] = 0b011001;
  execute_opcode(0x8B82, &cpu);
  EXPECT_EQ(cpu.V[0xB], 0);
  cpu.V[0xB] = 0b011001;
  cpu.V[8] = 0;
  execute_opcode(0x8B82, &cpu);
  EXPECT_EQ(cpu.V[0xB], 0);
  cpu.V[8]    =  0b011001;
  cpu.V[0xB]  =  0b101001;
  int expected = 0b001001;
  execute_opcode(0x8B82, &cpu);
  EXPECT_EQ(cpu.V[0xB], expected);
}

TEST(OpCodeTest, 0x8xy3){
  // XOR
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.V[0xB] = 0b011001;
  cpu.V[8]   = 0b011001;
  execute_opcode(0x8B83, &cpu);
  EXPECT_EQ(cpu.V[0xB], 0);
  cpu.V[0xB] = 0b110001;
  cpu.V[8]   = 0b011001;
  execute_opcode(0x8B83, &cpu);
  EXPECT_EQ(cpu.V[0xB], 0b101000);
}

TEST(OpCodeTest, 0x8xy4){
  // ADD
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.V[0xB] = 5;
  cpu.V[8]   = 5;
  execute_opcode(0x8B84, &cpu);
  EXPECT_EQ(cpu.V[0xB], 10);
  cpu.V[0xB] = 15;
  cpu.V[8]   = 30;
  execute_opcode(0x8B84, &cpu);
  EXPECT_EQ(cpu.V[0xB], 45);
}

TEST(OpCodeTest, 0x8xy5){
  // SUB
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.V[0xB] = 50;
  cpu.V[8]   = 5;
  execute_opcode(0x8B85, &cpu);
  EXPECT_EQ(cpu.V[0xB], 45);
  cpu.V[0xB] = 20;
  cpu.V[8]   = 10;
  execute_opcode(0x8B85, &cpu);
  EXPECT_EQ(cpu.V[0xB], 10);
}

TEST(OpCodeTest, 0x8xy6){
  // SHR
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.V[0xB] = 0b101;
  execute_opcode(0x8B86, &cpu);
  EXPECT_EQ(cpu.V[0xF], 1);
  EXPECT_EQ(cpu.V[0xB], 0b10);
  cpu.V[0xB] = 0b1110;
  execute_opcode(0x8B86, &cpu);
  EXPECT_EQ(cpu.V[0xF], 0);
  EXPECT_EQ(cpu.V[0xB], 0b111);
}

TEST(OpCodeTest, 0x8xy7){
  // SUBN
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.V[0xB] = 0b11;
  cpu.V[0x8] = 0b01;
  execute_opcode(0x8B87, &cpu);
  EXPECT_EQ(cpu.V[0xB], UCHAR_MAX - 1);
  EXPECT_EQ(cpu.V[0xF], 0);
  cpu.V[0xB] = 0b01;
  cpu.V[0x8] = 0b11;
  execute_opcode(0x8B87, &cpu);
  EXPECT_EQ(cpu.V[0xB], 0b10);
  EXPECT_EQ(cpu.V[0xF], 1);
}

TEST(OpCodeTest, 0x8xyE){
//   // SHL
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.V[0xB] = 0b1010;
  execute_opcode(0x8B8E, &cpu);
  EXPECT_EQ(cpu.V[0xF], 1);
  EXPECT_EQ(cpu.V[0xB], 0b10100 & 0xF);
  cpu.V[0xB] = 0b10;
  execute_opcode(0x8B8E, &cpu);
  EXPECT_EQ(cpu.V[0xF], 0);
  EXPECT_EQ(cpu.V[0xB], 0b100);
}

TEST(OpCodeTest, 0x9xy0){
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.V[0xB] = 42;
  cpu.V[0x8] = 42;
  cpu.pc = 0;
  execute_opcode(0x9B80, &cpu);
  //equal so don't skip
  EXPECT_EQ(cpu.pc, 2);
  cpu.V[0x8] = 41;
  cpu.pc = 0;
  execute_opcode(0x9B80, &cpu);
  //not equal so skip pc + 2
  EXPECT_EQ(cpu.pc, 4);
}

TEST(OpCodeTest, 0xAnnn){
  struct cpu cpu;
  init_cpu(&cpu);
  execute_opcode(0xA042, &cpu);
  EXPECT_EQ(cpu.I, 0x42);
}

TEST(OpCodeTest, 0xBnnn){
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.V[0] = 2;
  execute_opcode(0xB040, &cpu);
  EXPECT_EQ(cpu.pc, 0x42);
}

TEST(OpCodeTest, 0xCxkk){
  struct cpu cpu;
  init_cpu(&cpu);
  execute_opcode(0xCA42, &cpu);
  // HOW TO STUB RAND() IN C ??
  // No idea....
  // So I'm not going to test this.
}

// DISPLAY
TEST(OpCodeTest, 0xDxyn){
struct cpu cpu;
 init_cpu(&cpu);
 cpu.I = 0;
 cpu.V[0x3] = 0;
 cpu.V[0x4] = 0;
 cpu.I = 0;
 cpu.memory[0] = 'h';
 cpu.memory[1] = 'e';
 cpu.memory[2] = 'y';
 cpu.memory[3] = '!';
 cpu.memory[4] = 'g';
 cpu.memory[5] = 'u';
 cpu.memory[6] = 'y';
 cpu.memory[7] = 'z';
 cpu.memory[8] = '!';
 cpu.memory[9] = 'Z';
 execute_opcode(0xD34A, &cpu);
 EXPECT_EQ(cpu.display[0 * DISPLAY_W], 'h');
 EXPECT_EQ(cpu.display[1 * DISPLAY_W], 'e');
 EXPECT_EQ(cpu.display[2 * DISPLAY_W], 'y');
 EXPECT_EQ(cpu.display[3 * DISPLAY_W], '!');
 EXPECT_EQ(cpu.display[4 * DISPLAY_W], 'g');
 EXPECT_EQ(cpu.display[5 * DISPLAY_W], 'u');
 EXPECT_EQ(cpu.display[6 * DISPLAY_W], 'y');
 EXPECT_EQ(cpu.display[7 * DISPLAY_W], 'z');
 EXPECT_EQ(cpu.display[8 * DISPLAY_W], '!');
 EXPECT_EQ(cpu.display[9 * DISPLAY_W], 'Z');
 EXPECT_EQ(cpu.V[0xF], 0);

 // testing xoring && v[0xf]
 init_cpu(&cpu);
 cpu.I = 242;
 cpu.V[0xD] = 0xD; //W
 cpu.V[7] = 2;     // H
 cpu.memory[242] = 'T';
 cpu.memory[243] = 'o';
 cpu.memory[244] = 's';
 cpu.memory[245] = 'k';
 cpu.display[141 + (DISPLAY_W * 1)] = 0b00010000;
 execute_opcode(0xDD74,&cpu);
 EXPECT_EQ(cpu.display[141], 'T');
 EXPECT_EQ(cpu.display[141 + (DISPLAY_W * 1)], 127);
 EXPECT_EQ(cpu.display[141 + (DISPLAY_W * 2)], 's');
 EXPECT_EQ(cpu.display[141 + (DISPLAY_W * 3)], 'k');
 EXPECT_EQ(cpu.V[0xF], 1);

 // testing wrapping
 init_cpu(&cpu);
 cpu.I = 87;
 cpu.V[0xD] = DISPLAY_W - 4;
 cpu.V[7] = DISPLAY_H - 2;
 uint16_t total = DISPLAY_W * DISPLAY_H;
 cpu.memory[87] = 'T';
 cpu.memory[88] = 'o';
 cpu.memory[89] = 's';
 cpu.memory[90] = 'k';
 cpu.memory[91] = '!';
 execute_opcode(0xDD75,&cpu);
 EXPECT_EQ(cpu.display[total - (1 * DISPLAY_W) - 4], 'T');
 EXPECT_EQ(cpu.display[total + (0 * DISPLAY_W) - 4], 'o');
 EXPECT_EQ(cpu.display[0 + (1 * DISPLAY_W) - 4], 's');
 EXPECT_EQ(cpu.display[0 + (2 * DISPLAY_W) - 4], 'k');
 EXPECT_EQ(cpu.V[0xF], 0);
}

TEST(OpCodeTest, 0xEx9E){
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.pc = 0;
  execute_opcode(0xE59E, &cpu);
  // Not pressed so don't skip
  EXPECT_EQ(cpu.pc, 2);
  cpu.keyboard[5] = 1;
  cpu.pc = 0;
  execute_opcode(0xE59E, &cpu);
  // Pressed so skip
  EXPECT_EQ(cpu.pc, 4);
}


TEST(OpCodeTest, 0xExA1){
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.pc = 0;
  execute_opcode(0xE5A1, &cpu);
  EXPECT_EQ(cpu.pc, 4);
  cpu.keyboard[0x5] = 1;
  cpu.pc = 0;
  execute_opcode(0xE5A1, &cpu);
  EXPECT_EQ(cpu.pc, 2);
}

TEST(OpCodeTest, 0xFx07){
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.delay_timer = 42;
  execute_opcode(0xF907, &cpu);
  EXPECT_EQ(cpu.V[9], 42);
  cpu.delay_timer = 69;
  execute_opcode(0xFC07, &cpu);
  EXPECT_EQ(cpu.V[0xC], 69);
}

TEST(OpCodeTest, 0xFx0A){
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.keyboard[0xB] = 0x01;
  execute_opcode(0xF20A, &cpu);
  EXPECT_EQ(cpu.V[2], 0xB);
  cpu.keyboard[0xB] = 0;
  cpu.keyboard[4] = 1;
  execute_opcode(0xF20A, &cpu);
  EXPECT_EQ(cpu.V[2], 4);
}

TEST(OpCodeTest, 0xFx15){
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.V[0xA] = 42;
  execute_opcode(0xFA15, &cpu);
  EXPECT_EQ(cpu.delay_timer, 42);
  cpu.V[1] = 123;
  execute_opcode(0xF115, &cpu);
  EXPECT_EQ(cpu.delay_timer, 123);
}

TEST(OpCodeTest, 0xFx18){
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.V[0xA] = 42;
  execute_opcode(0xFA18, &cpu);
  EXPECT_EQ(cpu.sound_timer, 42);
  cpu.V[1] = 123;
  execute_opcode(0xF118, &cpu);
  EXPECT_EQ(cpu.sound_timer, 123);
}

TEST(OpCodeTest, 0xFx1E){
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.I = 1000;
  cpu.V[8] = 8;
  execute_opcode(0xF81E, &cpu);
  EXPECT_EQ(cpu.I, 1008);
  execute_opcode(0xF81E, &cpu);
  EXPECT_EQ(cpu.I, 1016);
}

TEST(OpCodeTest, 0xFx29){
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.V[8] = 0xA;
  execute_opcode(0xF829, &cpu);
  EXPECT_EQ(cpu.memory[5 * 10 + 0], 0xF0);
  EXPECT_EQ(cpu.memory[5 * 10 + 1], 0x90);
  EXPECT_EQ(cpu.memory[5 * 10 + 2], 0xF0);
  EXPECT_EQ(cpu.memory[5 * 10 + 3], 0x90);
  EXPECT_EQ(cpu.memory[5 * 10 + 4], 0x90);
    // 0xF0, 0x90, 0xF0, 0x90, 0x90, //A
}


TEST(OpCodeTest, 0xFx33){
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.V[4] = 250;
  cpu.I = 15;
  execute_opcode(0xF433, &cpu);
  EXPECT_EQ(cpu.memory[15], 2);
  EXPECT_EQ(cpu.memory[16], 5);
  EXPECT_EQ(cpu.memory[17], 0);
  cpu.V[9] = 123;
  cpu.I = 93;
  execute_opcode(0xF933, &cpu);
  EXPECT_EQ(cpu.memory[93], 1);
  EXPECT_EQ(cpu.memory[94], 2);
  EXPECT_EQ(cpu.memory[95], 3);
}

TEST(OpCodeTest, 0xFx55){
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.V[0] = 00;
  cpu.V[1] = 10;
  cpu.V[2] = 20;
  cpu.V[3] = 30;
  cpu.V[4] = 40;
  cpu.I = 129;
  execute_opcode(0xF455, &cpu);
  EXPECT_EQ(cpu.memory[129 + 0], 00);
  EXPECT_EQ(cpu.memory[129 + 1], 10);
  EXPECT_EQ(cpu.memory[129 + 2], 20);
  EXPECT_EQ(cpu.memory[129 + 3], 30);
  EXPECT_EQ(cpu.memory[129 + 4], 40);
}

TEST(OpCodeTest, 0xFx65){
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.memory[129 + 0] = 'h';
  cpu.memory[129 + 1] = 'e';
  cpu.memory[129 + 2] = 'y';
  cpu.memory[129 + 3] = '!';
  cpu.I = 129;
  execute_opcode(0xF365, &cpu);
  EXPECT_EQ(cpu.V[0], 'h');
  EXPECT_EQ(cpu.V[1], 'e');
  EXPECT_EQ(cpu.V[2], 'y');
  EXPECT_EQ(cpu.V[3], '!');
}

// Display: shouldn't mess with our display memory
// TEST(OpCodeTest, 0x00E0)
// {
//   struct cpu cpu;
//   memset(cpu.display, 'z', (int)sizeof(cpu.display));
//   execute_opcode(0x00E0, &cpu);
//   EXPECT_EQ(*cpu.display, 0);
//   EXPECT_EQ(*(cpu.display + (int)sizeof(cpu.display) - 1), 0);
// }
