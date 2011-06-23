#include "Watcher.hpp"
#include <sys/select.h>
#include <sys/time.h>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include "internal/defs.hpp"

using namespace std;
using namespace boost;

Watcher::Watcher(bool blocking) : m_inotifyFD(0), m_KeepRunning(0), m_ok_(false)
{
	m_inotifyFD = inotify_init1(0);
	
	if(m_inotifyFD > 0)
		m_ok_ = true;

}

inline Watcher::operator bool() const
{
	return m_ok_;
}


inline bool Watcher::operator!() const
{
	return !m_ok_;
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

	//Length of the buffer we read in when inotify triggers
	int EVENT_SIZE = sizeof(struct inotify_event);
	int BUF_LEN = 1024 * (EVENT_SIZE + 16);

	unsigned char buf[BUF_LEN];

	m_KeepRunning = 1;
	while(m_KeepRunning)
	{
		int loc = 0;
		int namelen = 0;
		int readlen = read(m_inotifyFD, buf, BUF_LEN);
	
		if(0 == readlen)
			continue;

		if(-1 == readlen)
		{
			//Do all the handling as per errno
			switch(errno)
			{
				case EINTR:
					continue;
				case EFAULT: //Fall through
				case EINVAL:
				case EBADF:
				default:
					break;
			}
		}
			
		while(loc < readlen)
		{
			struct inotify_event* evt = reinterpret_cast<struct inotify_event*>(buf + loc);
			
			m_watchMapIterator itr = m_watchMap.find(evt->wd);
			if(m_watchMap.end() != itr)
			{
				(itr->second)();
			}

			loc += evt->len + EVENT_SIZE;
		}
	}
}

//Use scopeguard here once its written
size_t Watcher::maxWatches() const
{
	//I am discouraged from using _sysctl(see man page). So, going into procfs. 
	ifstream procfile(INOTIFY_MAX_WATCHES_FILE, ifstream::in);
	size_t ret = 0;
	if(procfile.good())
	{
		char buffer[32];
		memset(&buffer, 0, sizeof(buffer));
		procfile.getline(buffer, 31);
	
		try
		{	
			ret = lexical_cast<size_t>(buffer);
		}
		catch(bad_lexical_cast & e)
		{
			//Do nothing 
		}

		procfile.close();
	}

	return ret;
}

size_t Watcher::numWatches() const
{
	return m_watchMap.size();
}

void Watcher::stop()
{
	m_KeepRunning = 0;
}
Watcher::~Watcher()
{
	close(m_inotifyFD);
}
