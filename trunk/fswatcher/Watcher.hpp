#include <map>
#include <string>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/inotify.h>
#include "Exception.hpp"
#include <boost/utility.hpp>
#include <boost/function.hpp>

class Watcher : private boost::noncopyable
{
int m_inotifyFD;
sig_atomic_t m_KeepRunning;
//A map of the currently being watched fds versus the handlers registered for them
std::map<int, boost::function<void(void)> > m_watchMap; 
typedef  map<int, boost::function<void(void)> >::iterator WatchMapIterator;
bool m_ok_;
public:

//File here means file/directory or any other kind of file.
//You can do a bitwise or of multiple of these flags to watch
// for multiple events.
enum{
	FILE_ACCESSED=IN_ACCESS, 
	METADATA_CHANGED=IN_ATTRIB,
	FILE_WRITE_CLOSED=IN_CLOSE_WRITE,
	FILE_NOWRITE_CLOSED=IN_CLOSE_NOWRITE,
	FILE_CREATED=IN_CREATE,
	FILE_DELETED= IN_DELETE,
	WATCH_FILE_DELETED=IN_DELETE_SELF,
	WATCH_FILE_MODIFIED=IN_MODIFY,
	WATCH_FILE_MOVED=IN_MOVE_SELF,
	FILE_MOVED_OUT=IN_MOVED_FROM,
	FILE_MOVED_IN=IN_MOVED_TO,
	FILE_OPEPED=IN_OPEN
};

typedef int watchtype_t;

//The object gets created even if we are not able to create a valid inotify instance
//Use safe bool and operator! to check if the object is fine before registering watches
explicit Watcher(bool blocking = false);

int registerWatch(watchtype_t type, const std::string& watch_, const boost::function<void (void)> & handler_);

bool removeWatch(int wd);

size_t maxWatches() const;

inline size_t numWatches() const;

bool run();

inline void stop();

inline operator bool() const;

inline bool operator!() const;

~Watcher();
};
