#ifndef LEVELDB_UTIL_POSIX_LOGGER_H_
#define LEVELDB_UTIL_POSIX_LOGGER_H_

#include <sys/time.h>

#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <sstream>
#include <thread>

#include "leveldb/env.h"

namespace leveldb {

// 使用关键字final，意味着继承该类会导致编译错误
class PosixLogger final : public Logger {
public:
    // 创建写入给定文件的记录器。
    // PosixLogger实例拥有文件句柄的所有权。 explicit修饰后, 就不能再被隐式调用
    explicit PosixLogger(std::FILE* fp) : fp_(fp) { assert(fp != nullptr); }

    ~PosixLogger() override { std::fclose(fp_); }

    void Logv(const char* format, va_list arguments) override {
        // Record the time as close to the Logv() call as possible.
        struct ::timeval now_timeval;
        ::gettimeofday(&now_timeval, nullptr);
        const std::time_t now_seconds = now_timeval.tv_sec;
        struct std::tm now_components;
        ::localtime_r(&now_seconds, &now_components);

        // Record the thread ID.
        constexpr const int kMaxThreadIdSize = 32;
        std::ostringstream thread_stream;
        thread_stream << std::this_thread::get_id();
        std::string thread_id = thread_stream.str();
        if (thread_id.size() > kMaxThreadIdSize) {
            thread_id.resize(kMaxThreadIdSize);
        }

        // 我们首先尝试打印到堆栈分配的缓冲区。如果这次尝试
        // 如果失败，我们将使用动态分配的缓冲区进行第二次尝试。
        constexpr const int kStackBufferSize = 512;
        char stack_buffer[kStackBufferSize];
        static_assert(sizeof(stack_buffer) == static_cast<size_t>(kStackBufferSize),
                    "sizeof(char) is expected to be 1 in C++");

        int dynamic_buffer_size = 0;  // Computed in the first iteration.
        for (int iteration = 0; iteration < 2; ++iteration) {
            const int buffer_size =
                (iteration == 0) ? kStackBufferSize : dynamic_buffer_size;
            char* const buffer =
                (iteration == 0) ? stack_buffer : new char[dynamic_buffer_size];

            // Print the header into the buffer.
            int buffer_offset = snprintf(
                buffer, buffer_size, "%04d/%02d/%02d-%02d:%02d:%02d.%06d %s ",
                now_components.tm_year + 1900, now_components.tm_mon + 1,
                now_components.tm_mday, now_components.tm_hour, now_components.tm_min,
                now_components.tm_sec, static_cast<int>(now_timeval.tv_usec),
                thread_id.c_str());

            // 标题最多可包含28个字符（10个日期+15个时间）+
            // 3个分隔符）加上线程ID，它应该适合
            // 静态缓冲区。
            assert(buffer_offset <= 28 + kMaxThreadIdSize);
            static_assert(28 + kMaxThreadIdSize < kStackBufferSize,
                            "stack-allocated buffer may not fit the message header");
            assert(buffer_offset < buffer_size);

            // Print the message into the buffer.
            std::va_list arguments_copy;
            va_copy(arguments_copy, arguments);
            buffer_offset +=
                std::vsnprintf(buffer + buffer_offset, buffer_size - buffer_offset,
                                format, arguments_copy);
            va_end(arguments_copy);

            // 下面的代码可能会在缓冲区的末尾附加一个新行
            // 需要额外的字符。
            if (buffer_offset >= buffer_size - 1) {
                // The message did not fit into the buffer.
                if (iteration == 0) {
                    // 重新运行循环并使用动态分配的缓冲区。缓冲器
                    // 将足够大的日志消息，一个额外的换行符和一个
                    // 空终止符。
                    dynamic_buffer_size = buffer_offset + 2;
                    continue;
                }

                // 动态分配的缓冲区大小不正确。这应该
                // 假设（v）snprintf实现正确，则不会发生。失败
                // 在测试中，通过截断生产中的日志消息进行恢复。
                assert(false);
                buffer_offset = buffer_size - 1;
            }

            // Add a newline if necessary.
            if (buffer[buffer_offset - 1] != '\n') {
                buffer[buffer_offset] = '\n';
                ++buffer_offset;
            }

            assert(buffer_offset <= buffer_size);
            std::fwrite(buffer, 1, buffer_offset, fp_);
            std::fflush(fp_);

            if (iteration != 0) {
                delete[] buffer;
            }
            break;
        }
    }

private:
    std::FILE* const fp_;
};

}  // namespace leveldb

#endif  // LEVELDB_UTIL_POSIX_LOGGER_H_
