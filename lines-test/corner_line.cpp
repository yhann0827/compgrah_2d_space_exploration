#include <catch2/catch_amalgamated.hpp>

#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE("Corner Line", "[corner]")
{
	Surface surface(640, 480);
	surface.clear();


	draw_line_solid(surface,
		{ 0.f, 00.f },
		{ 140.f, 0.f },
		{ 255, 255, 255 }
	);

	draw_line_solid(surface,
		{ 0.f, 0.f },
		{ 0.f, 140.f },
		{ 255, 255, 255 }
	);

	auto const counts = count_pixel_neighbours(surface);

	REQUIRE(counts[0] == 0);


}
