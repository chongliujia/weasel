#ifndef WEASEL_CORE_COUNTDOWNLATCH_H
#define WEASEL_CORE_COUNTDOWNLATCH_H

#include "Condition.h"
#include "Mutex.h"

namespace weasel
{
	class CountDownLatch : noncopyable
	{
		public:
			explicit CountDownLatch(int count);

			void wait();

			void countDown();

			int getCount() const;

		private:
			mutable MutexLock mutex_;
			Condition condition_ GUARDED_BY(mutex_);
			int count_ GUARDED_BY(mutex_);

	};

}

#endif
