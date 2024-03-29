#include "Exception.h"
#include "CurrentThread.h"

namespace weasel
{
	Exception::Exception(string msg)
		: message_(std::move(msg)),
		  stack_(CurrentThread::stackTrace(false))
	{
	}
}
