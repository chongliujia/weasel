#ifndef WEASEL_CORE_TIMESTAMP_H
#define WEASEL_CORE_TIMESTAMP_H

#include "Copyable.h"
#include "Types.h"

namespace weasel
{
	class Timestamp : public weasel::copyable,
					  public std::equailty_comparable<Timestamp>,
}
