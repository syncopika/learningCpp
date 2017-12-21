// header file for TSPGenome

#include <vector>
#include "Point.hh"

class TSPGenome {

	private:
		std::vector<int> orderOfPoints;
		
		// needs to be initialized to a dummy value
		double circuitLength;

	public:
		
		// constructor with just an int
		TSPGenome(int num);
		
		// constructor with vector arg
		TSPGenome(const std::vector<int> &order);
		
		//destructor
		~TSPGenome();
		
		// getters
		std::vector<int> getOrder() const;
		double getCircuitLength() const;
		
		//compute circuit length
		void computeCircuitLength(const std::vector<Point> &points);
		
		void mutate();

};

// non-member function to crosslink genomes
// returns a TSPGenome
TSPGenome crosslink(const TSPGenome &g1, const TSPGenome &g2);

// find a short path 
TSPGenome findAShortPath(const std::vector<Point> &points, 
                         int populationSize, 
						 int numGenerations, 
						 int keepPopulation, 
						 int numMutations);