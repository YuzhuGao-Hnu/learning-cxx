#include "../exercise.h"
#include <cstring>

// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>
template<class T>
struct Tensor4D {
    unsigned int shape[4];
    T *data;

    Tensor4D(unsigned int const shape_[4], T const *data_) {
        // 1. 拷贝输入的形状到当前Tensor的shape数组
        std::memcpy(shape, shape_, 4 * sizeof(unsigned int));
        // 2. 计算总元素数：4个维度的乘积
        unsigned int size = 1;
        for (int i = 0; i < 4; ++i) {
            size *= shape[i];
        }
        // 3. 分配内存并拷贝数据
        data = new T[size];
        std::memcpy(data, data_, size * sizeof(T));
    }

    ~Tensor4D() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 单向广播加法实现
    Tensor4D &operator+=(Tensor4D const &others) {
        // 第一步：验证广播形状合法性（others维度必须为1或与当前维度相等）
        for (int dim = 0; dim < 4; ++dim) {
            ASSERT(others.shape[dim] == 1 || others.shape[dim] == shape[dim],
                   "Broadcast shape mismatch: dim %d, this=%u, others=%u",
                   dim, shape[dim], others.shape[dim]);
        }

        // 预计算当前Tensor的维度步长（用于4维索引转扁平索引）
        unsigned int step0 = shape[1] * shape[2] * shape[3]; // 维度0的步长
        unsigned int step1 = shape[2] * shape[3];            // 维度1的步长
        unsigned int step2 = shape[3];                      // 维度2的步长
        unsigned int step3 = 1;                             // 维度3的步长

        // 预计算others Tensor的维度步长
        unsigned int o_step0 = others.shape[1] * others.shape[2] * others.shape[3];
        unsigned int o_step1 = others.shape[2] * others.shape[3];
        unsigned int o_step2 = others.shape[3];
        unsigned int o_step3 = 1;

        // 第二步：遍历当前Tensor的所有4维索引，执行广播加法
        for (unsigned int d0 = 0; d0 < shape[0]; ++d0) { // 维度0遍历
            for (unsigned int d1 = 0; d1 < shape[1]; ++d1) { // 维度1遍历
                for (unsigned int d2 = 0; d2 < shape[2]; ++d2) { // 维度2遍历
                    for (unsigned int d3 = 0; d3 < shape[3]; ++d3) { // 维度3遍历
                        // 计算当前元素的扁平索引（一维数组的位置）
                        unsigned int self_idx = d0 * step0 + d1 * step1 + d2 * step2 + d3 * step3;
                        
                        // 计算others的对应索引（广播维度取0）
                        unsigned int o_d0 = (others.shape[0] == 1) ? 0 : d0;
                        unsigned int o_d1 = (others.shape[1] == 1) ? 0 : d1;
                        unsigned int o_d2 = (others.shape[2] == 1) ? 0 : d2;
                        unsigned int o_d3 = (others.shape[3] == 1) ? 0 : d3;
                        unsigned int other_idx = o_d0 * o_step0 + o_d1 * o_step1 + o_d2 * o_step2 + o_d3 * o_step3;
                        
                        // 执行广播加法
                        data[self_idx] += others.data[other_idx];
                    }
                }
            }
        }

        return *this;
    }
};
// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        auto t0 = Tensor4D(shape, data);
        auto t1 = Tensor4D(shape, data);
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
}
