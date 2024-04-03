#include "Game.hh"

#include "qoi_texture.hh"

#include <filesystem>
#include <random>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/generate.hpp>
#include <range/v3/algorithm/transform.hpp>
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
    ranges::generate(_field.viewGems(), [this] { return Gem::randomColor(_rng); });
}

void Game::update(Uint64 dt_ns) {}

void Game::draw() {
    SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(_renderer.get());

    // TODO: center grid
    // TODO: scale grid dynamically
    ranges::for_each(_field.indices(), [this](auto index) {
        auto [r, c] = index;
        auto color = _field.gems(r, c);
        constexpr auto scale_factor = 6;
        auto const& tex = _gem_textures[color];
        auto x = static_cast<float>(c * tex.width * scale_factor);
        auto y = static_cast<float>(r * tex.height * scale_factor);
        auto dst = SDL_FRect{
                x, y, static_cast<float>(tex.width * scale_factor),
                static_cast<float>(tex.height * scale_factor)
        };
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
