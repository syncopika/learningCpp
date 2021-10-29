#include "Vec3.h"
#include "color.h"
#include "ray.h"
#include <iostream>

Color ray_color(const Ray& r) {
    Vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5f*(unit_direction.y() + 1.0f);
    return (1.0f - t)*Color(1.0f, 1.0f, 1.0f) + t*Color(0.5f, 0.7f, 1.0f);
}

int main(void) {

    // image
    const auto aspect_ratio = 16.0f / 9.0f;
    const int width = 400;
    const int height = static_cast<int>(width / aspect_ratio);
    
    // camera
    auto viewport_height = 2.0f;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;
    
    auto origin = Point3(0, 0, 0);
    auto horizontal = Vec3(viewport_width, 0, 0);
    auto vertical = Vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - Vec3(0, 0, focal_length);
    
    std::cout << "P3\n" << width << ' ' << height << "\n255\n";
    
    for(int i = height-1; i >= 0; i--){
        std::cerr << "\rScanlines remaining: " << i << '\n';
        for(int j = 0; j < width; j++){
            auto u = (double)j / (width - 1);
            auto v = (double)i / (height - 1);
            Ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            Color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    
    std::cerr << "\nDone!\n";
    
    return 0;
}