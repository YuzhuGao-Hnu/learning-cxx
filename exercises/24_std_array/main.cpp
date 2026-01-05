#include "../exercise.h"
#include <array>
#include <cstring>

// READ: std::array <https://zh.cppreference.com/w/cpp/container/array>

// TODO: 将下列 `?` 替换为正确的代码
int main(int argc, char **argv) {
    {
        std::array<int, 5> arr{{1, 2, 3, 4, 5}};
        ASSERT(arr.size() == 5, "Fill in the correct value.");
        ASSERT(sizeof(arr) == 20, "Fill in the correct value."); // 5×4=20（int占4字节）
        int ans[]{1, 2, 3, 4, 5};
        // data()返回底层数组指针，sizeof(arr)是总字节数（20）
        ASSERT(std::memcmp(arr.data(), ans, sizeof(arr)) == 0, "Fill in the correct values.");
    }
    {
        std::array<double, 8> arr;
        ASSERT(arr.size() == 8, "Fill in the correct value."); // 模板参数指定8个元素
        ASSERT(sizeof(arr) == 64, "Fill in the correct value."); // 8×8=64（double占8字节）
    }
    {
        std::array<char, 21> arr{"Hello, InfiniTensor!"};
        ASSERT(arr.size() == 21, "Fill in the correct value."); // 模板参数指定21个char
        ASSERT(sizeof(arr) == 21, "Fill in the correct value."); // 21×1=21（char占1字节）
        // data()返回char*，适配strcmp的C风格字符串参数
        ASSERT(std::strcmp(arr.data(), "Hello, InfiniTensor!") == 0, "Fill in the correct value.");
    }
    return 0;
}
