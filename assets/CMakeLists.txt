include(CMakeRC)

cmrc_add_resource_library(match3-assets
    ALIAS match3::assets
    NAMESPACE assets

    sprites/gems/Blue.qoi
    sprites/gems/Green.qoi
    sprites/gems/Orange.qoi
    sprites/gems/Purple.qoi
    sprites/gems/Red.qoi
    sprites/gems/White.qoi
    sprites/gems/Yellow.qoi
)
