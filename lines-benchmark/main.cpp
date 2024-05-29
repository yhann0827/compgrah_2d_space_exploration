#include <benchmark/benchmark.h>

#include "../draw2d/draw.hpp"
#include "../draw2d/surface.hpp"

namespace
{
	// This is a placeholder. Replace this with yor own code. Refer to
	// blit-benchmark/main.cpp for a more complete example. 
	void placeholder_( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{
			// Horizontal Line
			draw_line_solid(surface, { 0.f, 0.f }, { 140.f, 0.f }, { 255, 255, 0 });

			//Diagonal Line
			draw_line_solid( surface,{ 100.f, 100.f}, { width-100.f, height-100.f }, {255, 255, 0 });

			//Line Drawn Completely Off the Screen
			draw_line_solid(surface,
					{ width + 10.f, height / 3.f }, { width + 100.f, height / 3.f },
					{ 255, 255, 0 }
			);

			//Clipping Line
			draw_line_solid(surface,
				{ width / 2.f, height / 2.f }, { width + 10.f, height / 2.f },
				{ 255, 255, 0 }
			);
			// ClobberMemory() ensures that the compiler won't optimize away
			// our draw_line_solid operation. (Unlikely, but technically possible.)
			benchmark::ClobberMemory();
		}
	}
}

BENCHMARK( placeholder_ )
	->Args({ 320, 240 })
	->Args({ 1280, 720 })
	->Args({ 1920, 1080 })
	->Args({ 7680, 4320 })
;


BENCHMARK_MAIN();
