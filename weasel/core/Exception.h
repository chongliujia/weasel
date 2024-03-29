#ifndef WEASEL_CORE_EXCEPTION_H
#define WEASEL_CORE_EXCEPTION_H

#include "Types.h"

#include <exception>

namespace weasel
{
	class Exception : public std::exception
	{
		public:
			Exception(string what);
			~Exception() noexcept override = default;

			const char* what() const noexcept override
			{
				return message_.c_str();
			}

			const char* stackTrace() const noexcept
			{
				return stack_.c_str();
			}

		private:
			string message_;
			string stack_;

	};
}

#endif
