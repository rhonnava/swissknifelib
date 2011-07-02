#include "Reactor.hpp"
#include "internal/defs.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>

using namespace std;

Reactor::Reactor() : m_ok_(false), m_KeepRunning(false)
{
	//Create an epoll fd with FD_CLOSEXEC set on it
	m_ePollFD = epoll_create1(EPOLL_CLOEXEC); 
	if(m_ePollFD > 0)
	{
		m_ok_ = true;
		return;
	}

	switch(errno)
	{
		case EMFILE:
		break;
		case ENFILE:
		break;
		case ENOMEM:
		break;
		default:
		break;
	}
}

Reactor::operator bool()
{
	return m_ok_;
}

bool Reactor::operator!()
{
	return !m_ok_;
}

bool Reactor::registerWatch(int type_, int fd_, const boost::function<void(void)>& fn_, bool edgetriggered_)
{

	struct epoll_event event_;
	event_.events = 0;
	event_.data.fd = fd_;

	//To avoid an invalid input from crashing epoll, sanitizing type_ by bitwise AND with a collection of
	// all possible valid inputs
	type_ & (EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLPRI | EPOLLERR | EPOLLHUP | EPOLLET | EPOLLONESHOT);

	//Set the event type to the bitset
	event_.events | type_;
	if(edgetriggered_)
	{
		if(!(O_NONBLOCK & fcntl(fd_, F_GETFD)))
		{
			//Log here that we need a non blocking file descriptor in case of 
			//an edge triggered watch an return out an error;
			return -1;
		}

		//Add the EPOLLET event to the bitset for edge trigger.
		event_.events | EPOLLET;
		
	}

	if(0 == epoll_ctl(m_ePollFD, EPOLL_CTL_ADD, fd_, &event_))
	{
		m_map.insert(make_pair(fd_, fn_) );
		return true;
	}

	//handle errno here
}

bool Reactor::run()
{
	if(!m_ok_)
		return false;

	m_KeepRunning = true;
	struct epoll_event events_[MAX_EPOLL_WAIT_EVENTS];

	//Fillup a sigset and block all signals until epoll_wait returns
	sigset_t mask_;
	sigfillset(&mask_);

	while(m_KeepRunning)
	{
		int ntriggered = epoll_pwait(m_ePollFD, events_, MAX_EPOLL_WAIT_EVENTS, EPOLL_WAIT_TIMEOUT, &mask_);

		for(int i=0; i<ntriggered; ++i)
		{
			MapIterator itr = m_map.find(events_[i].data.fd);
			if(m_map.end() != itr)
			{
				(itr->second)();
			}
		}
	}
}

void Reactor::stop()
{
	m_KeepRunning = false;
}
Reactor::~Reactor()
{
	if( -1 == close(m_ePollFD))
	{
		//Log an error	
	}
}
