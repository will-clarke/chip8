#include <gtest/gtest.h>
#include <limits.h>
#include "../src/cpu.c"
#include "../src/io.c"
#include "../src/io.h"
#include "../src/opcodes.c"
#include "../src/debug.c"

TEST(CpuTests, InitCpu) {
  struct cpu cpu;
  init_cpu(&cpu);
  EXPECT_EQ(cpu.I, 0);
  EXPECT_EQ(cpu.pc, (uint8_t)0x2000);
  EXPECT_EQ(cpu.delay_timer, 0);
  EXPECT_EQ(cpu.sound_timer, 0);
  EXPECT_EQ(*(cpu.stack.stack + 15), 0);
  EXPECT_EQ(*(cpu.V + 15), 0);
  EXPECT_EQ(*(cpu.display), 0);
  EXPECT_EQ(*(cpu.display + 2047), 0);
  EXPECT_EQ(*(cpu.keyboard + 15), 0);
}

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
  EXPECT_EQ(cpu.pc, 1);
  cpu.pc = 0;
  cpu.V[3] = 0x23;
  execute_opcode(0x3323, &cpu);
  EXPECT_EQ(cpu.pc, 2);
}

TEST(OpCodeTest, 0x4000)
{
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.pc = 0;
  execute_opcode(0x4123, &cpu);
  EXPECT_EQ(cpu.pc, 2); // Not equal so jump 2
  cpu.pc = 0;
  cpu.V[1] = 0x23;
  execute_opcode(0x4123, &cpu);
  EXPECT_EQ(cpu.pc, 1); // Equal so jump 1
}

TEST(OpCodeTest, 0x5xy0){
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.pc = 0;
  execute_opcode(0x5120, &cpu);
  // V[1] == v[2]
  EXPECT_EQ(cpu.pc, 2);
  cpu.pc = 0;
  cpu.V[2] = 99;
  execute_opcode(0x5120, &cpu);
  // V[1] != v[2]
  EXPECT_EQ(cpu.pc, 1);
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
  EXPECT_EQ(cpu.pc, 1);
  cpu.V[0x8] = 41;
  cpu.pc = 0;
  execute_opcode(0x9B80, &cpu);
  //not equal so skip pc + 2
  EXPECT_EQ(cpu.pc, 2);
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
 cpu.I = 129;
 cpu.memory[129 + 0] = 'h';
 cpu.memory[129 + 1] = 'e';
 cpu.memory[129 + 2] = 'l';
 cpu.memory[129 + 3] = 'l';
 cpu.memory[129 + 4] = 'o';
 execute_opcode(0xD595, &cpu);

 EXPECT_EQ(cpu.display[5 * 9], 'h');
 // EXPECT_EQ(cpu.display[(5 * DISPLAY_W + 1) + 9], 'e');
 // EXPECT_EQ(cpu.display[(5 * DISPLAY_W + 2) + 9], 'l');
 // EXPECT_EQ(cpu.display[(5 * DISPLAY_W + 2) + 9], 'l');
 // EXPECT_EQ(cpu.display[(5 * DISPLAY_W + 3) + 9], 'o');
}

TEST(OpCodeTest, 0xEx9E){
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.pc = 0;
  execute_opcode(0xE59E, &cpu);
  // Not pressed so don't skip
  EXPECT_EQ(cpu.pc, 1);
  cpu.keyboard[5] = 1;
  cpu.pc = 0;
  execute_opcode(0xE59E, &cpu);
  // Pressed so skip
  EXPECT_EQ(cpu.pc, 2);
}


TEST(OpCodeTest, 0xExA1){
  struct cpu cpu;
  init_cpu(&cpu);
  cpu.pc = 0;
  execute_opcode(0xE5A1, &cpu);
  EXPECT_EQ(cpu.pc, 2);
  cpu.keyboard[0x5] = 1;
  cpu.pc = 0;
  execute_opcode(0xE5A1, &cpu);
  EXPECT_EQ(cpu.pc, 1);
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





TEST(OpCodeTest, 0x00E0)
{
  struct cpu cpu;
  memset(cpu.display, 'z', (int)sizeof(cpu.display));
  execute_opcode(0x00E0, &cpu);
  EXPECT_EQ(*cpu.display, 0);
  EXPECT_EQ(*(cpu.display + (int)sizeof(cpu.display) - 1), 0);
}




TEST(CpuTests, stack_pop){
  struct stack my_stack;
  int my_val;
  for(int i = 0; i < 4; i++)
    my_stack.stack[i] = i * 2;
  my_stack.stack_pointer = 3;
  my_val = stack_pop(&my_stack);
  EXPECT_EQ(my_val, 4);
  my_val = stack_pop(&my_stack);
  EXPECT_EQ(my_val, 2);
  my_val = stack_pop(&my_stack);
  EXPECT_EQ(my_val, 0);
  EXPECT_EQ(my_stack.stack_pointer, 0);
  EXPECT_DEATH({stack_pop(&my_stack);}, "");
}

TEST(CpuTests, stack_push){
  struct stack my_stack;
  int my_val;
  memset(my_stack.stack, 0, sizeof(my_stack.stack));
  my_stack.stack_pointer = 0;
  EXPECT_EQ(my_stack.stack[0], 0);
  my_val = stack_push(&my_stack, 99);
  EXPECT_EQ(my_val, 1);
  EXPECT_EQ(my_stack.stack_pointer, 1);
  EXPECT_EQ(my_stack.stack[0], 99);
  my_val = stack_push(&my_stack, 1);
  EXPECT_EQ(my_stack.stack_pointer, 2);
  EXPECT_EQ(my_stack.stack[1], 1);
  EXPECT_EQ(my_val, 1);
  my_val = stack_push(&my_stack, 152);
  EXPECT_EQ(my_stack.stack_pointer, 3);
  EXPECT_EQ(my_stack.stack[2], 152);
  my_stack.stack_pointer = 14;
  my_val = stack_push(&my_stack, 9);
  EXPECT_EQ(my_stack.stack[14],9);
}

TEST(CpuTests, stack_push_and_pop) {
  struct stack my_stack;
  memset(my_stack.stack, 0, sizeof(my_stack.stack));
  my_stack.stack_pointer = 0;

  stack_push(&my_stack, 4);
  int expected[16] = {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  EXPECT_EQ(my_stack.stack_pointer, 1);
  EXPECT_TRUE(0 == memcmp(my_stack.stack, expected, 16));

  int value = stack_pop(&my_stack);
  EXPECT_EQ(value, 4);

  stack_push(&my_stack, 18);
  stack_push(&my_stack, 9);
  stack_push(&my_stack, 1);
  value = stack_pop(&my_stack);
  EXPECT_EQ(value, 1);
  value = stack_pop(&my_stack);
  EXPECT_EQ(value, 9);
  value = stack_pop(&my_stack);
  EXPECT_EQ(value, 18);
  EXPECT_DEATH({stack_pop(&my_stack);}, "");

  memset(&my_stack.stack, 0, 16);
  my_stack.stack_pointer = 15;
  stack_push(&my_stack, 4);
  int expected2[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4};
  EXPECT_TRUE(0 == memcmp(my_stack.stack, expected2, 16));
  EXPECT_DEATH({stack_push(&my_stack, 96);}, "");
}
