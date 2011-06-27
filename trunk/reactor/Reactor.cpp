#include "Reactor.hpp"
#include "defs.hpp"
#include <unistd.h>
#include <fcntl.h>
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
	}
}

Reactor::operator bool()
{
	return m_ok_;
}

Reactor::operator!()
{
	return !m_ok_;
}

int Reactor::registerWatch(int type_, int fd_, const boost::function<void(void)>& fn_, bool edgetriggered_)
{
	if(edgetriggered_)
	{
		if(!(O_NONBLOCK & fcntl(fd_, F_GETFD)))
		{
			//Log here that we need a non blocking file descriptor in case of 
			//an edge triggered watch an return out an error;
			return -1;
		}
	}	
}

Reactor::~Reactor()
{
	if( -1 == close(m_ePollFD))
	{
		//Log an error	
	}
}
