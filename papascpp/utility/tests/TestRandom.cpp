//
//  TestRandom.cpp
//  papas
//
//  Created by Alice Robson on 15/06/16.
//
//

#include "random.h"
#include "gtest/gtest.h"

TEST(Random, random) {

  randomgen::RandUniform runi1{0, 1};
  randomgen::RandUniform runi2{0, 1};
  randomgen::RandExponential rexp1{3};
  randomgen::RandExponential rexp2{4};

  runi1.setSeed(0xdeadbeef);
  int a1 = runi1.next();
  int a2 = runi2.next();
  int a3 = rexp1.next();
  int a4 = rexp2.next();
  runi1.setSeed(0xdeadbeef);
  int b1 = runi1.next();
  int b2 = runi2.next();
  int b3 = rexp1.next();
  int b4 = rexp2.next();
  EXPECT_EQ(a1, b1);
  EXPECT_EQ(a2, b2);
  EXPECT_EQ(a3, b3);
  EXPECT_EQ(a4, b4);

  runi1.setSeed(0xdeadbeef);
  for (int i = 0; i < 6; i++)
    std::cout << runi2.next() << std::endl;

  randomgen::RandExponential rexp{49.3};
  rexp.setSeed(100);
  std::cout << rexp.next() << rexp.next() << std::endl;
  ;
}