#include "Vec3.h"
#include "color.h"
#include <iostream>

int main(void){

	const int width = 256;
	const int height = 256;
	
	std::cout << "P3\n" << width << ' ' << height << "\n255\n";
	
	for(int i = height-1; i >= 0; i--){
		std::cerr << "\rScanlines remaining: " << i << '\n';
		for(int j = 0; j < width; j++){
			Color pixel_color((double)j / (width-1), (double)i / (height-1), 0.25);
			write_color(std::cout, pixel_color);
		}
	}
	
	std::cerr << "\nDone!\n";
	
	return 0;
}