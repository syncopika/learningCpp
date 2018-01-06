/**
*	character class implementation
*/

// this contains the loadTexture function def AND references Character.hh 
#include "../headers/functions.hh"

// constructor
Character::Character(const std::string name, const std::string file, SDL_Renderer *ren) : 
	GameObject(name, file, ren){}


// get character's walking textures 
// note that they're facing south
std::vector<SDL_Texture*> Character::getWalkTextures(){
	return walkingTextures;
}


// add a walking sprite texture 
void Character::addWalkTexture(SDL_Texture* texture){
	walkingTextures.push_back(texture);
}


// destroy texture member variable
// overriding the super class' destroyTexture method!!
void Character::destroyTexture(){
	SDL_DestroyTexture(objectTexture);
	for(SDL_Texture* t : walkingTextures){
		SDL_DestroyTexture(t);
	}
}
