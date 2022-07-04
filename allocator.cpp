#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "factorial.h"
#include "list.h"
#include "allocator.h"

int main()
{
    try
    {
        std::forward_list<int> list1;
        for (int i = 0; i < 10; ++i)
        {
            list1.push_front(factorial(i));
        }

        MyForwardList<int, std::allocator<int>> list2;
        for (int i = 0; i < 10; ++i)
        {
            list2.push_front(factorial(i));
        }

        list2.print();

        std::map<int, int> map1;
        for (int i = 0; i < 10; ++i)
        {
            map1[i] = factorial(i);
        }

        auto map2 = std::map<
            int,
            int,
            std::less<int>,
            logging_allocator<
                std::pair<const int, int>
            >
        >{};
        
        for (int i = 0; i < 10; ++i)
        {
            map2[i] = factorial(i);
        }

        for (const auto& it : map2)
        {
            std::cout << it.first << " " << it.second << std::endl;
        }

    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
