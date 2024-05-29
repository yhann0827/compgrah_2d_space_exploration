#include "image.hpp"

#include <memory>
#include <algorithm>

#include <cstdio>
#include <cstring>
#include <cassert>

#include <stb_image.h>

#include "surface.hpp"

#include "../support/error.hpp"

namespace
{
	struct STBImageRGBA_ : public ImageRGBA
	{
		STBImageRGBA_( Index, Index, std::uint8_t* );
		virtual ~STBImageRGBA_();
	};
}

ImageRGBA::ImageRGBA()
	: mWidth( 0 )
	, mHeight( 0 )
	, mData( nullptr )
{}

ImageRGBA::~ImageRGBA() = default;


std::unique_ptr<ImageRGBA> load_image( char const* aPath )
{
	assert( aPath );

	stbi_set_flip_vertically_on_load( true );

	int w, h, channels;
	stbi_uc* ptr = stbi_load( aPath, &w, &h, &channels, 4 );
	if( !ptr )
		throw Error( "Unable to load image \"%s\"", aPath );

	return std::make_unique<STBImageRGBA_>(
		ImageRGBA::Index(w),
		ImageRGBA::Index(h),
		ptr
	);
}

/* 
 * I implemented this function after reading the materials from Lecture 7 and lecture 8 of the course modules.
 */
void blit_masked(Surface& aSurface, ImageRGBA const& aImage, Vec2f aPosition) {

	for (ImageRGBA::Index y = 0; y < aImage.get_height(); y++) {
		for (ImageRGBA::Index x = 0; x < aImage.get_width(); x++) {
			ColorU8_sRGB_Alpha sourcePixel = aImage.get_pixel(x, y);

			if (sourcePixel.a <  128) {
				continue;
			}

			Vec2f desPosition = { aPosition.x + x, aPosition.y + y };

			if (desPosition.x >= 0 && desPosition.x < aSurface.get_width() && desPosition.y >= 0 && desPosition.y < aSurface.get_height()) {
				aSurface.set_pixel_srgb(
					static_cast<Surface::Index>(desPosition.x),
					static_cast<Surface::Index>(desPosition.y),
					ColorU8_sRGB{ sourcePixel.r, sourcePixel.g, sourcePixel.b }
				);
			}
		}
	}
}


namespace
{
	STBImageRGBA_::STBImageRGBA_( Index aWidth, Index aHeight, std::uint8_t* aPtr )
	{
		mWidth = aWidth;
		mHeight = aHeight;
		mData = aPtr;
	}

	STBImageRGBA_::~STBImageRGBA_()
	{
		if( mData )
			stbi_image_free( mData );
	}
}

/*
* References
* Anon 2023. More Images Part 2. Comp3811: Computer Graphics[Lecture]. University of Leeds.
*Anon 2023. Triangle and More Images. Comp3811: Computer Graphics[Lecture]. University of Leeds.
*/