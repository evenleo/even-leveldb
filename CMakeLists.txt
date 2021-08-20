cmake_minimum_required(VERSION 3.9)
# Keep the version below in sync with the one in db.h
project(leveldb VERSION 1.22.0 LANGUAGES C CXX)

# C++ standard can be overridden when this is used as a sub-project.
if(NOT CMAKE_CXX_STANDARD)
  # This project requires C++11.
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif(NOT CMAKE_CXX_STANDARD)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_definitions(-DLEVELDB_PLATFORM_POSIX=1)

include(TestBigEndian)
test_big_endian(LEVELDB_IS_BIG_ENDIAN)

include(CheckIncludeFile)
check_include_file("unistd.h" HAVE_UNISTD_H)

include(CheckLibraryExists)
check_library_exists(crc32c crc32c_value "" HAVE_CRC32C)
check_library_exists(snappy snappy_compress "" HAVE_SNAPPY)
check_library_exists(tcmalloc malloc "" HAVE_TCMALLOC)

include(CheckCXXSymbolExists)
# Using check_cxx_symbol_exists() instead of check_c_symbol_exists() because
# we're including the header from C++, and feature detection should use the same
# compiler language that the project will use later. Principles aside, some
# versions of do not expose fdatasync() in <unistd.h> in standard C mode
# (-std=c11), but do expose the function in standard C++ mode (-std=c++11).
check_cxx_symbol_exists(fdatasync "unistd.h" HAVE_FDATASYNC)
check_cxx_symbol_exists(F_FULLFSYNC "fcntl.h" HAVE_FULLFSYNC)
check_cxx_symbol_exists(O_CLOEXEC "fcntl.h" HAVE_O_CLOEXEC)

set(LEVELDB_PORT_CONFIG_DIR "include/port")

configure_file(
  "port/port_config.h.in"
  "${PROJECT_BINARY_DIR}/${LEVELDB_PORT_CONFIG_DIR}/port_config.h"
)

include_directories(
  "${PROJECT_BINARY_DIR}/include"
  "."
)

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}/include
  .
)

add_library(leveldb "")
target_sources(leveldb
  PRIVATE
    "${PROJECT_BINARY_DIR}/${LEVELDB_PORT_CONFIG_DIR}/port_config.h"
    "util/arena.cc"
    "util/coding.cc"
    "util/env.cc"
    "util/env_posix.cc"
    "util/status.cc"
    "util/hash.cc"
    "util/logging.cc"
    "util/comparator.cc"
)

add_executable(arena_test test/arena_test.cc)
target_link_libraries(arena_test leveldb)

add_executable(posix_logger_test test/posix_logger_test.cc)
target_link_libraries(posix_logger_test leveldb -lpthread)

add_executable(coding_test test/coding_test.cc)
target_link_libraries(coding_test leveldb)

add_executable(no_destructor_test test/no_destructor_test.cc)

add_executable(skiplist_test test/skiplist_test.cc)
target_link_libraries(skiplist_test leveldb)