#include "Reactor.hpp"
#include "internal/defs.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>

Reactor::Reactor() : m_ok_(false)
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
		return true;	
	}

}

Reactor::~Reactor()
{
	if( -1 == close(m_ePollFD))
	{
		//Log an error	
	}
}
