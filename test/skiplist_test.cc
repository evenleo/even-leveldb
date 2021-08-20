#include "db/skiplist.h"
#include "leveldb/comparator.h"

#include <iostream>
#include <string>

using namespace leveldb;

int main() {
  Arena arena;
  const Comparator *comparator = BytewiseComparator();
  SkipList<std::string, Comparator> skiplist((Comparator)*comparator, &arena);
}