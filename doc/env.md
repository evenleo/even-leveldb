# Env类

## Env

参考资料：https://www.ravenxrz.ink/archives/c451920b.html

Env 代表 Envirenment，环境类，在其内部封装了 leveldb 所有的文件操作和线程调度操作。Env 是一个虚基类，具体操作由子类实现。因为我是在 Linux 平台研究，只关注 PosixEnv。

```cpp
leveldb/util/env_posix.cc

using PosixDefaultEnv = SingletonEnv<PosixEnv>;

Env* Env::Default() {
    static PosixDefaultEnv env_container;
    return env_container.env();
}
```

env::Default() 调用 env_container.env()，而 env_container 是 PosixDefaultEnv 类型（PosixDefaultEnv 是 SingletonEnv 的别名），env 返回 env_storage_.

关于 env_storage_，这个变量在 SingletonEnv 类中：

```cpp
// 用于字节对齐，这里表示env_storage_按照EnvType的大小对齐，同时自身的大小也是env_storage_
typename std::aligned_storage<sizeof(EnvType), alignof(EnvType)>::type
    env_storage_;

// env_storage_在PosixDefaultEnv的构造函数中被初始化：
// 这句话的意思是，按照EvnType的大小申请一个对象，然后将这个对象填充到env_storage_的地址空间中去。
new (&env_storage_) EnvType();
```

## 线程池、队列






