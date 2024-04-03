#pragma once

#include <experimental/mdspan>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>

namespace mdspan_ext {
    namespace _detail {
        template <typename T, typename Extents, std::size_t... I>
        constexpr auto
        extentsProduct(std::experimental::mdspan<T, Extents> const& span, std::index_sequence<I...>) {
            return ranges::views::cartesian_product(
                           ranges::views::iota(std::size_t{0}, span.extent(I))...
                   ) |
                    ranges::views::transform([](auto&& tuple) {
                        return std::apply(
                                [](auto... indices) { return std::array{indices...}; }, tuple
                        );
                    });
        }
    }  // namespace _detail

    template <typename T, typename Extents>
    constexpr auto indices(std::experimental::mdspan<T, Extents> const& span) {
        return _detail::extentsProduct(span, std::make_index_sequence<Extents::rank()>{});
    }
}  // namespace mdspan_ext
