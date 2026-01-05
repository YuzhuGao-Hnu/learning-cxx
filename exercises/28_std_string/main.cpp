#include "../exercise.h"
#include <string>
// 必须包含此头文件才能使用 std::is_same_v
#include <type_traits>

int main(int argc, char **argv) {
    using namespace std::string_literals;
    auto hello = "Hello"s;
    auto world = "world";

    // 替换占位符：推导 hello 的类型是 std::string
    ASSERT((std::is_same_v<decltype(hello), std::string>), "Fill in the missing type.");
    // 替换占位符：推导 world 的类型是 const char*
    ASSERT((std::is_same_v<decltype(world), const char*>), "Fill in the missing type.");
    // 替换占位符：拼接结果是 "Hello, world!"
    ASSERT(hello + ", " + world + '!' == "Hello, world!", "Fill in the missing string.");
    return 0;
}