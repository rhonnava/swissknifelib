#ifndef INTERNAL_ITERATOR_HPP
#define INTERNAL_ITERATOR_HPP

#define ITERATOR_TYPEDEFS     typedef typename iterator_traits<ContainerIterator>::value_type T; \
    typedef T         value_type; \
    typedef ptrdiff_t  difference_type;\
    typedef T*   pointer; \
    typedef T& reference; \
    typedef random_access_iterator_tag  iterator_category;



#endif

