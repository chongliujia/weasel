#ifndef WEASEL_CORE_CURRENTTHREAD_H
#define WEASEL_CORE_CURRENTTHREAD_H

#include "Types.h"

/*

currentthread can obtain current running thread.

*/


namespace weasel
{
	namespace CurrentThread
	{
		/*
		internal

		__thread is The standard C storage class specifier for thread local variables.

		the __thread specifier may be applied to any global, file-scoped static, 
		function-scoped static, or static data member of a class

		*/
		extern __thread int t_cachedTid;
		extern __thread char t_tidString[32];
		extern __thread int t_tidStringLength;
		extern __thread const char* t_threadName;

		void cacheTid();

		inline int tid() 
		{
			/*

			__builtin_expect is to tell the compiler that you'll usually find that 
			the expression evaluates to c, so that the compiler can optimize for that case.

			*/
			if (__builtin_expect(t_cachedTid == 0, 0)) {
				cacheTid();
			}

			return t_cachedTid;
		}

		inline const char* tidString() 
		{
			 return t_tidString;
		}

		inline int tidStringLength() 
		{
			return t_tidStringLength;
		}

		inline const char* name()
		{
			return t_threadName;
		}

		bool isMainThread();

		void sleepUsec_test(int64_t usec);

		string stackTrace(bool demangle);
	}
}

#endif
