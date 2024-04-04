#include "Game.hh"

#include "qoi_texture.hh"

#include <filesystem>
#include <random>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/generate.hpp>
#include <range/v3/algorithm/transform.hpp>
#include <range/v3/iterator/operations.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/take_while.hpp>
#include <utility>

Game::Game(sdl::State const& lib)
    : _state(lib), _window(SDL_CreateWindow(
                           "Match 3", _window_width, _window_height,
                           SDL_WINDOW_RESIZABLE bitor SDL_WINDOW_MAXIMIZED
                   )),
      _renderer(SDL_CreateRenderer(_window.get(), nullptr, SDL_RENDERER_PRESENTVSYNC)), _rng([] {
          auto rd = std::random_device{};
          auto eng = std::default_random_engine{rd()};
          auto initial_dist = std::uniform_int_distribution<std::uint64_t>{};
          return Xoshiro256ss{initial_dist(eng)};
      }()),
      _field(8, 8) {
    ranges::transform(
            magic_enum::enum_entries<Gem::Color>(), _gem_textures.begin(),
            [this](auto pair) -> sdl::SizedTexture {
                if (pair.first == Gem::Color::None) return {};

                auto name = pair.second;
                return loadTexture(
                        _renderer.get(),
                        std::filesystem::path("sprites") / "gems" / (std::string{name} + ".qoi")
                );
            }
    );
    ranges::generate(_field.viewGems(), [this] { return Gem{.color = Gem::randomColor(_rng)}; });
}

void Game::update(Uint64 dt_ns) {
    // TODO: animation system
    // TODO: detect matches

    // v/h 3
    // v/h 4 leaves bomb, AND bomb prefers to be placed where swap occurred
    // L-shape 3 leaves a cross bomb
    // v/h 5 leaves wildcard, wildcard can swap with anything and deletes all of that color
    // L-shape 4/3 leaves cross bomb and bomb

    struct GemMeta {
        bool in_match = false;
        std::array<int, 2> match_lengths = {0, 0};
    };

    ranges::for_each(_field.indices(), [this](auto index) {
        auto [r, c] = index;
        auto root = _field.gems(r, c);
        auto meta = GemMeta{};
        if (root.in_match) {
            meta.in_match = true;
            return meta;
        }
        auto horizontal_match = ranges::views::take_while(
                ranges::views::iota(c, _field.width()),
                [this, r = r, root](auto c2) { return _field.gems(r, c2).color == root.color; }
        );
        auto vertical_match = ranges::views::take_while(
                ranges::views::iota(r, _field.height()),
                [this, c = c, root](auto r2) { return _field.gems(r2, c).color == root.color; }
        );
        auto horizontal_length = ranges::distance(horizontal_match);
        if (horizontal_length >= 3) {
            ranges::for_each(ranges::views::iota(c, c + horizontal_length), [this, r = r](auto c2) {
                _field.gems(r, c2).in_match = true;
            });
        }
        auto vertical_length = ranges::distance(vertical_match);
        if (vertical_length >= 3) {
            ranges::for_each(ranges::views::iota(r, r + vertical_length), [this, c = c](auto r2) {
                _field.gems(r2, c).in_match = true;
            });
        }
        return meta;
    });
}

void Game::draw() {
    SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(_renderer.get());
    SDL_SetRenderDrawBlendMode(_renderer.get(), SDL_BLENDMODE_BLEND);

    // TODO: center grid
    // TODO: scale grid dynamically
    ranges::for_each(_field.indices(), [this](auto index) {
        auto [r, c] = index;
        auto gem = _field.gems(r, c);
        constexpr auto scale_factor = 6;
        auto const& tex = _gem_textures[gem.color];
        auto x = static_cast<float>(c * tex.width * scale_factor);
        auto y = static_cast<float>(r * tex.height * scale_factor);
        auto dst = SDL_FRect{
                x, y, static_cast<float>(tex.width * scale_factor),
                static_cast<float>(tex.height * scale_factor)
        };
        if (gem.in_match) {
            SDL_SetRenderDrawColor(_renderer.get(), 255, 0, 0, 255);
            SDL_RenderFillRect(_renderer.get(), &dst);
        }
        SDL_RenderTexture(_renderer.get(), tex.texture.get(), nullptr, &dst);
    });

    SDL_RenderPresent(_renderer.get());
}

void Game::handleEvent(SDL_Event e) {
    switch (e.type) {
        case SDL_EVENT_WINDOW_RESIZED:
            _window_width = e.window.data1;
            _window_height = e.window.data2;
            break;
        default:
            break;
    }
}
