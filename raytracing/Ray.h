#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

class Ray {
	public:
		Point3 origin;
		Vec3 dir;
		
		Ray(){}
		
		Ray(const Point3& org, const Vec3& direction)
			: origin(org), dir(direction) {}
		
		Point3 origin() const {
			return origin;
		}
		
		Vec3 direction() const {
			return dir;
		}
		
		Point3 at(double t) const {
			return origin + t*dir;
		}
};

#endif