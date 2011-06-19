#include "Watcher.hpp"

using namespace std;

Watcher::Watcher(bool blocking) : m_inotifyFD(0) 
{
	m_inotifyFD = inotify_init1(O_NONBLOCK);

	if (-1 == m_inotifyFD)
	{
		throw Exception(L"Unable to initialize Watcher"); //Add strerror once Exception class is ready
	}
}

//Register watch type from one of those declared in Watcher.
bool Watcher::registerWatch(watchtype_t type, const string& watch_, const boost::function<void(void)>& handler_)
{
	if(!m_inotifyFD)
		return false;

	int watchFD = inotify_add_watch(m_inotifyFD, watch_.c_str(), type); 

	//Log an error here when the logging toolkit is done.
	if(-1 == watchFD)
		return false;

	m_watchMap.insert(make_pair(watchFD, handler_));
	return true;
}

bool Watcher::run()
{
	if(!m_inotifyFD)
		return false;

	


}
