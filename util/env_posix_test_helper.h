#ifndef LEVELDB_UTIL_ENV_POSIX_TEST_HELPER_H_
#define LEVELDB_UTIL_ENV_POSIX_TEST_HELPER_H_

namespace leveldb {

class EnvPosixTest;

class EnvPosixTestHelper {
private:
    friend class EnvPosixTest;
    
    // 设置将打开的只读文件的最大数目。
    // 必须在创建环境之前调用。
    static void SetReadOnlyFDLimit(int limit);
    
    // 设置将通过mmap映射的只读文件的最大数目。
    // 必须在创建环境之前调用。
    static void SetReadOnlyMMapLimit(int limit);
};

}  // namespace leveldb

#endif