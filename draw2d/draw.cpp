#include "draw.hpp"

#include <algorithm>

#include <cmath>

#include "surface.hpp"


/*
 * This code implements the Cohen-Sutherland Line Clipping Algorithm for clipping line segments.
 * source - https://www.geeksforgeeks.org/line-clipping-set-1-cohen-sutherland-algorithm
 * 
 * DDA algorithm is implemented in draw_line_solid function to draw the line
 *  Source - https://www.geeksforgeeks.org/dda-line-generation-algorithm-computer-graphics/
 * I read and understand the algorihtm thoroughly before implementing it in my code using my own way
 *
 */

void draw_line_solid(Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor)
{
    const float xMin = 0;
    const float xMax = aSurface.get_width();
    const float yMin = 0;
    const float yMax = aSurface.get_height();

    const int LEFT = 1;   
    const int RIGHT = 2;  
    const int BOTTOM = 4; 
    const int TOP = 8;    
    const int INSIDE = 0;      

    int code1 = INSIDE;
    int code2 = INSIDE;

    if (aBegin.x < xMin || aEnd.x < xMin) {
        code1 |= LEFT;
        code2 |= LEFT;
    }
    if (aBegin.x > xMax || aEnd.x > xMax) {
        code1 |= RIGHT;
        code2 |= RIGHT;
    }
    if (aBegin.y < yMin || aEnd.y < yMin) {
        code1 |= BOTTOM;
        code2 |= BOTTOM;
    }
    if (aBegin.y > yMax || aEnd.y > yMax) {
        code1 |= TOP;
        code2 |= TOP;
    }

    bool accept = true;

    while (true) {
        if ((code1 | code2) == 0) {
            accept = true;
            break;
        }
        else if (code1 & code2) {
            break;
        }
        else {
            int x, y;

            if (code1) {
                x = aBegin.x;
                y = aBegin.y;
            }
            else {
                x = aEnd.x;
                y = aEnd.y;
            }

            if (code1 & TOP) {
                x = x + (aEnd.x - aBegin.x) * (yMax - y) / (aEnd.y - aBegin.y);
                y = yMax;
            }
            else if (code1 & BOTTOM) {
                x = x + (aEnd.x - aBegin.x) * (yMin - y) / (aEnd.y - aBegin.y);
                y = yMin;
            }
            else if (code1 & RIGHT) {
                y = y + (aEnd.y - aBegin.y) * (xMax - x) / (aEnd.x - aBegin.x);
                x = xMax;
            }
            else if (code1 & LEFT) {
                y = y + (aEnd.y - aBegin.y) * (xMin - x) / (aEnd.x - aBegin.x);
                x = xMin;
            }

            if (code1) {
                aBegin.x = x;
                aBegin.y = y;
                code1 = INSIDE;

                if (aBegin.x < xMin) code1 |= LEFT;
                if (aBegin.x > xMax) code1 |= RIGHT;
                if (aBegin.y < yMin) code1 |= BOTTOM;
                if (aBegin.y > yMax) code1 |= TOP;
            }
            else {
                aEnd.x = x;
                aEnd.y = y;
                code2 = INSIDE;

                if (aEnd.x < xMin) code2 |= LEFT;
                if (aEnd.x > xMax) code2 |= RIGHT;
                if (aEnd.y < yMin) code2 |= BOTTOM;
                if (aEnd.y > yMax) code2 |= TOP;
            }
        }
    }

    if (accept) {
        float dx = aEnd.x - aBegin.x;
        float dy = aEnd.y - aBegin.y;

        int steps;

        if (abs(dx) > abs(dy)) {
            steps = static_cast<int>(abs(dx));
        }
        else {
            steps = static_cast<int>(abs(dy));
        }

        float xIncrement = dx / steps;
        float yIncrement = dy / steps;

        float x = aBegin.x;
        float y = aBegin.y;

        for (int i = 0; i <= steps; ++i) {
            if (x >= 0 && x < aSurface.get_width() && y >= 0 && y < aSurface.get_height()) {
                aSurface.set_pixel_srgb(static_cast<int>(round(x)), static_cast<int>(round(y)), aColor);
            }

            x = x + xIncrement;
            y = y + yIncrement;
        }
    }
}



void draw_triangle_wireframe( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aP0;   // is properly implemented.
	(void)aP1;
	(void)aP2;
	(void)aColor;
}

/*
 * This code implements the bounding-box concept for optimisation.
 * source - https://www.geeksforgeeks.org/how-to-get-bounding-box-of-different-shapes-in-p5-js/
 *
 * Related research about triangle drawing is conducted before implementing the draw_triangle_solid and draw_triangle_interp function.
 *  Source - http://courses.cms.caltech.edu/cs171/assignments/hw2/hw2-notes/notes-hw2.html#NotesSection1.1
 * 
 * I understand the logic of triangle drawing and concept of barycentric through lecture 7 before writing the code.
 * I read and understand the algorihtm thoroughly before implementing it in my code using my own
 *
 */
