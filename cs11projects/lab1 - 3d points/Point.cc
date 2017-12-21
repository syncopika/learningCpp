#include "Point.hh"
#include <cmath>

// Default constructor:  initializes the point to (0, 0, 0).
Point::Point() {
  x_coord = 0;
  y_coord = 0;
  z_coord = 0;
}

// Initializes the point to (x, y, z).
Point::Point(double x, double y, double z) {
  x_coord = x;
  y_coord = y;
  z_coord = z;
}

// Destructor - Point allocates no dynamic resources.
Point::~Point() {
  // no-op
}

// Mutators: (setters)

void Point::setX(double val) {
  x_coord = val;
}

void Point::setY(double val) {
  y_coord = val;
}

void Point::setZ(double val) {
  z_coord = val;
}

// Accessors: (getters)
// make these const to guarantee that these functions don't change 
// member variables of this class. also, so you can use a const vector of points!
double Point::getX() const {
  return x_coord;
}

double Point::getY() const{
  return y_coord;
}

double Point::getZ() const{
  return z_coord;
}

// Get distance between two points

double Point::distanceTo(const Point &p) const{
	
	//the formula is: square root(a^2 + b^2 + c^2)
	//first get the difference between x, y, and z coords
	double a = x_coord - p.getX();
	double b = y_coord - p.getY();
	double c = z_coord - p.getZ();
	
	return sqrt(a*a + b*b + c*c);
	
}

// equals method for three points

bool Point::equals(Point &other){
	
	bool x = (x_coord == other.getX());
			
	bool y = (y_coord == other.getY());
	
	bool z = (z_coord == other.getZ());
			
	return (x && y) && z;
}
