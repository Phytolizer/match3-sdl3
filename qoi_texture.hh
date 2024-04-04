#pragma once

#include "SDLwrap.hh"

#include <SDL3/SDL_render.h>
#include <filesystem>

auto loadTexture(SDL_Renderer* renderer, std::filesystem::path const& path) -> sdl::SizedTexture;
