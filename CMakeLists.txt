cmake_minimum_required(VERSION 3.9)
# Keep the version below in sync with the one in db.h
project(leveldb VERSION 1.22.0 LANGUAGES C CXX)

include_directories(
  "${PROJECT_BINARY_DIR}/include"
  "."
)

add_executable(arena_test test/arena_test.cc util/arena.cc)
# target_link_libraries(arena_test )