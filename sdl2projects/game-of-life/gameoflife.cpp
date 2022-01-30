/*

an implementation of John Horton Conway's Game of Life.
https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

rules:
    for each cell:
        - if it is alive has 2 or 3 live neighbors, it lives on to the next generation
        - if it is dead but has 3 live neighbors, it becomes alive in the next generation
        - all other live cells become dead in the next generation

TODO:
improve performance

*/

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

// 5 x 5 block
#define BLOCK_SIZE 5
#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 200

const int gridHeight = SCREEN_HEIGHT / BLOCK_SIZE;
const int gridWidth = SCREEN_WIDTH / BLOCK_SIZE;

bool gamePaused = false;

void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}


struct GridCell {
    bool isAlive;
    SDL_Texture* sprite;
    
    /*
    void update(bool state, SDL_Texture* sprite){
        isAlive = state;
        this->sprite = sprite 
    }*/
};

struct Grid {
    GridCell grid[gridHeight][gridWidth];
    
    void initGrid(SDL_Texture* blockSprite){
        for(int i = 0; i < gridHeight; i++){
            for(int j = 0; j < gridWidth; j++){
                if((i == (gridHeight/3) && j == (gridWidth/4)) ||
                   (i == (gridHeight/3)+1 && j == (gridWidth/4)) ||
                   (i == (gridHeight/3)+2 && j == (gridWidth/4)) ||
                   (i == (gridHeight/3)+1 && j == (gridWidth/4)+3) ||
                   (i == (gridHeight/3)+3 && j >= (gridWidth/4)+4) ||
                   (i == (gridHeight/3)+1 && j == (gridWidth/4)+5) ||
                   (i == (gridHeight/3)+3 && j == (gridWidth/4)+6) ||
                   (i == (gridHeight/3)+2 && j == (gridWidth/4)+3) ||
                   (i == (gridHeight/3)+3 && j == (gridWidth/4)+4) ||
                   (i == (gridHeight/3) && j == (gridWidth/4)+6) ||
                   (i == (gridHeight/3)-1 && j == (gridWidth/4)+6) ||
                   (i == (gridHeight/3) && j == (gridWidth/4)-2) ||
                   (i == (gridHeight/3)-2 && j == (gridWidth/4)+3) ||
                   (i == (gridHeight/3)-5 && j == (gridWidth/4)+2) ||
                   (i == (gridHeight/3) && j == (gridWidth/4)+2) ||
                   (i >= (gridHeight/3)-2 && j == (gridWidth/4)+5) ||
                   (i == (gridHeight/3)-4 && j == (gridWidth/4)+3) ||
                   (i == (gridHeight/3)-4 && j == (gridWidth/4)-5)
                   ){
                    grid[i][j].isAlive = true;
                    grid[i][j].sprite = blockSprite;
                    //std::cout << "row: " << i << ", col: " << j << '\n';
                }else{
                    grid[i][j] = GridCell{false, nullptr};
                }
            }
        }
    }
    
    int getNumLiveNeighbors(int row, int col){
        int numLiveNeighbors = 0;
        
        // top-left
        if(row-1 >= 0 && col-1 >= 0 && grid[row-1][col-1].isAlive){
            numLiveNeighbors++;
        }
        // top
        if(row-1 >= 0 && grid[row-1][col].isAlive){
            numLiveNeighbors++;
        }
        // top-right
        if(row-1 >= 0 && col+1 < gridWidth && grid[row-1][col+1].isAlive){
            numLiveNeighbors++;
        }
        // bottom-left
        if(row+1 < gridHeight && col-1 >= 0 && grid[row+1][col-1].isAlive){
            numLiveNeighbors++;
        }
        // bottom
        if(row+1 < gridHeight && grid[row+1][col].isAlive){
            numLiveNeighbors++;
        }
        // bottom-right
        if(row+1 < gridHeight && col+1 < gridWidth && grid[row+1][col+1].isAlive){
            numLiveNeighbors++;
        }
        
        return numLiveNeighbors;
    }

    // for live cells
    bool hasTwoOrThreeLiveNeighbors(int row, int col){
        int numNeighbors = getNumLiveNeighbors(row, col);
        return numNeighbors == 2 || numNeighbors == 3;
    }

    // for dead cells
    bool hasThreeLiveNeighbors(int row, int col){
        int numNeighbors = getNumLiveNeighbors(row, col);
        return numNeighbors == 3;        
    }
    
