#include <catch2/catch_amalgamated.hpp>

#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE("Zigzag lines", "[zigzag-line-drawing]")
{
	Surface surface(640, 480);
	surface.clear();


	draw_line_solid(surface,
		{ 10.f, 10.f }, { 10.f, 140.f },
		{ 255, 255, 0 }
	);
	draw_line_solid(surface,
		{ 140.f, 140.f }, { 10.f, 10.f },
		{ 255, 255, 0 }
	);
	draw_line_solid(surface,
		{ 140.f, 10.f }, { 140.f, 140.f },
		{ 255, 255, 0 }
	);
	draw_line_solid(surface,
		{ 280.f, 140.f }, { 140.f, 10.f },
		{ 255, 255, 0 }
	);
	draw_line_solid(surface,
		{ 280.f, 10.f }, { 280.f, 140.f },
		{ 255, 255, 0 }
	);

	auto const counts = count_pixel_neighbours(surface);
	REQUIRE(counts[0] == 0);


}
