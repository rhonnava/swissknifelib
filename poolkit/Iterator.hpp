#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iostream>
#include <iterator>

namespace internal
{

enum {SAFE=true, UNSAFE=false};

//What I am doing is a very bad idea theoritically as an iterator is not a kind of Common. 
// But there is a lot of commonality between the safe and unsafe iterators. So there is a 
// lot of code repetition. So, doing a public inheritance. Please suggest if you have a 
// better idea.FYI, I want to keep the the template specialization as Iterator is the most 
// legible name and also want to challenge the always never rule.
template <typename ContainerIterator>
struct Common
{
    typedef typename iterator_traits<ContainerIterator>::value_type T; 
    typedef T         value_type;
    typedef ptrdiff_t  difference_type;
    typedef T*   pointer;
    typedef T& reference;
    typedef random_access_iterator_tag  iterator_category;

};

//The default version
template <typename ContainerIterator, bool bparam = true>
class Iterator{
public:
Iterator(){}
};

//The safe version
template<typename ContainerIterator>
class Iterator<ContainerIterator, true> : public Common<ContainerIterator>
{
typedef typename iterator_traits<ContainerIterator>::value_type T;
ContainerIterator m_itr;
friend class Iterator<ContainerIterator, false>;
public:
Iterator(){}
Iterator(ContainerIterator itr) : m_itr(itr)
{
}

//Copy constructor that takes in the unsafe version
Iterator(const Iterator<ContainerIterator, false>& other)
{
	m_itr = other.m_itr;	
}

bool operator==(const Iterator& other)
{
	return (m_itr == other.m_itr);
}

bool operator !=(const Iterator& other)
{
	return (m_itr != other.m_itr);
}

T* operator->()
{
	return &(*m_itr);
}

ContainerIterator unwrap()
{
	return m_itr;
}
};

//The unsafe version
template<typename ContainerIterator> 
class Iterator<ContainerIterator, false> : public Common<ContainerIterator>
{
typedef typename iterator_traits<ContainerIterator>::value_type T;
ContainerIterator m_itr;
friend class Iterator<ContainerIterator, true>;
public:
Iterator(){}
T operator*() 
{
        return *m_itr;
}

Iterator<ContainerIterator, false> operator++(int)
{
        Iterator<ContainerIterator, false> tempItr = *this;
        ++m_itr;
        return tempItr;
}

const Iterator<ContainerIterator, false>& operator++()
{
        ++m_itr;
        return *this;
}

Iterator<ContainerIterator, false> operator--(int)
{
	Iterator<ContainerIterator, false> tempItr;
	--m_itr;
	return *this;
}

const Iterator<ContainerIterator, false>& operator--()
{
}

bool operator!=(const Iterator<ContainerIterator, false>& other)
{
	return (m_itr != other.m_itr);
}
Iterator(ContainerIterator itr) : m_itr(itr)
{
}

const Iterator<ContainerIterator, false>& operator=(const Iterator<ContainerIterator, false>& other)
{
	if(this == &other)
		return *this;


	m_itr = other.m_itr;
	return *this;
}

const Iterator<ContainerIterator, false>& operator=(const ContainerIterator& other)
{
	if(&(this->m_itr) == &other)
		return *this;

        m_itr = other;
        return *this;
}

bool operator==(const Iterator& other)
{
	return (m_itr == other.m_itr);
}

T* operator->()
{
	return &(*m_itr);
}

ContainerIterator unwrap()
{
	return m_itr;
}

};
}//namespace internal
#endif
