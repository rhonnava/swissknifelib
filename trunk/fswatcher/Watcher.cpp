#include "Watcher.hpp"
#include <sys/select.h>
#include <sys/time.h>

using namespace std;

Watcher::Watcher(bool blocking) : m_inotifyFD(0), m_KeepRunning(0) 
{
	m_inotifyFD = inotify_init1(O_NONBLOCK);

	//Throwing in the constructor is not the best of things to do, but
	//if you are not able to get a handle to inotify there is nothing you
	//can do with a Watcher object.
	if (-1 == m_inotifyFD || 0 == m_inotifyFD)
	{
		throw Exception(L"Unable to initialize Watcher"); //Add strerror once Exception class is ready
	}
}

//Register watch type from one of those declared in Watcher.
int Watcher::registerWatch(watchtype_t type, const string& watch_, const boost::function<void(void)>& handler_)
{
	int watchFD = inotify_add_watch(m_inotifyFD, watch_.c_str(), type); 

	//Log an error here when the logging toolkit is done.
	if(-1 == watchFD)
		return watchFD;

	m_watchMap.insert(make_pair(watchFD, handler_));
	return watchFD;
}

bool Watcher::removeWatch(int wd)
{
	inotify_rm_watch(m_inotifyFD, wd);			
}

bool Watcher::run()
{
	if(!m_inotifyFD)
		return false;

	//Right now we are going to select only on one file descriptor which is the inotify fd
	//But still prefered select over read.
	struct timeval time;
	fd_set readfds;
	int ret;

	//Select once in 5 seconds for new events
	time.tv_sec = 5;
	time.tv_usec = 0;

	//REset all fds in the fd set and set the inotify fd
	FD_ZERO (&readfds);
	FD_SET (m_inotifyFD, &readfds);
	
	m_KeepRunning = 1;
	while(m_KeepRunning)
	{
		int ret = select(m_inotifyFD+1 , &readfds, NULL, NULL, &time);

		if(0 == ret)
		{
			continue;
		}

		if(-1 == ret)
		{
			switch(errno)
			{
				case EINTR:
				break;
				case EBADF: //Fall through
				case EINVAL:
				case ENOMEM:
				default:
				m_KeepRunning = 0;
			}
		}
	}
}

void Watcher::stop()
{
	m_KeepRunning = 0;
}
Watcher::~Watcher()
{
	close(m_inotifyFD);	
}
