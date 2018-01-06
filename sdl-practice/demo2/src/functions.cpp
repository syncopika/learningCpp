// something interesting to check out: 
// https://eliasdaler.wordpress.com/2014/11/01/using-lua-with-c-luabridge-part-2-using-scripts-for-object-behaviour/
// http://gamedevgeek.com/tutorials/moving-sprites-with-sdl/
// https://stackoverflow.com/questions/25738096/c-sdl2-error-when-trying-to-render-sdl-texture-invalid-texture

#include "../headers/functions.hh"

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
		
		// color key the surface to make any 255,255,255 pixels transparent!
		 SDL_SetColorKey(loadedImage, SDL_TRUE, SDL_MapRGB(loadedImage->format, 0xFF, 0xFF, 0xFF ) );
		
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
* render a texture 
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int Direction){
	// set up a destination rectangle to be at the position given by x and y 
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	
	// query the texture to get the width and height 
	// the rectangle will take on the width and height of the texture 
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	
	if(Direction == UP){
		SDL_RenderCopyEx(ren, tex, NULL, &dst, 0, NULL, SDL_FLIP_VERTICAL);
	}else if(Direction == LEFT){
		SDL_RenderCopyEx(ren, tex, NULL, &dst, 90, NULL, SDL_FLIP_NONE);
	}else if(Direction == RIGHT){
		SDL_RenderCopyEx(ren, tex, NULL, &dst, 270, NULL, SDL_FLIP_NONE);
	}else{
		SDL_RenderCopyEx(ren, tex, NULL, &dst, 0, NULL, SDL_FLIP_NONE);
	}
}

/**
* cleanup allocated memory from SDL stuff 
* the texture would be the background art in this case 
*/
void cleanup(SDL_Window* win, SDL_Texture* background, SDL_Renderer* ren){
	SDL_DestroyTexture(background);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
}

/**
* determine movement for sprite  
**/
int determineMovement(Character& character, const Uint8 *state){
	
	int x = character.getXPos();
	int y = character.getYPos();
	int direction = NONE;
	
	if(state[SDL_SCANCODE_LEFT]){
		character.setXPos(x - 5);
		direction = LEFT;
	}else if(state[SDL_SCANCODE_RIGHT]){
		character.setXPos(x + 5);
		direction = RIGHT;
	}else if(state[SDL_SCANCODE_UP]){
		character.setYPos(y - 5);
		direction = UP;
	}else if(state[SDL_SCANCODE_DOWN]){
		character.setYPos(y + 5);
		direction = DOWN;
	}
	
	// basic "collision detection"
	x = character.getXPos(); // this is the updated x value after the above
	y = character.getYPos();
	
	if(x < 0){
		character.setXPos(0);
	}else if(x > SCREEN_WIDTH - SPRITE_WIDTH){
		character.setXPos(SCREEN_WIDTH - SPRITE_WIDTH);
	}
	
	if(y < 0){
		character.setYPos(0);
	}else if(y > SCREEN_HEIGHT - SPRITE_HEIGHT){
		character.setYPos(SCREEN_HEIGHT - SPRITE_HEIGHT);
	}
	
	return direction;
	
}
