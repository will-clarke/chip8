#include <stdio.h>
#include "io.cc"
#include "cpu.cc"

int main(int argc, char *argv[]){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
