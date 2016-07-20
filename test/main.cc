#include <stdio.h>
#include "io.cc"
#include "cpu.cc"
#include "opcodes.cc"
#include "../src/cpu.c"
#include "../src/io.c"
#include "../src/opcodes.c"
#include "../src/debug.c"

int main(int argc, char *argv[]){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
