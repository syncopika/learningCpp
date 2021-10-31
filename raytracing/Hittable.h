#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"

struct HitRecord {
    Point3 p;
    Vec3 normal;
    double t;
    bool front_face;
    
    void set_front_face(const Ray& r, const Vec3& out_normal) {
        front_face = dot(r.direction(), out_normal) < 0;
        normal = front_face ? out_normal : -out_normal;
    }
};

class Hittable {
    public:
        virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
};

#endif