#pragma once

#include "EnumMap.hh"
#include "Gem.hh"
#include "PlayField.hh"
#include "SDLwrap.hh"
#include "Xoshiro256ss.hh"

#include <SDL3/SDL.h>
#include <magic_enum.hpp>

class Game {
private:
    int _window_width = 1024;
    int _window_height = 768;
    sdl::State const& _state;
    sdl::Window _window;
    sdl::Renderer _renderer;
    EnumMap<Gem::Color, sdl::SizedTexture> _gem_textures;
    Xoshiro256ss _rng;
    PlayField _field;

public:
    explicit Game(sdl::State const& lib);

    void handleEvent(SDL_Event e);
    void update(Uint64 dt_ns);
    void draw();
};
