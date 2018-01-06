// header for GameObject class
// a new class like Character or NatureObject can extend this one 

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>

class GameObject{

	// these variables are protected, not private, so that subclasses can access them
	// without needing to use the getters
	protected:
		
		std::string objectName;
		SDL_Texture *objectTexture;
		
		// location of object
		int xPos;
		int yPos;
		
		// dimensions of image (useful for bounding box)
		int width;
		int height;

	public:
	
		// constructor 
		// @param name = the character's name 
		// @param file = the file path to the character's .bmp image
		GameObject(const std::string name, const std::string file, SDL_Renderer *ren);
		
		// get the object's name 
		std::string getName();
		
		// get the object's x position
		int getXPos();
		
		// get the object's y position 
		int getYPos();
		
		// get object's width 
		int getWidth();
		
		// get object's height 
		int getHeight();
		
		// get texture 
		SDL_Texture* getTexture();
				
		// set x pos 
		void setXPos(int x);
		
		// set y pos 
		void setYPos(int y);
		
		// destroy texture
		virtual void destroyTexture();

};
