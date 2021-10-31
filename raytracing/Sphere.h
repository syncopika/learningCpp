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
            const Ray& r,
            double t_min,
            double t_max,
            HitRecord& rec
        ) const override;
};

bool Sphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    Vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0f*dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    
    if(discriminant < 0){
        return false;
    }
    
    auto sqrtd = sqrt(discriminant);
    
    // find the nearest root within the acceptable range
    auto t = (b + sqrtd) / a; // following quadratic formula
    if (t < t_min || t > t_max) {
        t = (b - sqrtd) / a;
        if(t < t_min || t > t_max){
            return false;
        }
    }
    
    rec.t = t;
    rec.p = r.at(t);
    
    Vec3 normal = (rec.p - center) / radius;
    rec.set_front_face(r, normal); // check if normal is outside of sphere or actually inside
    
    return true;
}


#endif