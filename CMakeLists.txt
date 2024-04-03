cmake_minimum_required(VERSION 3.29)
project(match3_sdl3)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

fetchcontent_declare(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.2.1
)
fetchcontent_makeavailable(fmt)

fetchcontent_declare(sdl
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG prerelease-3.1.1
)
fetchcontent_makeavailable(sdl)

fetchcontent_declare(magic_enum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
    GIT_TAG v0.9.5
)
fetchcontent_makeavailable(magic_enum)

fetchcontent_declare(range-v3
    GIT_REPOSITORY https://github.com/ericniebler/range-v3.git
    GIT_TAG 0.12.0
)
fetchcontent_makeavailable(range-v3)

fetchcontent_declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.13.0
)
fetchcontent_makeavailable(spdlog)

fetchcontent_declare(cmrc
    URL https://raw.githubusercontent.com/vector-of-bool/cmrc/952ffddba731fc110bd50409e8d2b8a06abbd237/CMakeRC.cmake
    DOWNLOAD_NO_EXTRACT TRUE
)
fetchcontent_makeavailable(cmrc)

list(APPEND CMAKE_MODULE_PATH ${cmrc_SOURCE_DIR})

fetchcontent_declare(mdspan
    GIT_REPOSITORY https://github.com/kokkos/mdspan.git
    GIT_TAG mdspan-0.6.0
)
fetchcontent_makeavailable(mdspan)

fetchcontent_declare(qoi_h
    URL https://github.com/phoboslab/qoi/raw/master/qoi.h
    DOWNLOAD_NO_EXTRACT TRUE
)
fetchcontent_makeavailable(qoi_h)

file(GENERATE
    OUTPUT qoi.c
    CONTENT "#define QOI_IMPLEMENTATION\n#include \"qoi.h\"\n"
)

add_library(qoi qoi.c)
target_include_directories(qoi PUBLIC ${qoi_h_SOURCE_DIR})
add_library(qoi::qoi ALIAS qoi)

add_subdirectory(assets)

add_executable(match3 main.cc)
target_sources(match3 PRIVATE
    FILE_SET HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
    FILES
    qoi_texture.hh
    EnumMap.hh
    Game.hh
    Gem.hh
    PlayField.hh
    SDLwrap.hh
    Xoshiro256ss.hh
)
target_sources(match3 PRIVATE
    qoi_texture.cc
    Game.cc
    PlayField.cc
)
target_link_libraries(match3 PRIVATE
    qoi::qoi
    match3::assets
    SDL3::SDL3
    magic_enum::magic_enum
    fmt::fmt
    spdlog::spdlog
    range-v3::range-v3
    std::mdspan
)
