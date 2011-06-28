#ifndef _REACTOR_HPP_
#define _REACTOR_HPP_
#include <boost/utility.hpp>
#include <boost/function.hpp>

class Reactor : private boost::noncopyable
{
bool m_ok_;
int m_ePollFD;
public:
Reactor(); 

//Providing a safe bool to check if reactor got constructed properly. 
//If there was a problem in constructing the Reactor, we do not throw an exception.
//Instead we set a flag.
operator bool();

bool operator!();

//Registers a handler to watch fd_. It is necessary to have a non bocking fd if we need the watch to be 
//edge triggered.
bool registerWatch(int type_, int fd_, const boost::function<void(void)>& fn_, bool edgetriggered_=false);

int modifyWatch();

int removeWatch();

~Reactor();
};
#endif
