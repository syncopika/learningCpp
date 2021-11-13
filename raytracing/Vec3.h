#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include <stdexcept>

class Vec3 {
	private:
		double coords[3];

	public:
		Vec3() : coords{0,0,0} {} // list initialization in constructor
		Vec3(double x, double y, double z): coords{x,y,z} {}
		
		double x() const { return coords[0]; }
		double y() const { return coords[1]; }
		double z() const { return coords[2]; }
		
		// -v1 (inverse of v1)
		Vec3 operator-() const { 
			return Vec3(-coords[0], -coords[1], -coords[2]); 
		}
		
		// v1[0 or 1 or 2]
		double operator[](int i) const {
			if(i > 2 || i < 0){
				throw std::invalid_argument("index provided must be 0, 1 or 2.");
			}
			return coords[i];
		}
		
		// v1[0 or 1 or 2]
		double& operator[](int i) {
			if(i > 2 || i < 0){
				throw std::invalid_argument("index provided must be 0, 1 or 2.");
			}
			return coords[i];
		}
		
		// v1 += v2 
		Vec3& operator+=(const Vec3 &v) {
			coords[0] += v[0];
			coords[1] += v[1];
			coords[2] += v[2];
			return *this; // 'this' is a pointer
		}

		// vec3 *= n
		Vec3& operator*=(double factor) {
			coords[0] *= factor;
			coords[0] *= factor;
			coords[0] *= factor;
			return *this;
		}
		
		// vec3 /= n
		Vec3& operator/=(double factor) {
			return (*this) *= (1/factor); 
		}
		
		double length() const {
			return std::sqrt(length_squared());
		}
        
		double length_squared() const {
			return coords[0]*coords[0] + coords[1]*coords[1] + coords[2]*coords[2];
		}
        
        // static methods
        static Vec3 random() {
            return Vec3(random_double(), random_double(), random_double());
        }
        
        static Vec3 random(double min, double max) {
            return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
        }
};

// type aliases for other 3 dimensional things
using Point3 = Vec3;
using Color = Vec3;


// vec3 utility functions

// u + v
Vec3 operator+(const Vec3& u, const Vec3& v) {
	return Vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

// u - v
Vec3 operator-(const Vec3& u, const Vec3& v) {
	return Vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

//  u * v
Vec3 operator*(const Vec3& u, const Vec3& v) {
	return Vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

// t * u (t is a number)
Vec3 operator*(double t, const Vec3& u) {
	return Vec3(t * u[0], t * u[1], t * u[2]);
}

// u * t (t is a number)
Vec3 operator*(const Vec3& u, double t) {
	return t * u;
}

// u / t (t is a number)
Vec3 operator/(const Vec3& u, double t){
	return (1/t) * u;
}

// dot product
double dot(const Vec3& u, const Vec3& v){
    return u[0] * v[0]
        +  u[1] * v[1]
		+  u[2] * v[2];
}

// cross product
Vec3 cross(const Vec3& u, const Vec3& v){
	return Vec3(
		u[1] * v[2] - u[2] * v[1],
		u[2] * v[0] - u[0] * v[2],
		u[0] * v[1] - u[1] * v[0]
	);
}

Vec3 unit_vector(Vec3 v){
	return v / v.length();
}

// keep trying new points until we get one within the sphere (length_squared < 1)
Vec3 random_in_unit_sphere() {
    while (true) {
        auto p = Vec3::random(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

// std::cout << v1
std::ostream& operator<<(std::ostream &out, const Vec3& v) {
	return out << v[0] << ", " << v[1] << ", " << v[2];
}


#endif