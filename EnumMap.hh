#pragma once

#include <concepts>
#include <magic_enum.hpp>

template <typename E, typename V>
    requires std::is_default_constructible_v<V> and std::is_enum_v<E>
class EnumMap {
private:
    static_assert(magic_enum::enum_count<E>() > 0, "Enum must have at least one value");
    using Data = std::array<V, magic_enum::enum_count<E>()>;
    Data _data{};

public:
    EnumMap() = default;

    V& operator[](E key) { return _data[magic_enum::enum_integer(key)]; }

    using iterator = typename Data::iterator;
    iterator begin() { return _data.begin(); }
    iterator end() { return _data.end(); }
};
