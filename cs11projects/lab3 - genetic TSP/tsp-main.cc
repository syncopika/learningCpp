// main method for genetic tsp 
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "tsp-ga.hh"

using namespace std;

int main(int argc, char** argv){
	
	// verify argc 
	if(argc != 5){
		cout << "not the right number of arguments!" << endl;
		return 0;
	}
	
	// get args 
	int population = atoi(argv[1]);
	int generations = atoi(argv[2]);
	double keep = atof(argv[3]);
	double mutate = atof(argv[4]);
	
	// seed random generator 
	srand(time(nullptr));
	
	// convert args into the proper argument values to pass to shortPath function 
	int keepPop = (int)(population * keep);
	int numMutations = (int)(mutate * population);
	
	// prompt user for number of points/ specify points 
	int numPoints;
	vector<Point> points;
	
	cout << "How many points?" << endl;
	cin >> numPoints;
	
	for(int i = 0; i < numPoints; i++){
		
		Point newPoint;
		int x, y, z;
		
		cout << "Point " << i << ": ";
		cin >> x >> y >> z;
		
		newPoint.setX(x);
		newPoint.setY(y);
		newPoint.setZ(z);
		
		points.push_back(newPoint);
	}
	
	cout << endl;
	
	TSPGenome aShortPath = findAShortPath(points, population, generations, keepPop, numMutations);
	vector<int> sPath = aShortPath.getOrder();
	double length = aShortPath.getCircuitLength();
	
	cout << "best order: ";
	
	int size = (int)sPath.size();
	for(int i = 0; i < size; i++){
		cout << sPath[i] << " ";
	}
	
	cout << endl;
	cout << "the shortest path length is: " << length << " units." << endl; 
	
	return 0;
	
}