#include "Watcher.hpp"

using namespace std;

Watcher::Watcher()
{
	if (-1 == inotify_init1(O_NONBLOCK))
	{
		throw Exception(L"Unable to initialize watch"); //Add strerror once Exception class is ready
	}
}

bool Watcher::registerHandler(boost::function<void(void)> handler)
{
	cout << "registering handler" << endl;
	return true;
}

bool Watcher::registerWatch(watchtype_t type, const string& watch_)
{
	cout << "register watch" << endl;
	return true;
}

