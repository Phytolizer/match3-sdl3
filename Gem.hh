#pragma once

#include <SDL3/SDL_pixels.h>
#include <magic_enum.hpp>
#include <random>

struct Gem {
    enum class Color {
        None,
        White,
        Red,
        Yellow,
        Green,
        Orange,
        Blue,
        Purple,
    };

    Color color = Color::None;
    bool in_match = false;

    template <typename Rng>
    static Color randomColor(Rng& rng) {
        auto dist = std::uniform_int_distribution<>{1, magic_enum::enum_count<Color>() - 1};
        return static_cast<Color>(dist(rng));
    }
};
