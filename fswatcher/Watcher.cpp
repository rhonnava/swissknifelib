#include "Watcher.hpp"

using namespace std;

Watcher::Watcher(bool blocking)
{
	m_inotifyFD = inotify_init1(O_NONBLOCK);

	if (-1 == m_inotifyFD)
	{
		throw Exception(L"Unable to initialize Watcher"); //Add strerror once Exception class is ready
	}
}

void Watcher::registerHandler(const boost::function<void(void)>& handler )
{
	m_handler = handler;
}

bool Watcher::registerWatch(watchtype_t type, const string& watch_)
{
	m_watchFD = inotify_add_watch(m_inotifyFD, watch_.c_str(), type); 

	if(-1 == m_watchFD)
		return false;

	return true;
}

