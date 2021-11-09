#ifndef CHARACTER_H
#define CHARACTER_H

// Character class header file
#include "GameObject.hh"

// Character extends GameObject 
class Character : public GameObject{

	private:
		
		// keep a vector to hold the textures corresponding to the left and right
		// walking sprites of the character 
		// each step will alternate between left and right walking sprites, but with
		// the regular standing sprite (charTexture) in between, i.e.
		// left -> stand -> right -> stand -> ....
		// this one holds sprites facing the down direction
		std::vector<SDL_Texture*> walkingTextures;

	public:
	
		// constructor 
		// @param name = the character's name 
		// @param file = the file path to the character's .bmp image
		// 
		Character(const std::string name, const std::string file, SDL_Renderer *ren);
		
		
		// get the walking textures
		std::vector<SDL_Texture*> getWalkTextures();
		
		// add the walking textures - by default going in the DOWN DIRECTION
		void addWalkTexture(SDL_Texture* texture);
		
		// destroy textures 
		void destroyTexture();
		
};

#endif