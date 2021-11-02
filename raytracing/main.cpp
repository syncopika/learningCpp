#include "common.h"

#include "color.h"
#include "HittableList.h"
#include "Sphere.h"

#include <iostream>


Color ray_color(const Ray& r, const Hittable& world) {
    HitRecord rec;
    if(world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + Color(1,1,1));
    }
    Vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0);
}

int main(void) {
    // image
    const auto aspect_ratio = 16.0 / 9.0;
    const int width = 400;
    const int height = static_cast<int>(width / aspect_ratio);
    
    // world
    HittableList world;
    world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));
    
    // camera
    auto viewport_height = 2.0;
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
            Color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }
    
    std::cerr << "\nDone!\n";
    
    return 0;
}