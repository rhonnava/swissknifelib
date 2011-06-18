#include <set>
#include <string>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <sys/inotify.h>
#include "Exception.hpp"
#include <boost/utility.hpp>
#include <boost/function.hpp>

class Watcher : private boost::noncopyable
{
int m_inotifyFD;

//Watch can be called on an already watched file, in which case the same fd is returned
//We need a set to avoid duplicates. Insertion, traversal is going to be slower than vector
//but given the cvheck for uniqueness this is the best choice.
set<int> m_watchFDs; 
boost::function<void(void)> m_handler;
public:

//File here means file/directory or any other kind of file.
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

explicit Watcher(bool blocking = false);

void registerHandler(const boost::function<void(void)>& handler);

bool registerWatch(watchtype_t type, const std::string& watch_);

bool run();
};
