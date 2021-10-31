#include "Vec3.h"
#include "color.h"
#include "ray.h"
#include <iostream>

double hit_sphere(const Point3& center, double radius, const Ray& r){
    // t^2(b*b) + 2tb*(A - C) + (A - C) * (A - C) - r^2 = 0
    // where b = direction vector of the ray, A = the origin of the ray, r = radius of sphere
    // C is the center of the sphere, and t is what we're trying to solve for
    Vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0f*dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    if(discriminant < 0){
        return -1.0;
    } else {
        return (-b - sqrt(discriminant)) / (2.0f*a);
    }
}

Color ray_color(const Ray& r) {
    auto t = hit_sphere(Point3(0,0,-1), 0.5f, r);
    if(t > 0.0f){
        Vec3 normal = unit_vector(r.at(t) - Point3(0, 0, -1)); // create a vector using the point hit by the ray and the center of the sphere
        return 0.5f*Color(normal.x()+1, normal.y()+1, normal.z()+1);
    }
    
    Vec3 unit_direction = unit_vector(r.direction());
    t = 0.5f*(unit_direction.y() + 1.0f);
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