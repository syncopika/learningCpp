#include "common.h"

#include "color.h" // includes Vec3
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"

#include <iostream>


Color ray_color(const Ray& r, const Hittable& world, int depth) {
    HitRecord rec;
    
    // depth indicates the ray bounce limit. if we exceed it,
    // no more light is gathered
    if (depth <= 0) {
        return Color(0,0,0);
    }
    
    if(world.hit(r, 0, infinity, rec)) {
        Point3 target = rec.p + rec.normal + random_in_unit_sphere();
        
        // simulate diffuse bounce rays
        return 0.5 * ray_color(Ray(rec.p, target - rec.p), world, depth-1);
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
    const int samples_per_pixel = 100;
    const int max_depth = 50;
    
    // world
    HittableList world;
    world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));
    
    // camera
    Camera cam;
    
    std::cout << "P3\n" << width << ' ' << height << "\n255\n";
    
    for(int i = height-1; i >= 0; i--) {
        std::cerr << "\rScanlines remaining: " << i << '\n';
        for(int j = 0; j < width; j++) {
            Color pixel_color(0, 0, 0);
            // calculate the color contribution of many rays on this pixel
            for(int s = 0; s < samples_per_pixel; s++) {
                auto u = (j + random_double()) / (width - 1);
                auto v = (i + random_double()) / (height - 1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    
    std::cerr << "\nDone!\n";
    
    return 0;
}