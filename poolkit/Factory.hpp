#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <deque>
#include <utility>
#include <limits>
#include <iterator>
#include <iostream>
#include <functional>
#include "Exception.hpp"
#include "Iterator.hpp"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/mpl/list.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/static_assert.hpp>

using namespace std;

template <typename T, typename Container = deque< pair<T, bool> > >
class Pool
{
public:
typedef internal::Iterator<typename Container::iterator> iterator;
typedef internal::Iterator<typename Container::iterator, internal::UNSAFE> unsafe_iterator; 

private:
#define INIT_POOLSIZE 10
#define MAX_POOLSIZE numeric_limits<size_t>::max()
Container dec;
size_t m_maxSize;
boost::optional<pair<T, bool> > m_default; //Does this need to be optional?
unsafe_iterator next;

inline bool _init(size_t initSize)
{
//We do some random access operation during acquire and release from pool. 
//So we expect the container to give us a random access iterator.
BOOST_STATIC_ASSERT((boost::is_same<typename iterator_traits<typename Container::iterator>::iterator_category, random_access_iterator_tag >::value));

if(!m_default)
	return false;

if(_resize(initSize))
{
	next= unsafe_iterator(dec.begin());
	return true;
}
return false;
}

inline bool _resize(size_t newsize)
{
        if(m_default && newsize < m_maxSize && newsize > dec.size())
        {
               	dec.resize(newsize, *m_default);
		return true;
	}
	return false;
}

public:

Pool(size_t initSize=INIT_POOLSIZE, size_t max_size=MAX_POOLSIZE) : m_maxSize(max_size)
{
	m_default = pair<T, bool>(T(), false);
	_init(initSize);
}

//A1
template <typename A1>
Pool(A1 a1, size_t initSize=INIT_POOLSIZE, size_t max_size=MAX_POOLSIZE) : m_maxSize(max_size)
{
	m_default = pair<T, bool>(T(a1), false);
	_init(initSize);
}

//A2
template <typename A1, typename A2>
Pool(A1 a1, A2 a2, size_t initSize=INIT_POOLSIZE, size_t max_size=MAX_POOLSIZE) : m_maxSize(max_size)
{
	m_default = pair<T, bool>(T(a1, a2), false);
	_init(initSize);
}

//A3
template <typename A1, typename A2, typename A3>
Pool(A1 a1, A2 a2, A3 a3, size_t initSize=INIT_POOLSIZE, size_t max_size=MAX_POOLSIZE) : m_maxSize(max_size)
{
	m_default = pair<T, bool>(T(a1, a2, a3), false);
	_init(initSize);
}

inline size_t size() 
{
	return static_cast<size_t>(dec.size());
}

inline iterator begin()
{
	return iterator(dec.begin());
}

inline iterator end()
{
	return iterator(dec.end());
}

//Does not do a bounds check. Anybody used to STL should be used to this.
inline iterator at(size_t loc) 
{
	return iterator(dec.begin() + loc);
}

inline unsafe_iterator begin_unsafe()
{
	return unsafe_iterator(dec.begin());
}

inline unsafe_iterator end_unsafe()
{
	return unsafe_iterator(dec.end());
}

iterator acquire()
{
	iterator ret = dec.end();
	if(end_unsafe() == next)
	{
		if(dec.size() >= m_maxSize)
		{
			return ret;
		}
		
		size_t newsize = 2*dec.size() + 1;
		if(newsize > m_maxSize)
			newsize =m_maxSize;

		_resize(newsize);
		next = find_if(dec.begin(), dec.end(), !boost::bind(&pair<T, bool>::second, _1)); 
	}
	
	//This one is acquired
	next->second = true;

	return iterator(next++);
}

bool release(iterator param)
{
	if(!m_default)
		return false;

	param.unwrap() = *m_default;
	if(next != begin_unsafe())
	{
		unsafe_iterator top = next;
		//iter_swap(param, top);
	}
	
	return true;
}

void rearrange()
{
	 next = partition(dec.begin(), dec.end(), !boost::bind(&pair<T, bool>::second, _1)); 
}

void reset()
{
	if(!m_default)
		return;

	fill(begin_unsafe(), end_unsafe(), *m_default);
}

};

#endif

