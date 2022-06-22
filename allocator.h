#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <memory>

#ifndef __PRETTY_FUNCTION__
#include "pretty.h"
#endif

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

        if (!m_ptr)
            m_ptr = reinterpret_cast<T*>(::operator new(N * sizeof(T)));

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

#endif // ALLOCATOR_H
