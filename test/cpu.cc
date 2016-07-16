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
  EXPECT_EQ(*(cpu.stack + 15), 0);
  EXPECT_EQ(*(cpu.V + 15), 0);
  EXPECT_EQ(*(cpu.graphics), 0);
  EXPECT_EQ(*(cpu.graphics + 2047), 0);
  EXPECT_EQ(*(cpu.keyboard + 15), 0);
}

TEST(CpuTests, execute_opcode)
{
  struct cpu cpu;
  init_cpu(&cpu);
  EXPECT_EQ(*cpu.graphics, 0);
  for(int i = 0; i < (int)sizeof(cpu.graphics); i++)
    *(cpu.graphics + i) = 'z';
  EXPECT_EQ(*cpu.graphics, 'z');
  execute_opcode(0x00E0, &cpu);
  EXPECT_EQ(*cpu.graphics, 0);

  // printf("OMGOMGOMG v: %d\n", sizeof(cpu.V));
  // printf("OMGOMGOMG key: %d\n", sizeof(cpu.keyboard));
  // printf("OMGOMGOMG graphics: %d\n", sizeof(cpu.graphics));
}
