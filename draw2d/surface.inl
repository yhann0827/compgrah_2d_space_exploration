/* Inline functions need to have a definition that is visible to the compiler
 * whenever the function is used. THey could be define in the header that
 * declares the function. However, to keep the definitions and declarations
 * somewhat apart, it is a common practice to move them to an "inline" file
 * such as this one (*.inl extension). This file is then #include:ed at the end
 * of the header, to ensure that whoever includes the header also automatically
 * includes the inline file.
 *
 * Inlining allows us to avoid any overheads related to call (when building in
 * "release mode" / with optimizations enabled). This makes functions like
 * set_pixel_srgb() zero overhead abstractions.
 *
 */

inline void Surface::set_pixel_srgb(Index aX, Index aY, ColorU8_sRGB const& aColor) {
    assert(aX < mWidth && aY < mHeight);

    Index linearIndex = get_linear_index(aX, aY);

    mSurface[linearIndex] = aColor.r; 
    mSurface[linearIndex + 1] = aColor.g;
    mSurface[linearIndex + 2] = aColor.b; 
    mSurface[linearIndex + 3] = 0;
}


inline 
auto Surface::get_width() const noexcept -> Index
{
	return mWidth;
}
inline
auto Surface::get_height() const noexcept -> Index
{
	return mHeight;
}

inline
Surface::Index Surface::get_linear_index(Index aX, Index aY) const noexcept
{

    Surface::Index linearIndex = (aY * mWidth + aX) * 4;

    return linearIndex;

}
