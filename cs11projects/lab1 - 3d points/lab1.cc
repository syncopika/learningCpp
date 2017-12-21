#include <iostream>
#include <cmath>
#include "Point.hh"

using namespace std;

// compute area of three points

// prototype 
double computeArea(Point &p1, Point &p2, Point &p3);

//the ampersand means "take in a Point object by reference"
//it doesn't mean pass in a Point object with an ampersand
double computeArea(Point &p1, Point &p2, Point &p3){
	
	double a = p1.distanceTo(p2);
	double b = p1.distanceTo(p3);
	double c = p2.distanceTo(p3);
	double s = (a + b + c) / 2;
	
	return sqrt(s * (s-a) * (s-b) * (s-c));
	
}

int main(){
	
	Point p1;
	Point p2;
	Point p3;
	
	double x;
	double y;
	double z;
	
	cout << "Please enter an x, y, and z coordinate for a point." << endl;
	cin >> x >> y >> z;
	p1.setX(x);
	p1.setY(y);
	p1.setZ(z);
	
	cout << "Please enter an x, y, and z coordinate for a point." << endl;
	cin >> x >> y >> z;
	p2.setX(x);
	p2.setY(y);
	p2.setZ(z);
	
	cout << "Please enter an x, y, and z coordinate for a point." << endl;
	cin >> x >> y >> z;
	p3.setX(x);
	p3.setY(y);
	p3.setZ(z);
	
	
	cout << "Point1: " << p1.getX() << " " << p1.getY() << " " << p1.getZ() << endl;
	cout << "Point2: " << p2.getX() << " " << p2.getY() << " " << p2.getZ() << endl;
	cout << "Point3: " << p3.getX() << " " << p3.getY() << " " << p3.getZ() << endl;
	
	
	double area;

	if(p1.equals(p2) || p2.equals(p3) || p1.equals(p3)){
		area = 0;
	}else{
		area = computeArea(p1, p2, p3);
	}
	
	cout << "area: " << area << endl;
	
	return 0;
}
