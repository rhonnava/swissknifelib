#ifndef DEFS_HPP
#define DEFS_HPP
namespace internal
{
const char* INOTIFY_MAX_WATCHES_FILE = "/proc/sys/fs/inotify/max_user_watches";


enum{STICKY=0, PATH, WATCHTYPE, FUNC};


} //namespace internal
#endif