void draw_triangle_solid(Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor) {
	float crossProduct = (aP1.x - aP0.x) * (aP2.y - aP0.y) - (aP2.x - aP0.x) * (aP1.y - aP0.y);
	if (std::abs(crossProduct) < 1e-6) {
		return;
	}

	for (int y = static_cast<int>(std::min({ aP0.y, aP1.y, aP2.y })); y <= static_cast<int>(std::max({ aP0.y, aP1.y, aP2.y })); y++) {
		for (int x = static_cast<int>(std::min({ aP0.x, aP1.x, aP2.x })); x <= static_cast<int>(std::max({ aP0.x, aP1.x, aP2.x })); x++) {
			if (x >= 0 && static_cast<Surface::Index>(x) < aSurface.get_width() && y >= 0 && static_cast<Surface::Index>(y) < aSurface.get_height())
			{
				float alpha = ((aP1.y - aP2.y) * (x - aP2.x) + (aP2.x - aP1.x) * (y - aP2.y)) /
					((aP1.y - aP2.y) * (aP0.x - aP2.x) + (aP2.x - aP1.x) * (aP0.y - aP2.y));

				float beta = ((aP2.y - aP0.y) * (x - aP2.x) + (aP0.x - aP2.x) * (y - aP2.y)) /
					((aP1.y - aP2.y) * (aP0.x - aP2.x) + (aP2.x - aP1.x) * (aP0.y - aP2.y));

				float gamma = static_cast<float>(1.0 - alpha - beta);

				if (alpha >= 0.0 && beta >= 0.0 && gamma >= 0.0) {
					aSurface.set_pixel_srgb(x, y, aColor);
				}
			}
		}
	}
}

/*
* I studied about the code and concept of interpolation using Barycentric Coordinates using the website below before implementing it in the function.
* Source - https://observablehq.com/@jobleonard/linear-interpolation-along-a-triangle-with-barycentric-co
*/

void draw_triangle_interp(Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2) {
	float crossProduct = (aP1.x - aP0.x) * (aP2.y - aP0.y) - (aP2.x - aP0.x) * (aP1.y - aP0.y);
	if (std::abs(crossProduct) < 1e-6) {
		return;
	}

	for (int y = static_cast<int>(std::min({ aP0.y, aP1.y, aP2.y })); y <= static_cast<int>(std::max({ aP0.y, aP1.y, aP2.y })); y++) {
		for (int x = static_cast<int>(std::min({ aP0.x, aP1.x, aP2.x })); x <= static_cast<int>(std::max({ aP0.x, aP1.x, aP2.x })); x++) {
			if (x >= 0 && static_cast<Surface::Index>(x) < aSurface.get_width() && y >= 0 && static_cast<Surface::Index>(y) < aSurface.get_height())
			{
				float alpha = ((aP1.y - aP2.y) * (x - aP2.x) + (aP2.x - aP1.x) * (y - aP2.y)) /
					((aP1.y - aP2.y) * (aP0.x - aP2.x) + (aP2.x - aP1.x) * (aP0.y - aP2.y));

				float beta = ((aP2.y - aP0.y) * (x - aP2.x) + (aP0.x - aP2.x) * (y - aP2.y)) /
					((aP1.y - aP2.y) * (aP0.x - aP2.x) + (aP2.x - aP1.x) * (aP0.y - aP2.y));

				float gamma = 1.0 - alpha - beta;

				if (alpha >= 0.0 && beta >= 0.0 && gamma >= 0.0) {
					ColorF pixelColor;
					pixelColor.r = aC0.r * alpha + aC1.r * beta + aC2.r * gamma;
					pixelColor.g = aC0.g * alpha + aC1.g * beta + aC2.g * gamma;
					pixelColor.b = aC0.b * alpha + aC1.b * beta + aC2.b * gamma;
					aSurface.set_pixel_srgb(x, y, linear_to_srgb(pixelColor));
				}
			}
		}
	}
}


void draw_rectangle_solid( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aMinCorner;   // is properly implemented.
	(void)aMaxCorner;
	(void)aColor;
}

void draw_rectangle_outline( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments
	(void)aMinCorner;
	(void)aMaxCorner;
	(void)aColor;
}

/*
 * References
 * Anon n.d. Amazon.com. [Online]. [Accessed 31 October 2023b]. Available from: https://aws.amazon.com/what-is/batch-processing/#:~:text=Batch%20processing%20is%20the%20method,run%20on%20individual%20data%20transactions.
 * Anon 2019a. Comparisons between DDA and Bresenham Line Drawing algorithm. GeeksforGeeks. [Online]. [Accessed 29 October 2023]. Available from: https://www.geeksforgeeks.org/comparions-between-dda-and-bresenham-line-drawing-algorithm/.
 * Anon 2017. DDA Line generation Algorithm in Computer Graphics. GeeksforGeeks. [Online]. [Accessed 29 October 2023]. Available from: https://www.geeksforgeeks.org/dda-line-generation-algorithm-computer-graphics/.
 * Anon 2019b. How to get bounding-box of different Shapes in p5.js ? GeeksforGeeks. [Online]. [Accessed 29 October 2023]. Available from: https://www.geeksforgeeks.org/how-to-get-bounding-box-of-different-shapes-in-p5-js/.
 * Anon 2017b. Introduction to Computer Graphics. Caltech.edu. [Online]. [Accessed 2 November 2023]. Available from: http://courses.cms.caltech.edu/cs171/assignments/hw2/hw2-notes/notes-hw2.html.
 * Anon 2016. Line clipping. GeeksforGeeks. [Online]. [Accessed 31 October 2023]. Available from: https://www.geeksforgeeks.org/line-clipping-set-1-cohen-sutherland-algorithm/.
 * van der Zwan, J. 2020. Linear interpolation along a triangle with barycentric coordinates. Observable. [Online]. [Accessed 6 November 2023]. Available from: https://observablehq.com/@jobleonard/linear-interpolation-along-a-triangle-with-barycentric-co 
 */