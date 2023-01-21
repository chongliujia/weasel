#ifndef WEASEL_CORE_NONCOPYABLE_H
#define WEASEL_CORE_NONCOPYABLE_H

namespace weasel 
{
	class noncopyable
	{
		public:
			noncopyable(const noncopyable&) = delete;
			void operator=(const noncopyable&) = delete;

		protected:
			noncopyable() = default;
			~noncopyable() = default;
	};
}

#endif
