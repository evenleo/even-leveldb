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

int main() {
    Env* env_ = Env::Default();
    Logger* logger = nullptr;
    env_->NewLogger("./test.log", &logger);
    int n = 100;
    Log(logger, "even test [%d]", n);
    
    return 0;
}