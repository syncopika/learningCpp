#include <iostream>
#include <vector>
#include <algorithm>
#include "Point.hh"

// using brute-force to figure out shortest path

using namespace std;

//function prototypes
double circuitLength(const vector<Point> &points, const vector<int> &order);
vector<int> findShortestPath(const vector<Point> &points);

//function definitions
double circuitLength(const vector<Point> &points, const vector<int> &order){
	
	double circuitLength = 0;
	unsigned int size = order.size();
	
	for(unsigned int i = 0; i < size; i++){
		
		int fromIndex = order[i];
		
		int toIndex = order[(i + 1) % size]; //circular array indexing
		
		circuitLength += points[fromIndex].distanceTo(points[toIndex]);
		
	}
	
	return circuitLength;
}

vector<int> findShortestPath(const vector<Point> &points){
	
	double shortestPathLength;
	
	//the order of points that lead to the shortest distance
	vector<int> shortestPath;
	
	//store current path being looked at (note: point indices, not the points themselves)
	vector<int> currPathOrder; 
	
	//prepopulate currPathOrder from 0 to n-1, where n is the number of elements in 
	//the points vector. note: points.size() is an unsigned int
	for(unsigned int i = 0; i < points.size(); i++){
		currPathOrder.push_back((int)i);
	}
	
	//so now currPathOrder should be like [0, 1, 2, ...., n-1]
	shortestPathLength = circuitLength(points, currPathOrder);
		
	//we can loop through all permutations of currPathOrder
	//and find the shortest path
	while(next_permutation(currPathOrder.begin(), currPathOrder.end()) != false){
		
		//calling next_permutation transforms currPath into the next permutations
		//so we just need to analyze currPath here again
		double currLength = circuitLength(points, currPathOrder);
		
		if(currLength < shortestPathLength){
			shortestPathLength = currLength;
			shortestPath = currPathOrder;
		}
	}
	
	return shortestPath;
}

int main(){


	int numPoints;
	vector<Point> points;
	
	cout << "How many points?" << endl;
	cin >> numPoints;
	
	for(int i = 0; i < numPoints; i++){
		
		Point newPoint;
		int x, y, z;
		
		cout << "Point " << i << ": ";
		cin >> x >> y >> z;
		//cout << endl;
		
		newPoint.setX(x);
		newPoint.setY(y);
		newPoint.setZ(z);
		
		points.push_back(newPoint);
		
	}
	
	vector<int> shortestPath = findShortestPath(points);
	double length = circuitLength(points, shortestPath);
	
	cout << "the shortest path is: ";
	
	for(unsigned int i = 0; i < shortestPath.size(); i++){
		cout << shortestPath[i] << " ";
	}
	
	cout << endl;
	cout << "the shortest path length is: " << length << " units." << endl; 
	

	return 0;

}
