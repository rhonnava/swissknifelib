#include "../Watcher.hpp"
#include <iostream>

using namespace std;

void func()
{
	cout << "Pringting in callback" << endl;
}

int main()
{
	Watcher w;
	w.registerWatch(Watcher::FILE_ACCESSED, "/home/rhonnava/test.txt", func);
	cout << "The number of files that can be watched is" << w.maxWatches() << endl;
	w.run();
}
