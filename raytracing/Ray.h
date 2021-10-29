#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

class Ray {
	public:
		Point3 orig;
		Vec3 dir;
		
		Ray(){}
		
		Ray(const Point3& orgin, const Vec3& direction)
			: orig(orgin), dir(direction) {}
		
		Point3 origin() const {
			return orig;
		}
		
		Vec3 direction() const {
			return dir;
		}
		
		Point3 at(double t) const {
			return orig + t*dir;
		}
};

#endif