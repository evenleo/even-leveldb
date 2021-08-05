#include "util/no_destructor.h"
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <utility>

namespace leveldb {

struct Foo {
  Foo() {} // 这里需要定义才能编译通过
  Foo(uint32_t a, uint64_t b) : a(a), b(b) {}
  ~Foo() { std::abort(); }
  uint32_t a;
  uint64_t b;
};

constexpr const uint32_t kGoldenA = 0xdeadbeef;
constexpr const uint64_t kGoldenB = 0xaabbccddeeffaabb;

void test1() {
  static NoDestructor<Foo> instance(kGoldenA, kGoldenB);
  assert(kGoldenA == instance.get()->a);
  assert(kGoldenB == instance.get()->b);
}

} // namespace leveldb

int main() {
  leveldb::test1();
  return 0;
}