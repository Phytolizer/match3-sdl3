#include "PlayField.hh"

PlayField::PlayField(std::size_t width, std::size_t height)
    : _gems_data(width * height, Gem::Color::None),
      gems(_gems_data.data(), std::experimental::extents{height, width}) {}
