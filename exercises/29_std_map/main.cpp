#include "../exercise.h"
#include <map>
#include <string>  // 用于 std::string_literals
#include <utility> // 可选：用于 std::move 优化

// READ: `std::map` <https://zh.cppreference.com/w/cpp/container/map>
// READ: `std::unordered_map` <https://zh.cppreference.com/w/cpp/container/unordered_map>

template<class k, class v>
bool key_exists(std::map<k, v> const &map, k const &key) {
    // 查找键：const map 用 find 返回 const_iterator
    auto it = map.find(key);
    // 找到则迭代器 != end()，否则不存在
    return it != map.end();
}

template<class k, class v>
void set(std::map<k, v> &map, k key, v value) {
    // std::map[] 特性：键存在则修改值，不存在则插入新键值对
    // 用 std::move 减少拷贝（可选，基础写法可直接 map[key] = value）
    map[std::move(key)] = std::move(value);
}

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    using namespace std::string_literals;

    std::map<std::string, std::string> secrets;

    set(secrets, "hello"s, "world"s);
    ASSERT(key_exists(secrets, "hello"s), "\"hello\" shoud be in `secrets`");
    ASSERT(!key_exists(secrets, "foo"s), "\"foo\" shoud not be in `secrets`");

    set(secrets, "foo"s, "bar"s);
    set(secrets, "Infini"s, "Tensor"s);
    ASSERT(secrets["hello"] == "world", "hello -> world");
    ASSERT(secrets["foo"] == "bar", "foo -> bar");
    ASSERT(secrets["Infini"] == "Tensor", "Infini -> Tensor");

    set(secrets, "hello"s, "developer"s);
    ASSERT(secrets["hello"] == "developer", "hello -> developer");

    return 0;
}