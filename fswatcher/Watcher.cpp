#include "Watcher.hpp"

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
	
	m_KeepRunning = 1;
	while(m_KeepRunning)
	{
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
