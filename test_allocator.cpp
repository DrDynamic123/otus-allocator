#define TEST_MODULE test_allocator

#include <sstream>
#include <list>

#include "gtest/gtest.h"

#include "factorial.h"
#include "list.h"
#include "allocator.h"

TEST(allocator_test_case, factorial_test) {
    EXPECT_EQ(factorial(0), 1);
    EXPECT_EQ(factorial(1), 1);
    EXPECT_EQ(factorial(2), 2);
    EXPECT_EQ(factorial(5), 120);
}

TEST(allocator_test_case, list_test) {
    MyForwardList<int> list;

    for (int i = 0; i < 10; ++i)
    {
        list.push_front(i);
    }

    EXPECT_EQ(list.size(), 10);
    EXPECT_EQ(list.front(), 9);

    list.pop_front();

    EXPECT_EQ(list.size(), 9);
    EXPECT_EQ(list.front(), 8);

    std::stringstream sstream;
    list.print(sstream);
    EXPECT_EQ(sstream.str(), "8 8\n7 7\n6 6\n5 5\n4 4\n3 3\n2 2\n1 1\n0 0\n");
}

TEST(allocator_test_case, allocator_test) {
    std::list<int, logging_allocator<int, 5>> list;
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_NO_THROW(list.push_back(i));
    }
    EXPECT_THROW(list.push_back(1), std::bad_array_new_length);
}
