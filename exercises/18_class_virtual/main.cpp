#include "../exercise.h"

// READ: 虚函数 <https://zh.cppreference.com/w/cpp/language/virtual>

struct A {
    virtual char virtual_name() const {
        return 'A';
    }
    char direct_name() const {
        return 'A';
    }
};
struct B : public A {
    // READ: override <https://zh.cppreference.com/w/cpp/language/override>
    char virtual_name() const override {
        return 'B';
    }
    char direct_name() const {
        return 'B';
    }
};
struct C : public B {
    // READ: final <https://zh.cppreference.com/w/cpp/language/final>
    char virtual_name() const final {
        return 'C';
    }
    char direct_name() const {
        return 'C';
    }
};
struct D : public C {
    char direct_name() const {
        return 'D';
    }
};

int main(int argc, char **argv) {
    constexpr auto MSG = "Replace '?' with its correct name.";

    A a;
    B b;
    C c;
    D d;

    // 直接调用对象的成员函数
    ASSERT(a.virtual_name() == 'A', MSG);
    ASSERT(b.virtual_name() == 'B', MSG);
    ASSERT(c.virtual_name() == 'C', MSG);
    ASSERT(d.virtual_name() == 'C', MSG); // D未重写，继承C的final版本
    ASSERT(a.direct_name() == 'A', MSG);
    ASSERT(b.direct_name() == 'B', MSG);
    ASSERT(c.direct_name() == 'C', MSG);
    ASSERT(d.direct_name() == 'D', MSG);

    A &rab = b;
    B &rbc = c;
    C &rcd = d;

    // 引用绑定派生类对象（虚函数动态绑定，普通函数静态绑定）
    ASSERT(rab.virtual_name() == 'B', MSG); // 动态绑定b的实际类型B
    ASSERT(rbc.virtual_name() == 'C', MSG); // 动态绑定c的实际类型C
    ASSERT(rcd.virtual_name() == 'C', MSG); // 动态绑定d的实际类型D（继承C的虚函数）
    ASSERT(rab.direct_name() == 'A', MSG); // 静态绑定rab的类型A&
    ASSERT(rbc.direct_name() == 'B', MSG); 
    ASSERT(rcd.direct_name() == 'C', MSG); 

    A &rac = c;
    B &rbd = d;

    // 跨级引用
    ASSERT(rac.virtual_name() == 'C', MSG); // 动态绑定c的实际类型C
    ASSERT(rbd.virtual_name() == 'C', MSG); // 动态绑定d的实际类型D（继承C的虚函数）
    ASSERT(rac.direct_name() == 'A', MSG); // 静态绑定rac的类型A&
    ASSERT(rbd.direct_name() == 'B', MSG); // 静态绑定rbd的类型B&

    A &rad = d;

    // 最顶层基类引用绑定最底层派生类
    ASSERT(rad.virtual_name() == 'C', MSG); // 动态绑定d的实际类型D
    ASSERT(rad.direct_name() == 'A', MSG); // 静态绑定rad的类型A&

    return 0;
}
// READ: 扩展阅读-纯虚、抽象 <https://zh.cppreference.com/w/cpp/language/abstract_class>
// READ: 扩展阅读-虚继承 <https://zh.cppreference.com/w/cpp/language/derived_class>
