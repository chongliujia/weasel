#include "Condition.h"

#include <errno.h>


bool weasel::Condition::waitForSeconds(double seconds)
{
	struct timespec abstime;

	clock_gettime(CLOCK_REALTIME, &abstime);

	const int64_t kNanoSecondsPerSeconds = 1000000000;
	int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSeconds);

	abstime.tv_sec  += static_cast<time_t>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSeconds);
	abstime.tv_nsec  = static_cast<long>((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSeconds);

	MutexLock::UnassignGuard ug(mutex_);
	return ETIMEDOUT == pthread_cond_timedwait(&pcond_, mutex_.getPthreadMutex(), &abstime);
}
