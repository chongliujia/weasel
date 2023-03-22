#include "TimeZone.h"
#include "noncopyable.h"
#include "Data.h"

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <assert.h>
#include <endian.h>
#include <stdio.h>


using namespace weasel;

struct TimeZone::Data
{
    struct Transition
    {
        int64_t utctime;
        int64_t localtion;
        
        int localtimeIdx;

        Transition(int64_t t, int64_t l, int localIdx)
            : utctime(t), localtion(l), localtimeIdx(localIdx)
        {
        }
    };

    struct LocalTime
    {
        int32_t utcOffset;
        bool    isDst;
        int     desigIdx;

        LocalTime(int32_t offset, bool dst, int idx)
            : utcOffset(offset), isDst(dst), desigIdx(idx)
        {
        }
    };

    void addTransition(int64_t utcTime, int localtimeIdx)
    {
        LocalTime lt = localtimes.at(localtimeIdx);
        transitions.push_back(Transition(utcTime, utcTime + lt.utcOffset, localtimeIdx));
    }

    const LocalTime* findLocalTime(int64_t utcTime) const;
    const LocalTime* findLocalTime(const struct DataTime& local, bool postTransition) const;

    struct CompareUtcTime
    {
        bool operator() (const Transition& lhs, const Transition& rhs) const
        {
            return lhs.utctime < rhs.utctime;
        }
    };

    struct CompareLocalTime
    {
        bool operator() (const Transition& lhs, const Transition& rhs) const
        {
            return lhs.localtime < rhs.localtime;
        }
    };

    std::vector<Transition> transitions;
    std::vector<LocalTime>  localtimes;

    string abbreviation;
    string tzstring;
};

namespace weasel
{
    const int kSecondsPerDay = 24 * 60 * 60;

    namespace detail
    {
        class File : noncopyable
        {
            public:
                File(const char* file)
                    : fp_(::open(file, "rb"))
                {
                }

                ~File()
                {
                    if (fp_) {
                        ::fclose(fp_);
                    }
                }

                bool valid() const 
                {
                    return fp_;
                }

                string readBytes(int n)
                {
                    char buf[n];
                    ssize_t nr = ::fread(buf, 1, n, fp_);

                    if (nr != n) {
                        throw std::logic_error("no enough data");
                    }

                    return string(buf, n);
                }

                string readToEnd()
                {
                    char buf[4096];
                    string  result;
                    ssize_t nr = 0;

                    while ((nr = ::fread(buf, 1, sizeof buf, fp_)) > 0) {
                        result.append(buf, nr);
                    }

                    return result;
                }

                int64_t readInt64()
                {
                    int64_t x = 0;

                    ssize_t nr = ::fread(&x, 1, sizeof(int64_t), fp_);

                    if (nr != sizeof(int64_t)) {
                        throw std::logic_error("bad int64_t data");
                    }

                    return be64oh(x);
                }

                int32_t readInt32()
                {
                    int32_t x = 0;

                    ssize_t nr = ::fread(&x, 1, sizeof(int32_t), fp_);

                    if (nr != sizeof(int32_t)) {
                        throw std::logic_error("bad int32_t data");
                    }

                    return be32toh(x);
                }

                uint8_t readUInt8()
                {
                    uint8_t x = 0;

                    ssize_t nr = ::fread(&x, 1, sizeof(uint8_t), fp_);

                    if (nr != sizeof(uint8_t)) {
                        throw std::logic_error("bad uint8_t data");
                    }

                    return x;
                }

                off_t skip(ssize_t bytes)
                {
                    return ::fseek(fp_, bytes, SEEK_CUR);
                }

            private:
                FILE* fp_;
        }
    }
}
