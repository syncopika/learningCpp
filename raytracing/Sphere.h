#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "Vec3.h"

class Sphere : public Hittable {
    public:
        Point3 center;
        double radius;
        
        Sphere(){}
        Sphere(Point3 cen, double rad): center(cen), radius(rad) {};
        
        virtual bool hit(
            const Ray& ray,
            double t_min,
            double t_max,
            HitRecord& rec
        ) const override;
};

bool Sphere::hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const {
    // t^2(b*b) + 2tb*(A - C) + (A - C)*(A - C) - r^2 = 0
    // where b = direction vector of the ray, A = the origin of the ray, r = radius of sphere
    // C is the center of the sphere, and t is what we're trying to solve for
    Vec3 oc = ray.origin() - center;
    auto a = dot(ray.direction(), ray.direction());
    auto b = 2.0*dot(oc, ray.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    
    if(discriminant < 0){
        return false;
    }
    
    auto sqrtd = sqrt(discriminant);
    
    // find the nearest root within the acceptable range
    auto t = (-b + sqrtd) / (2*a); // following quadratic formula
    if (t < t_min || t > t_max) {
        t = (-b - sqrtd) / (2*a);
        if(t < t_min || t > t_max){
            return false;
        }
    }
    
    rec.t = t;
    rec.p = ray.at(t);
    Vec3 normal = (rec.p - center) / radius;
    rec.set_front_face(ray, normal); // check if normal is outside of sphere or actually inside
    
    return true;
}


#endif