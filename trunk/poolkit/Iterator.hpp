#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iostream>
#include <iterator>
#include "internal/Iterator.hpp"

using namespace internal;

enum {SAFE=true, UNSAFE=false};

//The default version
template <typename ContainerIterator, bool bparam = true>
class Iterator{
public:
Iterator(){}
};

//The safe version
template<typename ContainerIterator>
class Iterator<ContainerIterator, true> 
{

ContainerIterator m_itr;
friend class Iterator<ContainerIterator, false>;
public:

//Copy constructor that takes in the unsafe version
Iterator(const Iterator& other)
{
	m_itr = other.m_itr;	
}

bool operator !=(const Iterator& other)
{
	return (m_itr != other.m_itr);
}

CONSTRUCTORS

ITERATOR_TYPEDEFS

OPERATOR_ARROW

UNWRAP_FUNC

OPERATOR_COMPARISON
};

//The unsafe version
template<typename ContainerIterator> 
class Iterator<ContainerIterator, false> 
{
ContainerIterator m_itr;
friend class Iterator<ContainerIterator, true>;
public:
ITERATOR_TYPEDEFS

CONSTRUCTORS

OPERATOR_ARROW

UNWRAP_FUNC

OPERATOR_COMPARISON

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

};
#endif
