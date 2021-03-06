# even-leveldb
leveldb-仿写

## slice注意点

依赖调用者来保证Slice指向的外部字符数组有效

使用Slice需要特别小心，使用者需要确保底层引用的字符串没有被销毁。

使用 slice 之前必须要保证 slice 中指针指向的内存没有被收回

Slice对象仅包含长度和字符指针类型的成员，对象的复制非常高效，但也非常危险。如果拥有一个Slice对象，需要确保其初始化的指针数据也能够保留，并且要保证其上下文是线程安全的。这也是为什么不要在线程间共享Slice对象的原因：它们都具有Slice字符串指针引用的内存存储，并且这些内存区域可能会被其它线程进行修改。
