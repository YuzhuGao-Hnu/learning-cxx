#include "../exercise.h"
#include <cassert>

class DynFibonacci {
    size_t* cache;
    int cached;
    int capacity;  // 新增：记录缓存容量，用于越界检查和移动语义

public:
    // TODO: 实现动态设置容量的构造器
    DynFibonacci(int cap) 
        : cache(new size_t[cap]),  // 分配指定容量的动态数组
          cached(2),               // 初始缓存到索引1（0和1已初始化）
          capacity(cap)            // 记录缓存容量
    {
        // 防御性检查：容量至少为2（否则无法存储基础值）
        ASSERT(cap >= 2, "Capacity must be at least 2");
        // 斐波那契基础值：f(0)=0, f(1)=1
        cache[0] = 0;
        cache[1] = 1;
    }

    // TODO: 实现移动构造器
    // noexcept：保证不抛异常，容器/算法会优先选择移动而非拷贝
    DynFibonacci(DynFibonacci&& other) noexcept 
        : cache(other.cache),       // 接管源对象的缓存指针
          cached(other.cached),     // 拷贝已缓存索引
          capacity(other.capacity)  // 拷贝容量
    {
        // 源对象置空：避免析构时重复释放资源，且满足is_alive()检查
        other.cache = nullptr;
        other.cached = 0;
        other.capacity = 0;
    }

    // TODO: 实现移动赋值
    // NOTICE: ⚠ 注意移动到自身问题 ⚠
    DynFibonacci& operator=(DynFibonacci&& other) noexcept {
        // 1. 处理自赋值（移动到自身时直接返回）
        if (this == &other) {
            return *this;
        }

        // 2. 释放当前对象的资源（避免内存泄漏）
        delete[] cache;

        // 3. 接管源对象的资源
        cache = other.cache;
        cached = other.cached;
        capacity = other.capacity;

        // 4. 源对象置空（避免析构冲突）
        other.cache = nullptr;
        other.cached = 0;
        other.capacity = 0;

        return *this;
    }

    // TODO: 实现析构器，释放缓存空间
    ~DynFibonacci() {
        delete[] cache;  // delete[] nullptr 是安全的，无需额外判断
    }

    // TODO: 实现正确的缓存优化斐波那契计算
    size_t operator[](int i) {
        // 越界检查：确保索引在合法范围
        ASSERT(i >= 0 && i < capacity, "Index out of range");
        // 按需计算：仅填充到目标索引i，已缓存的部分直接复用
        for (; cached <= i; ++cached) {
            cache[cached] = cache[cached - 1] + cache[cached - 2];
        }
        return cache[i];
    }

    // NOTICE: 不要修改这个方法
    size_t operator[](int i) const {
        ASSERT(i <= cached, "i out of range");
        return cache[i];
    }

    // NOTICE: 不要修改这个方法
    bool is_alive() const {
        return cache;
    }

    // 禁用拷贝构造和拷贝赋值（避免浅拷贝问题）
    DynFibonacci(const DynFibonacci&) = delete;
    DynFibonacci& operator=(const DynFibonacci&) = delete;
};

int main(int argc, char** argv) {
    DynFibonacci fib(12);
    ASSERT(fib[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci const fib_ = std::move(fib);
    ASSERT(!fib.is_alive(), "Object moved");
    ASSERT(fib_[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci fib0(6);
    DynFibonacci fib1(12);

    fib0 = std::move(fib1);
    fib0 = std::move(fib0);  // 测试自赋值
    ASSERT(fib0[10] == 55, "fibonacci(10) should be 55");

    return 0;
}