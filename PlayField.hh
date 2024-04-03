#pragma once

#include "Gem.hh"
#include "mdspan_ext.hh"

#include <experimental/mdspan>
#include <range/v3/range/concepts.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>
#include <vector>

class PlayField {
    std::vector<Gem> _gems_data;

public:
    std::experimental::mdspan<Gem, std::experimental::dextents<std::size_t, 2>> gems;

    PlayField(std::size_t width, std::size_t height);

    [[nodiscard]] constexpr std::size_t width() const { return gems.extent(1); }

    [[nodiscard]] constexpr std::size_t height() const { return gems.extent(0); }

    [[nodiscard]] auto indices() const { return mdspan_ext::indices(gems); }

    auto viewGems() {
        return indices() | ranges::views::transform([this](auto index) -> Gem& {
                   auto [r, c] = index;
                   return this->gems(r, c);
               });
    }
};

static_assert(ranges::output_range<decltype(std::declval<PlayField&>().viewGems()), Gem>);
