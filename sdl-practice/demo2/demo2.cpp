// something interesting to check out: 
// https://eliasdaler.wordpress.com/2014/11/01/using-lua-with-c-luabridge-part-2-using-scripts-for-object-behaviour/
// http://gamedevgeek.com/tutorials/moving-sprites-with-sdl/
// https://stackoverflow.com/questions/25738096/c-sdl2-error-when-trying-to-render-sdl-texture-invalid-texture

#include "demo2.hh"

/**
*	character class 
*/

// constructor
Character::Character(const std::string name, const std::string file, SDL_Renderer *ren){
	
	// set the name 
	charName = name;
	
	// by default, positions set to 0, 0 
	xPos = 0;
	yPos = 0;
	
	// load the image 
	charTexture = loadTexture(file, ren);
	if(charTexture == nullptr){
		// what about freeing the window?
		// throw an exception here?
	}
}

// get the character's name 
std::string Character::getName(){
	return charName;
}

// get the character's x position
int Character::getXPos(){
	return xPos;
}

// get the character's y position 
int Character::getYPos(){
	return yPos;
}

// get the character's texture 
SDL_Texture* Character::getTexture(){
	return charTexture;
}

// set the character's x position 
void Character::setXPos(int x){
	xPos = x;
}

// set the character's y position 
void Character::setYPos(int y){
	yPos = y;
}

// destroy texture member variable
void Character::destroyTexture(){
	SDL_DestroyTexture(charTexture);
}




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
* render a texture 
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	// set up a destination rectangle to be at the position given by x and y 
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	
	// query the texture to get the width and height 
	// the rectangle will take on the width and height of the texture 
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);
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
void determineMovement(Character& character, const Uint8 *state){
	
	int x = character.getXPos();
	int y = character.getYPos();
	
	if(state[SDL_SCANCODE_LEFT]){
		character.setXPos(x - 2);
	}else if(state[SDL_SCANCODE_RIGHT]){
		character.setXPos(x + 2);
	}else if(state[SDL_SCANCODE_UP]){
		character.setYPos(y - 2);
	}else if(state[SDL_SCANCODE_DOWN]){
		character.setYPos(y + 2);
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
	
}
