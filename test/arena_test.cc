#include "util/arena.h"
#include "include/leveldb/slice.h"
#include <iostream>

using namespace leveldb;

int main() {
    Arena arena;
    Slice s;
    {
        std::string str = "hello";
        s = str;
        str.clear();
    }

    std::cout << s.data() << std::endl;
    
    return 0;
}