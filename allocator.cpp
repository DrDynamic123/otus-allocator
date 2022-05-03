#include <iostream>
#include <memory>
#include <vector>
#include <map>
#ifndef __PRETTY_FUNCTION__
#include "pretty.h"
#endif

#include "factorial.h"
#include "list.h"

#define USE_PRETTY 1

template<class T, std::size_t N = 10>
class logging_allocator
{
public:
    using value_type = T;

    logging_allocator()
    {
#ifndef USE_PRETTY
        std::cout << "constructor logging_allocator()" << std::endl;
#else
        std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
        m_ptr = reinterpret_cast<T*>(::operator new(N * sizeof(T)));
    }
    ~logging_allocator()
    {
#ifndef USE_PRETTY
        std::cout << "destructor ~logging_allocator()" << std::endl;
#else
        std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
        ::operator delete(m_ptr);
    }

    template<class U> 
    logging_allocator(const logging_allocator<U, N>&) {
#ifndef USE_PRETTY
        std::cout << "copy_construct logging_allocator(const logging_allocator<U, N>&)" << std::endl;
#else
        std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
        m_ptr = reinterpret_cast<T*>(::operator new(N * sizeof(T)));
    }

    template<class U>
    struct rebind
    {
        using other = logging_allocator<U, N>;
    };

    template<class U, class... Args>
    void construct(U* p, Args&&... args)
    {
#ifndef USE_PRETTY
        std::cout << "construct" << std::endl;
#else
        std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
        ::new((void*)p) U(std::forward<Args>(args)...);
    }

    template<class U>
    void destroy(U* p)
    {
#ifndef USE_PRETTY
        std::cout << "destroy" << std::endl;
#else
        std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
        p->~U();
    }

    T* allocate(std::size_t n)
    {
#ifndef USE_PRETTY
        std::cout << "allocate: [n = " << n << "]" << std::endl;
#else
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
#endif
        if (n > (std::numeric_limits<std::size_t>::max() / sizeof(T)) || ((m_n + n) > N))
            throw std::bad_array_new_length();

        auto shift = m_n;
        m_n += n;
        return m_ptr + shift;
    }

    void deallocate(T* p, std::size_t n)
    {
#ifndef USE_PRETTY
        std::cout << "deallocate: [n = " << n << "]" << std::endl;
#else
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
#endif
        (void) p;
        m_n -= n;
    }

private:
    T* m_ptr{nullptr};
    std::size_t m_n{0};
};

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
