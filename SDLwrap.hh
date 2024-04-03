#pragma once

#include <SDL3/SDL.h>
#include <memory>

namespace sdl {
    class State {
    private:
        bool _valid = true;

    public:
        State() { SDL_Init(0); }

        ~State() {
            if (_valid) SDL_Quit();
        }

        State(State const&) = delete;

        State& operator=(State const&) = delete;

        State(State&& o) noexcept {
            if (&o != this) {
                _valid = o._valid;
                o._valid = false;
            }
        }

        State& operator=(State&& o) noexcept {
            if (&o != this) {
                _valid = o._valid;
                o._valid = false;
            }
            return *this;
        }
    };

    namespace _detail {
        template <typename T, auto F>
        struct Deleter {
            void operator()(T* p) const { F(p); }
        };

        using WindowDeleter = Deleter<SDL_Window, SDL_DestroyWindow>;
        using RendererDeleter = Deleter<SDL_Renderer, SDL_DestroyRenderer>;
        using TextureDeleter = Deleter<SDL_Texture, SDL_DestroyTexture>;
    }// namespace _detail

    using Window = std::unique_ptr<SDL_Window, _detail::WindowDeleter>;
    using Renderer = std::unique_ptr<SDL_Renderer, _detail::RendererDeleter>;
    using Texture = std::unique_ptr<SDL_Texture, _detail::TextureDeleter>;

    struct SizedTexture {
        Texture texture;
        int width = 0;
        int height = 0;

        SizedTexture() = default;
        SizedTexture(Texture&& t, int w, int h) : texture(std::move(t)), width(w), height(h) {}
        SizedTexture(
                SDL_Renderer* renderer, SDL_PixelFormatEnum format, SDL_TextureAccess access, int w,
                int h
        )
            : texture(SDL_CreateTexture(renderer, format, access, w, h)), width(w), height(h) {}
    };
}// namespace sdl
