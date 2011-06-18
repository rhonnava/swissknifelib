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
int m_watchFD;
boost::function<void(void)> m_handler;
public:
typedef int watchtype_t;

explicit Watcher(bool blocking = false);

void registerHandler(const boost::function<void(void)>& handler);

bool registerWatch(watchtype_t type, const std::string& watch_);
};
