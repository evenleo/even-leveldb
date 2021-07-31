#include "util/coding.h"
#include <cassert>
#include <iostream>

using namespace leveldb;

void test_fixed() {
  uint32_t val_32 = 102;
  uint64_t val_64 = 10200000;

  uint32_t val_32_res = 0;
  uint64_t val_64_res = 0;
  std::string buf_dst_32;
  std::string buf_dst_64;

  PutFixed32(&buf_dst_32, val_32);
  PutFixed64(&buf_dst_64, val_64);
  assert(DecodeFixed32(buf_dst_32.data()) == val_32);
  assert(DecodeFixed64(buf_dst_64.data()) == val_64);
}

void test_varint() {
  uint32_t val_32 = 102;
  uint64_t val_64 = 10200000;

  std::string buf_dst_32;
  std::string buf_dst_64;

  uint32_t val_32_res = 0;
  uint64_t val_64_res = 0;

  PutVarint32(&buf_dst_32, val_32);
  PutVarint64(&buf_dst_64, val_64);

  const char *p = buf_dst_32.data();
  const char *limit = buf_dst_32.data() + buf_dst_32.size();
  const char *p_32 = GetVarint32Ptr(p, limit, &val_32_res);
  assert(p_32 != nullptr);
  assert(val_32_res == val_32);

  p = buf_dst_64.data();
  limit = buf_dst_64.data() + buf_dst_64.size();
  const char *p_64 = GetVarint64Ptr(p, limit, &val_64_res);
  assert(p_64 != nullptr);
  assert(val_64_res == val_64);
}

int main() {
  test_fixed();
  test_varint();

  return 0;
}