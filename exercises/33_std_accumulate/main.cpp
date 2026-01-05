#include "../exercise.h"
#include <numeric>
#include <iterator> // 用于 std::begin/std::end（C++11及以上可省略，但显式包含更稳妥）

// READ: `std::accumulate` <https://zh.cppreference.com/w/cpp/algorithm/accumulate>

int main(int argc, char **argv) {
    using DataType = float;
    int shape[]{1, 3, 224, 224};
    // TODO: 调用 `std::accumulate` 计算：
    //       - 数据类型为 float；
    //       - 形状为 shape；
    //       - 连续存储；
    //       的张量占用的字节数
    int size = std::accumulate(
        std::begin(shape), std::end(shape), 1,
        [](int acc, int dim) { return acc * dim; }
    ) * sizeof(DataType);
    
    ASSERT(size == 602112, "4x1x3x224x224 = 602112");
    return 0;
}