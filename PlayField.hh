#pragma once

#include "Gem.hh"

#include <experimental/mdspan>
#include <range/v3/range/concepts.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>
#include <vector>

class PlayField {
    std::vector<Gem::Color> _gems_data;

public:
    std::experimental::mdspan<Gem::Color, std::experimental::dextents<std::size_t, 2>> gems;

    PlayField(std::size_t width, std::size_t height);

    [[nodiscard]] auto indices() const {
        return ranges::views::cartesian_product(
                ranges::views::iota(std::size_t{0}, gems.extent(0)),
                ranges::views::iota(std::size_t{0}, gems.extent(1))
        );
    }

    auto viewGems() {
        return indices() | ranges::views::transform([this](auto index) -> Gem::Color& {
                   auto [r, c] = index;
                   return this->gems(r, c);
               });
    }
};

static_assert(std::convertible_to<
              std::pair<std::size_t, std::size_t>,
              ranges::value_type_t<decltype(std::declval<PlayField const&>().indices().begin())>>);
static_assert(ranges::output_range<decltype(std::declval<PlayField&>().viewGems()), Gem::Color>);
