#pragma once

#include "EnumMap.hh"

#include <cstddef>

struct MatchData {
    enum Direction { Horizontal, Vertical };

    EnumMap<Direction, std::size_t> lengths{};
};
