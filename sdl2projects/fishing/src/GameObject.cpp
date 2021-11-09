/**
*	GameObject class implementation
*/
// this contains the loadTexture function def
#include "../headers/functions.hh"

// constructor
GameObject::GameObject(const std::string name, const std::string file, SDL_Renderer *ren){
	
	// set the name 
	objectName = name;
	
	// by default, positions set to 0, 0 
	xPos = 0;
	yPos = 0;
	
	// load the image 
	objectTexture = loadTexture(file, ren);
	if(objectTexture == nullptr){
		// throw an exception here?
	}
	
	// get the dimensions of the texture 
	// pass in the addresses of width and height member variables
	int w = -1;
	int h = -1;
	if(SDL_QueryTexture(objectTexture, NULL, NULL, &w, &h) < 0){
		// throw an exception here?
	}else{
		width = w;
		height = h;
	}
	
}

// get the object's name 
std::string GameObject::getName(){
	return objectName;
}

// get the object's x position
int GameObject::getXPos(){
	return xPos;
}

// get the object's y position 
int GameObject::getYPos(){
	return yPos;
}

// get the object's texture 
SDL_Texture* GameObject::getTexture(){
	return objectTexture;
}

// get object's width 
int GameObject::getWidth(){
	return width;
}

// get object's height 
int GameObject::getHeight(){
	return height;
}



// set the object's x position 
void GameObject::setXPos(int x){
	xPos = x;
}

// set the object's y position 
void GameObject::setYPos(int y){
	yPos = y;
}



// destroy texture member variable
void GameObject::destroyTexture(){
	SDL_DestroyTexture(objectTexture);
}
