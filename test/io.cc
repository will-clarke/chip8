#include <gtest/gtest.h>
#include "../src/io.h"

TEST(IOTests, IO)
{
  struct cpu cpu;
  init_cpu(&cpu);
  char *argv[] = {(char*)"./chip8", (char*)"../roms/PONG"};
  load_rom(&cpu, 2, argv);
  EXPECT_EQ(cpu.memory[0x200 + 0], 0x6A );
  EXPECT_EQ(cpu.memory[0x200 + 1], 0x02 );
  EXPECT_EQ(cpu.memory[0x200 + 2], 0x6B );
  EXPECT_EQ(cpu.memory[0x200 + 3], 0x0C );
  EXPECT_EQ(cpu.memory[0x200 + 0xE0], 0xD4);
  EXPECT_EQ(cpu.memory[0x200 + 0xE1], 0x55 );
  EXPECT_EQ(cpu.memory[0x200 + 0xE2], 0x74 );
  EXPECT_EQ(cpu.memory[0x200 + 0xE3], 0x15 );
  EXPECT_EQ(cpu.memory[0x200 + 0xE4], 0xF2 );
  EXPECT_EQ(cpu.memory[0x200 + 0xE5], 0x29 );
  EXPECT_EQ(cpu.memory[0x200 + 0xEE], 0x80 );
}
