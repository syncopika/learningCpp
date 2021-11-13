#ifndef COLOR_H
#define COLOR_H

#include "Vec3.h"
#include <iostream>

void write_color(std::ostream &out, Color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();
    
    // divide the color by the number of samples for anti-aliasing
    // a single pixel has multiple color contributions 
    // due to multiple rays influencing the same pixel
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;
    
	out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif