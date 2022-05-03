#ifndef LIST_H
#define LIST_H

#include <memory>
#include <forward_list>
#include <iostream>


template <class T, class Alloc>
class MyForwardList
{
private:
    struct Node;

public:
    MyForwardList() = default;

    ~MyForwardList()
    {
        while (m_head)
        {
            pop_front();
        }
    }

    T front() const
    {
        return m_head->m_value;
    }

    void push_front(const T& value)
    {
        Node* newNode = Node_traits::allocate(m_alloc, 1);
        Node_traits::construct(m_alloc, newNode, m_head, value);
        m_head = newNode;
        ++m_size;
    }

    void pop_front()
    {
        if (m_head)
        {
            Node* tmpNode = m_head;
            m_head = m_head->m_next;
            Node_traits::destroy(m_alloc, tmpNode);
            Node_traits::deallocate(m_alloc, tmpNode, 1);
            --m_size;
        } 
    }

    std::size_t size() const
    {
        return m_size;
    }

    void print()
    {
        Node* node = m_head;
        std::size_t i = m_size - 1;
        while (node)
        {
            std::cout << i-- << " " << node->m_value << std::endl;
            node = node->m_next;
        }
    }

private:
    struct Node
    {
        Node() : m_next{nullptr}
        {}
        Node(Node* next, const T& value) : m_next{next}, m_value{value}
        {}

        Node* m_next;
        T m_value;
    };

    Node* m_head{nullptr};
    std::size_t m_size{0};

    using Node_alloc = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
    using Node_traits = typename std::allocator_traits<Alloc>::template rebind_traits<Node>;
    Node_alloc m_alloc;
};



#endif
