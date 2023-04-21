#ifndef WEASEL_CORE_LOGGING_H
#define WEASEL_CORE_LOGGING_H

#include "LogStream.h"
#include "Timestamp.h"



namespace weasel
{
    class TimeZone;

    class Logger
    {
        public:
            enum LogLevel 
            {
                TRACE,
                DEBUG,
                INFO,
                WARN,
                ERROR,
                FATAL,
                NUM_LOG_LEVELS,
            };

            class SourceFile
            {
                public:
                    template <int N>
                    SourceFile(const char (&arr)[N])
                        : data_(arr), size_(N - 1)
                    {
                        const char* slash = strrchr(data_, '/');
                        
                        if (slash) {
                            data_   = slash + 1;
                            size_t -= static_cast<int>(data_ - arr);
                        }
                    }

                    explicit SourceFile(const char* filename)
                        : data_(filename)
                    {
                        const char* slash = strrchr(filename, '/');

                        if (slash) {
                            data_ = slash + 1;
                        }

                        size_ = static_cast<int>(strlen(data_));
                    }

                private:
                    const char* data_;
                    
                    int size_;
            };


            Logger(SourceFile file, int line);
            Logger(SourceFile file, int line, LogLevel level);
            Logger(SourceFile file, int line, LogLevel level, const char* func);
            Logger(SourceFile file, int line, bool toAbort);

            ~Logger();

            LogStream& stream() { return impl_.stream_; }

            static LogLevel logLevel();
            static void setLogLevel(LogLevel level);

            typedef void (*OutputFunc)(const char* msg, int len);
            typedef void (*FlushFunc)();

            static void setOutput(OutputFunc);
            static void setFlush(FlushFunc);
            static void setTimeZone(const TimeZone& tz);

        private:
            class Impl 
            {
                public:
                    typedef Logger::LogLevel LogLevel;

                    Impl(LogLevel level, int old_errno, const SourceFile& file, int line);

                    void formatTime();

                    void finish();

                    Timestamp  time_;
                    LogStream  stream_;
                    LogLevel   level_;

                    int        line_;

                    SourceFile basename_;
            };

            Impl impl_;
    };

    extern Logger::LogLevel g_logLevel;

    inline Logger::LogLevel Logger::logLevel()
    {
        return g_logLevel;
    }

#define LOG_TRACE if (weasel::Logger::logLevel() <= weasel::Logger::TRACE) \
    weasel::Logger(__FILE__, __LINE__, weasel::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (weasel::Logger::logLevel() <= weasel::Logger::DEBUG) \
    weasel::Logger(__FILE__, __LINE__, weasel::Logger::DEBUG, __func__).stream()
#define LOG_INFO if (weasel::Logger::logLevel() <= weasel::Logger::INFO) \
    weasel::Logger(__FILE__, __LINE__).stream()

#define LOG_WARN weasel::Logger(__FILE__, __LINE__, weasel::Logger::WARN).stream()
#define LOG_ERROR weasel::Logger(__FILE__, __LINE__, weasel::Logger::ERROR).stream()
#define LOG_FATAL weasel::Logger(__FILE__, __LINE__, weasel::Logger::FATAL).stream()

#define LOG_SYSERR weasel::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL weasel::Logger(__FILE__, __LINE__, true).stream()
    
    const char* strerror_tl(int savedErrno);

#define CHECK_NOTNULL(val) \
    ::weasel::CheckNotNULL(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

    template <typename T>
    T* CheckNotNULL(Logger::SourceFile file, int line, const char *names, T* ptr)
    {
        if (ptr == NULL) {
            Logger(file, line, Logger::FATAL).stream() << names;
        }
        return ptr;
    }
}

#endif 