    void step(SDL_Texture* sprite){
        // create a new temp grid to store new changes based on current state
        // TODO: maybe we can optimize this slightly by only throwing in the coordinates
        // of cells that need to change state in a set? and then iterate the set.
        GridCell temp[gridHeight][gridWidth];
        for(int i = 0; i < gridHeight; i++){
            for(int j = 0; j < gridWidth; j++){
                GridCell& cell = grid[i][j];
                if(cell.isAlive){
                    if(hasTwoOrThreeLiveNeighbors(i, j)){
                        temp[i][j].isAlive = true;
                        temp[i][j].sprite = sprite;
                    }else{
                        temp[i][j].isAlive = false;
                        temp[i][j].sprite = nullptr;
                    }
                }else{
                    if(hasThreeLiveNeighbors(i, j)){
                        temp[i][j].isAlive = true;
                        temp[i][j].sprite = sprite;
                    }else{
                        temp[i][j].isAlive = false;
                        temp[i][j].sprite = nullptr;                        
                    }
                }
            }
        }
        
        // then update each cell
        for(int i = 0; i < gridHeight; i++){
            for(int j = 0; j < gridWidth; j++){
                grid[i][j].isAlive = temp[i][j].isAlive;
                grid[i][j].sprite = temp[i][j].sprite;
            }
        }
    }
    
    void render(SDL_Renderer* renderer){
        for(int i = 0; i < gridHeight; i++){
            for(int j = 0; j < gridWidth; j++){
                GridCell& cell = grid[i][j];
                if(cell.isAlive){
                    //std::cout << "drawing cell\n";
                    SDL_Texture* blockSprite = cell.sprite;
                    SDL_Rect dst;
                    dst.x = j * BLOCK_SIZE;
                    dst.y = i * BLOCK_SIZE;
                    SDL_QueryTexture(blockSprite, nullptr, nullptr, &dst.w, &dst.h); 
                    SDL_RenderCopyEx(renderer, blockSprite, nullptr, &dst, 0, nullptr, SDL_FLIP_NONE);
                }
            }
        }
    }
};

SDL_Texture* loadTexture(const std::string& file, SDL_Renderer *ren){
    // initialize texture to null first 
    SDL_Texture *texture = nullptr;
    
    // load the image 
    SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
    
    // if the load was successful, loadedImage won't be nullptr 
    if(loadedImage != nullptr){
        // color key the surface to make any 255,255,255 pixels transparent!
        //SDL_SetColorKey(loadedImage, SDL_TRUE, SDL_MapRGB(loadedImage->format, 0xFF, 0xFF, 0xFF));
        
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

void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, double rotation){
    // set up a destination rectangle to be at the position given by x and y 
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    
    // query the texture to get the width and height 
    // the rectangle will take on the width and height of the texture 
    SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
    
    SDL_RenderCopyEx(ren, tex, nullptr, &dst, rotation, nullptr, SDL_FLIP_NONE);
}

int main(int argc, char** argv){
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        logSDLError(std::cout,"SDL_Init Error: ");
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("game of life", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    
    if(window == nullptr){
        logSDLError(std::cout, "SDL_CreateWindow Error: ");
        SDL_Quit();
        return 1;
    }
    
    // create the renderer to render the window with 
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == nullptr){
        SDL_DestroyWindow(window);
        logSDLError(std::cout,"SDL_CreateRenderer Error: ");
        SDL_Quit();
        return 1;
    }
    
    /***
        set up background
    ***/
    SDL_Texture* bg = loadTexture("background.bmp", renderer);
    if(bg == nullptr){
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        logSDLError(std::cout,"BG creation failed: ");
        SDL_Quit();
        return 1;
    }
    renderTexture(bg, renderer, 0, 0, 0);    
    
    // for cells that are alive
    SDL_Texture* blockSprite = loadTexture("block.bmp", renderer);
    if(blockSprite == nullptr){
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        logSDLError(std::cout,"block sprite creation failed: ");
        SDL_Quit();
        return 1;
    }
 
    // set initial state (mark some cells as alive)
    Grid theGrid;
    theGrid.initGrid(blockSprite);
    
    //renderTexture(bg, renderer, 0, 0, 0);
    
    bool quit = false; 
    SDL_Event event;
    while(!quit){
        //SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        //SDL_RenderClear(renderer); // clear the frame (make #fff)
        
        // check for actions that will quit the program 
        if(SDL_PollEvent(&event)){
            switch(event.type){
                // clicking on the X button on top right of screen
                case SDL_QUIT:
                    quit = true;
                    break;
                
                // pressing q or Esc on keyboard and handles movement for player
                // we're avoiding SDL_GetKeyboardState because that registers multiples events for a single button press
                // which we don't want in this case
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_ESCAPE:
                        case SDLK_q:
                            quit = true;
                            break;
                        case SDLK_SPACE:
                            gamePaused = !gamePaused;
                            std::cout << "spacebar pressed\n";
                            break;
                    }
                break;
            }
        }
        
        if(!gamePaused){
            // redraw the background
            renderTexture(bg, renderer, 0, 0, 0);
            
            // check next state
            theGrid.step(blockSprite);
            
            // render all the live cells
            theGrid.render(renderer);
            
            // update screen 
            SDL_RenderPresent(renderer);
            
            SDL_Delay(800);
        }
    }
    
    // cleanup
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(blockSprite);
    
    SDL_Quit();
    
    return 0;
}