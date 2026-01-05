#include "../exercise.h"

// READ: 静态字段 <https://zh.cppreference.com/w/cpp/language/static>
// READ: 虚析构函数 <https://zh.cppreference.com/w/cpp/language/destructor>

struct A {
    // 1. 静态字段仅在类内声明，不能直接初始化（类内初始化仅支持constexpr静态成员）
    static int num_a;

    A() {
        ++num_a; // 构造时计数+1
    }

    // 2. 虚析构函数：确保delete基类指针时，调用派生类析构（核心修复点）
    virtual ~A() {
        --num_a; // 析构时计数-1
    }

    virtual char name() const {
        return 'A';
    }
};
// 静态字段必须在类外初始化（全局作用域）
int A::num_a = 0;

struct B final : public A {
    static int num_b;

    B() {
        ++num_b; // 构造时计数+1
    }

    ~B() {
        --num_b; // 析构时计数-1
    }

    char name() const final {
        return 'B';
    }
};
// 初始化B的静态字段
int B::num_b = 0;

int main(int argc, char **argv) {
    auto a = new A;   // 构造A：num_a = 1
    auto b = new B;   // 先构造A（num_a=2），再构造B（num_b=1）
    ASSERT(A::num_a == 2, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(a->name() == 'A', "Fill in the correct value for a->name()");
    ASSERT(b->name() == 'B', "Fill in the correct value for b->name()");

    delete a; // 析构A：num_a = 1
    delete b; // 先析构B（num_b=0），再析构A（num_a=0）
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    A *ab = new B; // 构造B：先A（num_a=1），再B（num_b=1）
    ASSERT(A::num_a == 1, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(ab->name() == 'B', "Fill in the correct value for ab->name()");

    // 3. 基类指针转派生类引用：用static_cast（B是final，安全）
    B &bb = static_cast<B&>(*ab);
    ASSERT(bb.name() == 'B', "Fill in the correct value for bb->name()");

    // 4. 虚析构保证：delete基类指针时，先调用B析构，再调用A析构
    delete ab;
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    return 0;
}