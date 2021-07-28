#include <iostream>

#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <atomic>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>

#include "util/posix_logger.h"

using namespace leveldb;

#if defined(HAVE_O_CLOEXEC)
constexpr const int kOpenBaseFlags = O_CLOEXEC;
#else
constexpr const int kOpenBaseFlags = 0;
#endif  // defined(HAVE_O_CLOEXEC)

Status NewLogger(const std::string& filename, Logger** result) {
    int fd = ::open(filename.c_str(),
                    O_APPEND | O_WRONLY | O_CREAT | kOpenBaseFlags, 0644);
    if (fd < 0) {
        *result = nullptr;
        // return PosixError(filename, errno);
        return Status::OK();
    }

    std::FILE* fp = ::fdopen(fd, "w");
    if (fp == nullptr) {
        ::close(fd);
        *result = nullptr;
        // return PosixError(filename, errno);
        return Status::OK();
    } else {
        *result = new PosixLogger(fp);
        return Status::OK();
    }
}

int main() {
    
    
    return 0;
}