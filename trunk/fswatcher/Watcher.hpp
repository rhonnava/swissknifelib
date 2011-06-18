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
public:
typedef int watchtype_t;

Watcher();

bool registerHandler(boost::function<void(void)> handler);

bool registerWatch(watchtype_t type, const std::string& watch_);
};
