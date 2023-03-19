#ifndef WEASEL_CORE_TIMESTAMP_H
#define WEASEL_CORE_TIMESTAMP_H

#include "Copyable.h"
#include "Types.h"

#include <boost/operators.hpp>


namespace weasel
{
	class Timestamp : public weasel::copyable,
					  public boost::equailty_comparable<Timestamp>,
                      public boost::less_than_comparable<Timestemp>
    {
        public:



        private:
            int64_t microSecondsSinceEpoch_;
    };
}
