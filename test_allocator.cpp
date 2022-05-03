#define TEST_MODULE test_allocator

#include "gtest/gtest.h"

#include "factorial.h"

TEST(allocator_test_case, factorial_test) {
    EXPECT_EQ(factorial(0), 1);
    EXPECT_EQ(factorial(1), 1);
    EXPECT_EQ(factorial(2), 2);
    EXPECT_EQ(factorial(5), 120);
}
