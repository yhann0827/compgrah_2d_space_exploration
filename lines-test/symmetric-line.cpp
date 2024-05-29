#include <catch2/catch_amalgamated.hpp>

#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE("Symmetric lines", "[symmetric-line-drawing]")
{
    Surface surface(640, 480);
    surface.clear();

    draw_line_solid(surface,
        { 115.f, 120.f }, { 172.5f, 30.f },
        { 255, 255, 0 }
    );
    draw_line_solid(surface,
        { 172.5f, 30.f }, { 230.f, 120.f },
        { 255, 255, 0 }
    );

    Surface mirroredSurface(640, 480);
    mirroredSurface.clear(); 

    draw_line_solid(mirroredSurface,
        { 230.f, 120.f }, { 172.5f, 210.f },
        { 255, 255, 0 }
    );
    draw_line_solid(mirroredSurface,
        { 172.5f, 210.f }, { 115.f, 120.f },
        { 255, 255, 0 }
    );

    std::size_t originalPixels = max_col_pixel_count(surface) + max_row_pixel_count(surface);
    std::size_t mirroredPixels = max_col_pixel_count(mirroredSurface) + max_row_pixel_count(mirroredSurface);

    REQUIRE(originalPixels == mirroredPixels);
}
