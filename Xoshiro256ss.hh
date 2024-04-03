#pragma once

#include <array>
#include <bit>
#include <concepts>
#include <cstdint>
#include <range/v3/algorithm/generate.hpp>

class Xoshiro256ss {
    using State = std::array<std::uint64_t, 4>;
    State _state;

    static constexpr auto splitmix64(std::uint64_t seed) -> State {
        auto tmp = seed;
        State out;
        ranges::generate(out, [&tmp] {
            tmp += 0x9E3779B97F4A7C15;
            auto z = tmp;
            z = (z xor (z >> 30)) * 0xBF58476D1CE4E5B9;
            z = (z xor (z >> 27)) * 0x94D049BB133111EB;
            return z xor (z >> 31);
        });
        return out;
    }

public:
    constexpr Xoshiro256ss() : Xoshiro256ss(0) {}

    explicit constexpr Xoshiro256ss(std::uint64_t seed) : _state{splitmix64(seed)} {}

    using result_type = std::uint64_t;

    static constexpr auto min() { return std::numeric_limits<result_type>::min(); }

    static constexpr auto max() { return std::numeric_limits<result_type>::max(); }

    constexpr auto operator()() -> result_type {
        auto const result = std::rotl(_state[1] * 5, 7) * 9;
        auto const t = _state[1] << 17;

        _state[2] xor_eq _state[0];
        _state[3] xor_eq _state[1];
        _state[1] xor_eq _state[2];
        _state[0] xor_eq _state[3];

        _state[2] xor_eq t;

        _state[3] = std::rotl(_state[3], 45);

        return result;
    }
};

static_assert(std::uniform_random_bit_generator<Xoshiro256ss>);
