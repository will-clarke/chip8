#include <gtest/gtest.h>

TEST(Something, ATestName)
{
  EXPECT_EQ(1, 2);
  EXPECT_EQ(1, 1);
  EXPECT_EQ(1, 8);
}
int main(int argc, char *argv[]){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
