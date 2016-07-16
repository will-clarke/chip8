#include <gtest/gtest.h>
#include "../src/cpu.c"

TEST(CpuTests, InitCpu)
{
  struct cpu cpu;
  init_cpu(&cpu);
  EXPECT_EQ(cpu.I, 0);
  EXPECT_EQ(cpu.pc, (uint8_t)0x2000);
  EXPECT_EQ(cpu.delay_timer, 0);
  EXPECT_EQ(cpu.sound_timer, 0);
  EXPECT_EQ(*(cpu.stack.stack + 15), 0);
  EXPECT_EQ(*(cpu.V + 15), 0);
  EXPECT_EQ(*(cpu.graphics), 0);
  EXPECT_EQ(*(cpu.graphics + 2047), 0);
  EXPECT_EQ(*(cpu.keyboard + 15), 0);
}

TEST(CpuTests, 0x00E0)
{
  struct cpu cpu;
  // init_cpu(&cpu);
  // EXPECT_EQ(*cpu.graphics, 0);
  // EXPECT_EQ(*(cpu.graphics + (int)sizeof(cpu.graphics) - 1), 0);
  memset(cpu.graphics, 'z', (int)sizeof(cpu.graphics));
  // EXPECT_EQ(*cpu.graphics, 'z');
  // EXPECT_EQ(*(cpu.graphics + (int)sizeof(cpu.graphics) - 1), 'z');
  execute_opcode(0x00E0, &cpu);
  EXPECT_EQ(*cpu.graphics, 0);
  EXPECT_EQ(*(cpu.graphics + (int)sizeof(cpu.graphics) - 1), 0);
}

TEST(CpuTests, stack_pop){
  struct stack my_stack;
  int my_val;
  for(int i = 0; i < 4; i++)
    my_stack.stack[i] = i * 2;
  my_stack.stack_pointer = 3;
  EXPECT_EQ(my_stack.stack[0], 0);
  EXPECT_EQ(my_stack.stack[1], 2);
  EXPECT_EQ(my_stack.stack[2], 4);
  EXPECT_EQ(my_stack.stack[3], 6);
  EXPECT_NE(my_stack.stack[4], 8);
  my_val = stack_pop(&my_stack);
  EXPECT_EQ(my_val, 6);
  EXPECT_EQ(my_stack.stack_pointer, 2);
  my_val = stack_pop(&my_stack);
  EXPECT_EQ(my_val, 4);
  EXPECT_EQ(my_stack.stack_pointer, 1);
  my_val = stack_pop(&my_stack);
  EXPECT_EQ(my_val, 2);
  EXPECT_EQ(my_stack.stack_pointer, 0);
  my_val = stack_pop(&my_stack);
  EXPECT_EQ(my_val, 0);


  // should fail. Perhaps catch the exception or something??
  my_val = stack_pop(&my_stack);
  EXPECT_EQ(my_val, -1);
  EXPECT_EQ(my_stack.stack_pointer, -1);
}
