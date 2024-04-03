#include "qoi_texture.hh"

#include <cmrc/cmrc.hpp>
#include <fstream>
#include <range/v3/algorithm/copy.hpp>
#include <span>
#include <spdlog/spdlog.h>
#include <vector>

CMRC_DECLARE(assets);

auto loadTexture(SDL_Renderer* renderer, std::filesystem::path const& path) -> sdl::SizedTexture {
    spdlog::info("load {}", path.string());
    auto fs = cmrc::assets::get_filesystem();
    auto qoi_data = std::vector<char>{};
    auto file = fs.open(path);
    qoi_data.resize(file.size());
    ranges::copy(file, qoi_data.begin());
    auto desc = qoi_desc{};
    void* raw_pixels = qoi_decode(qoi_data.data(), static_cast<int>(qoi_data.size()), &desc, 4);
    auto* texture = SDL_CreateTexture(
            renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC,
            static_cast<int>(desc.width), static_cast<int>(desc.height)
    );
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_UpdateTexture(texture, nullptr, raw_pixels, static_cast<int>(desc.width * 4));
    std::free(raw_pixels);
    return {sdl::Texture{texture}, static_cast<int>(desc.width), static_cast<int>(desc.height)};
}
