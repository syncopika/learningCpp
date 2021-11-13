#include "tutorial.hh"

/**
* log SDL error 
*/
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

/**
* load a texture 
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	
	// initialize texture to null first 
	SDL_Texture *texture = nullptr;
	
	// load the image 
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
	
	// if the load was successful, loadedImage won't be nullptr 
	if(loadedImage != nullptr){
		// turn the surface to a texture 
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		
		// free loadedImage memory since we're done with it 
		SDL_FreeSurface(loadedImage);
		
		// make sure texture creation was successful 
		if(texture == nullptr){
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
	}else{
		// error with loadedImage 
		logSDLError(std::cout, "LoadBMP");
	}
	
	return texture; 
}

/**
*  load a texture - this one can scale the image 
*  scale based on width and height (width and height of the canvas)
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren, int width, int height){
	
	int w = (int)(width / 2);
	int h = (int)(height / 2);
	
	// initialize texture to null first 
	SDL_Texture *texture = nullptr;
	SDL_Surface *scaled_surface = nullptr;
	
	// scaled surface pointer 
	scaled_surface = SDL_CreateRGBSurface(0, w, h, 24, 0, 0, 0, 0);
	if(scaled_surface == nullptr){
		logSDLError(std::cout, "createRGBsurface");
	}
	
	SDL_Rect scaled_rect;
	
	// give the scaled dimensions to the rectangle (here the scale is .5)
	scaled_rect.x = 0;
	scaled_rect.y = 0;
	scaled_rect.h = h;
	scaled_rect.w = w;
	
	// load the image 
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
	
	// if the load was successful, loadedImage won't be nullptr 
	if(loadedImage != nullptr){
		
		// scale the texture
		if(SDL_BlitScaled(loadedImage, NULL, scaled_surface, &scaled_rect) != 0){
			logSDLError(std::cout, "BlitScaled");
		}
		
		// assign texture the scaled surface 
		texture = SDL_CreateTextureFromSurface(ren, scaled_surface);

		// make sure texture creation was successful 
		if(texture == nullptr){
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
		
	}else{
		// error with loadedImage 
		logSDLError(std::cout, "LoadBMP");
	}
	
	// free loadedImage memory since we're done with it 
	SDL_FreeSurface(loadedImage);
	// free scaled surface memory
	SDL_FreeSurface(scaled_surface);
	
	return texture;
}

/**
* render a texture 
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	// set up a destination rectangle to be at the position given by x and y 
	// the destination rectangle is in the SDL window -> the x and y are coordinates 
	// with respect to the rectangle, which, depending on its width and height, is basically 
	// the same as the window 
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	
	// query the texture to get the width and height 
	// the rectangle will take on the width and height of the texture 
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);
}
