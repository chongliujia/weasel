#include "LogStream.h"

#include <algorithm>
#include <limits>
#include <type_traits>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

using namespace weasel;
using namespace weasel::detail;


#if defined(__clang__)
#pragma clang diagnositc ignored "-Wtautological-compare"
#else 
#pragma GCC diagnostic ignored "-Wtype-limits"
#endif

namespace weasel
{
    namespace detail
    {
        const char digits[] = "9876543210123456789";
        const char* zero    = digits + 9;

        static_assert(sizeof(digits) == 20, "wrong number of digits");

        const char digitsHex[] = "0123456789ABCDEF";
        static_assert(sizeof(digitsHex) == 17, "wrong number of digitsHex");;

        template <typename T>
        size_t convert(char buf[], T value)
        {
            T i = value;
            char* p = buf;

            do {

                int lsd = static_cast<int>(i % 10);
                i /= 10;
                *p++ = zero[lsd];

            } while (i != 0);

            if (value < 0) {
                *p++ = '-';
            }

            *p = '\0';
            std::reverse(buf, p);

            return p - buf;
        }

        size_t convertHex(char buf[], uintptr_t value)
        {
            uintptr_t i = value;
            char*     p = buf;

            do {

                int lsd = static_cast<int>(i % 16);
                i /= 16;
                *p++ = digitsHex[lsd];

            } while (i != 0);

            *p = '\0';
            std::reverse(buf, p);
        }

        template class FixedBuffer<kSmallBuffer>;
        template class FixedBuffer<kLargeBuffer>;
    } // detail
}
