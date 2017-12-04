// header file for TSPGenome

class TSPGenome {

	private:
		vector<int> orderOfPoints;
		
		// needs to be initialized to a dummy value
		double circuitLength;

	public:
		
		// constructor with just an int
		TSPGenome(int num);
		
		// constructor with vector arg
		TSPGenome(const vector<int> &order);
		
		//destructor
		~TSPGenome();
		
		// getters
		vector<int> getOrder() const;
		double getCircuitLength() const;
		
		//compute circuit length
		void computeCircuitLength(const vector<Point> &points);
		
		void mutate(vector<int> pointOrder);

};

// non-member function to crosslink genomes
// returns a TSPGenome
TSPGenome crosslink(const TSPGenome &g1, const TSPGenome &g2);