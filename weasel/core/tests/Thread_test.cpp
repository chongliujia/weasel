#include "../Thread.h"
#include "../CurrentThread.h"


#include <string>
#include <cstdio>
#include <unistd.h>



class Foo
{
	public:
		explicit Foo(double x)
			: x_(x)
		{
		}

		void memberFunc()
		{
			printf("tid=%d, Foo::x_=%f\n", weasel::CurrentThread::tid(), x_);
		}

		void memeberFunc2(const std::string& text)
		{
			printf("tid=%d, Foo::x_=%f, text=%s\n", weasel::CurrentThread::tid(), x_, text.c_str());
		}

	private:
		double x_;
};


void mysleep(int seconds)
{
	timespec t = { seconds, 0 };
	/*

	The nanosleep function is Linux system call, it can let process sleep during some time, 
	then the process will be awaken.

	*/
	nanosleep(&t, NULL);
}

void threadFunc()
{
	printf("tid=%d\n", weasel::CurrentThread::tid());
}

void threadFunc2(int x)
{
	printf("tid=%d, x=%d\n", weasel::CurrentThread::tid(), x);
}

void threadFunc3()
{
	printf("tid=%d\n", weasel::CurrentThread::tid());
	mysleep(1);
}

int main()
{
	printf("pid = %d\n, tid = %d\n", ::getpid(), weasel::CurrentThread::tid());

	weasel::Thread t1(threadFunc);
	t1.start();
	printf("t1.tid=%d\n", t1.tid());
	t1.join();

	weasel::Thread t2(std::bind(threadFunc2, 23), "Thread for free function with argument");
	t2.start();
	printf("t2.tid=%d\n", t2.tid());
	t2.join();

	Foo foo(100);
	weasel::Thread t3(std::bind(&Foo::memeberFunc, &foo), "Thread for member function without argument");
	t3.start();
	t3.join();

	weasel::Thread t4(std::bind(&Foo::memeberFunc2, std::ref(foo), std::string("jcl")));
	t4.start();
	t4.join();

	{
		weasel::Thread t5(threadFunc3);
		t5.start();
	}

	mysleep(2);
	
	{
		weasel::Thread t6(threadFunc3);
		t6.start();
		mysleep(2);
	}

	sleep(2);

	printf("Number of created threads %d\n", weasel::Thread::numCreated());

	return 0;
}
