#ifndef WEASEL_CORE_LOGSTREAM_H
#define WEASEL_CORE_LOGSTREAM_H

#include "noncopyable.h"
#include "StringPiece.h"
#include "Types.h"

#include <assert.h>
#include <string.h>

namespace weasel 
{
    namespace detail
    {
        const int kSmallBuffer = 4000;
        const int kLargeBuffer = 4000 * 1000;

        template <int SIZE>
        class FixedBuffer : noncopyable
        {
            public:
                FixedBuffer()
                    : cur_(data_)
                {
                    setCookie(cookieStart);
                }

                ~FixedBuffer()
                {
                    setCookie(cookieEnd);
                }

                void append(const char* buf, size_t len)
                {
                    if (implicit_cast<size_t>(avail()) > len) {
                        memcpy(cur_, buf, len);
                        cur_ += len;
                    }
                }

                const char* data() const { return data_; }
                const char* debugString();

                char* current() { return cur_; }

                int length() const { return static_cast<int>(cur_ - data_); }
                int avail() const { return static_cast<int>(end() - cur_); }

                void add(size_t len) { cur_ += len; }
                void reset() { cur_ = data_; }
                void bzero() { memZero(data_, sizeof(data_)); }
                void setCookie(void (*cookie)()) { cookie_ = cookie; }

                string toString() const { return string(data_, length()); }
                
                StringPiece toStringPiece() const { return StringPiece(data_, length()); }

            private:
                const char* end() const { return data_ + sizeof(data_); }

                static void cookieStart();
                static void cookieEnd();

                void (*cookie_) ();
                char  data_[SIZE];
                char* cur_;
        };
    }

    class LogStream : noncopyable
    {
        typedef LogStream self;

        public:
            typedef detail::FixedBuffer<detail::kSmallBuffer> Buffer;

            self& operator<<(bool v)
            {
                buffer_.append(v ? "1" : "0", 1);
                return *this;
            }

            self& operator<<(short);
            self& operator<<(unsigned short);
            self& operator<<(int);
            self& operator<<(unsigned int);
            self& operator<<(long);
            self& operator<<(unsigned long);
            self& operator<<(long long);
            self& operator<<(unsigned long long);
            self& operator<<(double);

            self& operator<<(const void*);

            self& operator<<(float v)
            {
                *this << static_cast<double>(v);
                return *this;
            }
            
            self& operator<<(char v)
            {
                buffer_.append(&v, 1);
                return *this;
            }

            self& operator<<(const char* str)
            {
                if (str) {
                    buffer_.append(str, strlen(str));
                }
                else {
                    buffer_.append("(null)", 6);
                }

                return *this;
            }

            self& operator<<(const unsigned char* str)
            {
                return operator<<(reinterpret_cast<const char*>(str));
            }

            self& operator<<(const string& v)
            {
                buffer_.append(v.c_str(), v.size());
                return *this;
            }

            self& operator<<(const StringPiece& v)
            {
                buffer_.append(v.data(), v.size());
                return *this;
            }

            self& operator<<(const Buffer& v)
            {
                *this << v.toStringPiece();
                return *this;
            }

            void append(const char* data, int len)
            {
                buffer_.append(data, len);
            }

            void resetBuffer() 
            {
                buffer_.reset();
            }

            const Buffer& buffer() const
            {
                return buffer_;
            }
            
        private:
            void staticCheck();

            template <typename T>
            void formatInteger(T);

            Buffer buffer_;

            static const int kMaxNumericSize = 48;
    };

    class Fmt
    {
        public:
            template <typename T>
            Fmt(const char* fmt, T val);

            const char* data() const { return buf_; }
            int length() const { return length_; }

        private:
            int  length_;
            char buf_[32];
    };

    inline LogStream& operator<<(LogStream& s, const Fmt& fmt)
    {
        s.append(fmt.data(), fmt.length());
        return s;
    }

    string formatSI(int64_t n);
    string formatIEC(int64_t n);
}

#endif