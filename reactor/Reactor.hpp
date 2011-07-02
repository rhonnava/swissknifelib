#ifndef _REACTOR_HPP_
#define _REACTOR_HPP_
#include <map>
#include <pthread.h>
#include <signal.h>
#include <boost/utility.hpp>
#include <boost/function.hpp>

class Reactor : private boost::noncopyable
{
bool m_ok_;
int m_ePollFD;
std::map<int, boost::function<void(void)> > m_map;
typedef std::map<int, boost::function<void(void)> >::iterator MapIterator;
sig_atomic_t m_KeepRunning;
public:
Reactor(); 

//Providing a safe bool to check if reactor got constructed properly. 
//If there was a problem in constructing the Reactor, we do not throw an exception.
//Instead we set a flag.
operator bool();

bool operator!();

//Registers a handler to watch fd_. It is necessary to have a non bocking fd if we need the watch to be 
//edge triggered. Edge triggered requires the registered handler to read/write until EAGAIN and reregister
// An event driven state machine should store the state after EAGAIN and then reregister.
bool registerWatch(int type_, int fd_, const boost::function<void(void)>& fn_, bool edgetriggered_=false);

int modifyWatch();

int removeWatch();

//Blocks this thread, so this needs to be called in a separate thread
bool run();

//This is thread safe and re-entrant, so can be called from a signal handler in the client code
//(probably to stop a daemon)
inline void stop();

~Reactor();
};
#endif
