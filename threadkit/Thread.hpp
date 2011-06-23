#ifndef THREAD_HPP
#define THREAD_HPP
#include <pthread.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>

class Thread
{
bool m_Joinable;
size_t m_StackSize;
pthread_t m_thread;
pthread_attr_t m_attr;
boost::function<void*(void*)> m_fn;

public:
enum{DETACHED = false, JOINABLE = true};


Thread() 
{
	m_fn = NULL;

	//Attr needs to be initialized only once. Do not call again.
	if(pthread_attr_init(&m_attr))
	{
		throw Exception(L"Error while construction thread.");
	}
}

bool setAttributes(bool Joinable=Thread::JOINABLE, size_t StackSize=0)
{
	if(Thread::DETACHED == Joinable)
	{
		if(pthread_attr_setdetachstate(&m_attr, PTHREAD_CREATE_DETACHED))
		{
			return false;
		}
	}

	if(0 != StackSize)
	{
		if(pthread_attr_setstacksize(&m_attr, StackSize))
		{
			return false;
		}
	}

	return true;
}

bool setFunction(const boost::function<void*(void*)>& func)
{
	m_fn = func;
}

//Supports rollback semantics. If call to run was not successful, the original value of m_fn will not be disturbed
// If it is successful, the new value becomes m_fn
template <typename T>
bool run(boost::function<void*(void*)> func, T* param = NULL)
{
	typedef void*(*fn)(void*);

	if(!func)
	{
		return false;
	}
	
	if(pthread_create(&m_thread, &m_attr, *func.target<fn>(), (void*)param))
	{
		return false;
	}

	return true;
}

template <typename T>
bool run(T* param = NULL)
{
	return run(m_fn, param);
}

bool join()
{
	int detachstate;
	if(pthread_attr_getdetachstate(&m_attr, &detachstate))
	{
		return  false;
	}

	if(PTHREAD_CREATE_JOINABLE != detachstate)
	{
		return false;
	}

	if(pthread_join(m_thread, NULL))
	{
		return false;
	}

	return true;
}

virtual ~Thread()
{
}
};

#endif
