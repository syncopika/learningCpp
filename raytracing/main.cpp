#include "Vec3.h"

#include <iostream>

int main(void){

	const int width = 256;
	const int height = 256;
	
	std::cout << "P3\n" << width << ' ' << height << "\n255\n";
	
	for(int i = height-1; i >= 0; i--){
		std::cerr << "\rScanlines remaining: " << i << '\n';
		for(int j = 0; j < width; j++){
			auto r = double(j) / (width-1);
			auto g = double(i) / (height-1);
			auto b = 0.25;
			
			int jr = static_cast<int>(255.999 * r);
			int jg = static_cast<int>(255.999 * g);
			int jb = static_cast<int>(255.999 * b);
			
			std::cout << jr << ' ' << jg << ' ' << jb << '\n';
		}
	}
	
	std::cerr << "\nDone!\n";
	
	return 0;
}