#include "Game.hh"
#include "Xoshiro256ss.hh"
#include "spdlog/pattern_formatter.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <memory>
#include <random>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

constexpr auto LOG_LEVEL =
#ifdef NDEBUG
        spdlog::level::info
#else
        spdlog::level::debug
#endif  // NDEBUG
        ;

int main(int /*argc*/, char** /*argv*/) {
    spdlog::set_default_logger(spdlog::stderr_color_mt("console"));
    spdlog::set_formatter(
            std::make_unique<spdlog::pattern_formatter>("[%Y-%m-%d %H:%M:%S]%^[%l]%$ %v")
    );
    spdlog::set_level(LOG_LEVEL);
    spdlog::info("init");
    auto sdl_lib = sdl::State{};
    auto g = Game(sdl_lib);

    auto running = true;
    auto last = SDL_GetTicksNS();
    while (running) {
        {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                auto handled = false;
                switch (e.type) {
                    case SDL_EVENT_QUIT:
                        running = false;
                        handled = true;
                        break;
                    case SDL_EVENT_KEY_DOWN:
                        switch (e.key.keysym.sym) {
                            case SDLK_ESCAPE:
                                running = false;
                                handled = true;
                                break;
                            default:
                                break;
                        }
                    default:
                        break;
                }

                if (!handled) {
                    g.handleEvent(e);
                }
            }
        }

        auto now = SDL_GetTicksNS();
        auto dt_ns = now - last;
        g.update(dt_ns);
        g.draw();
        last = SDL_GetTicksNS();
    }

    SDL_Quit();
    return 0;
}
