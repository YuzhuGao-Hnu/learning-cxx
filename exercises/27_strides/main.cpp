#include "../exercise.h"
#include <vector>
#include <iterator>

using udim = unsigned int;

std::vector<udim> strides(std::vector<udim> const &shape) {
    std::vector<udim> res(shape.size());
    if (shape.empty()) {
        return res;
    }

    udim current_stride = 1;
    // 分开声明两个迭代器（解决类型不一致的问题）
    std::vector<udim>::const_reverse_iterator s_it = shape.rbegin();
    std::vector<udim>::reverse_iterator st_it = res.rbegin();
    
    while (s_it != shape.rend() && st_it != res.rend()) {
        *st_it = current_stride;
        current_stride *= *s_it;
        ++s_it;
        ++st_it;
    }

    return res;
}

int main(int argc, char **argv) {
    ASSERT((strides({2, 3, 4}) == std::vector<udim>{12, 4, 1}), "Make this assertion pass.");
    ASSERT((strides({3, 4, 5}) == std::vector<udim>{20, 5, 1}), "Make this assertion pass.");
    ASSERT((strides({1, 3, 224, 224}) == std::vector<udim>{150528, 50176, 224, 1}), "Make this assertion pass.");
    ASSERT((strides({7, 1, 1, 1, 5}) == std::vector<udim>{5, 5, 5, 5, 1}), "Make this assertion pass.");
    return 0;
}