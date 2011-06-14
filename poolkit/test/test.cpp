#include <Factory.hpp>
#include <Thread.hpp>
#include <string>
#include <utility>
#include <boost/lambda/lambda.hpp>

using namespace std;
using namespace boost::lambda;

class Tester{
string m_str;
public:
Tester(string str) : m_str(str)
{
}

void print()
{
	cout << "Printing in tester" << endl;
}
};

int main()
{
	pair<int, int> p(10, 20);
	cout << p.second << endl;
	Pool<Tester> t(string("test"));
	t.reset();
	t.rearrange();
	Pool<Tester>::iterator obj = t.acquire();
	Pool<Tester>::iterator obj1 = t.acquire();
	t.acquire();
	t.acquire();
	t.acquire();
	cout << "The used count is: " << t.used() << endl;
	(obj->first).print();
	t.release(obj);
	//t.release(obj1);
	cout << "The used count is: " << t.used() << endl;

	Pool<Tester>::unsafe_iterator itr = t.begin_unsafe();	
	while(t.end_unsafe() != itr)
	{
		cout << boolalpha << (*itr).second << endl;
		++itr;
	}
}
