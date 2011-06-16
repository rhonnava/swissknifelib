#ifndef INTERNAL_ITERATOR_HPP
#define INTERNAL_ITERATOR_HPP

namespace internal
{

#define ITERATOR_TYPEDEFS     typedef typename iterator_traits<ContainerIterator>::value_type T; \
    typedef T         value_type; \
    typedef ptrdiff_t  difference_type;\
    typedef T*   pointer; \
    typedef T& reference; \
    typedef random_access_iterator_tag  iterator_category;


#define OPERATOR_ARROW T* operator->() \
{ \
        return &(*m_itr); \
}

#define UNWRAP_FUNC ContainerIterator unwrap() \
{ \
        return m_itr; \
}

#define OPERATOR_COMPARISON bool operator==(const Iterator& other) \
{ \
        return (m_itr == other.m_itr); \
}

#define CONSTRUCTORS Iterator(){} \
 \
Iterator(ContainerIterator itr) : m_itr(itr) \
{ \
} \

} //namespace internal

#endif

