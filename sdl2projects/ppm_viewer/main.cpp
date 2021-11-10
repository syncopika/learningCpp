// useful for the ppm images generated by the raytracer in the raytracing folder!

#include <SDL2/SDL.h>
#include <stdlib.h> // atoi

#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
std::vector<std::string> getTokens(std::string line){
    std::stringstream ss(line);
    std::vector<std::string> tokens;
    std::string buffer;
    
    while(ss >> buffer){
        tokens.push_back(buffer);
    }
    
    return tokens;
}

// https://stackoverflow.com/questions/23367098/getting-the-sdl-color-of-a-single-pixel-in-a-sdl-texture
// https://gamedev.stackexchange.com/questions/102490/fastest-way-to-render-image-data-from-buffer
int main(int argc, char* argv[]){
    if(argc != 2){
        // expect only 1 arg for the filename of the ppm file to read
        return 0;
    }

    char* filename = argv[1];
    std::string line;
    std::ifstream file(filename);
    
    uint8_t* imageData = nullptr;
    bool gotImageData = false;
    int width, height;
    
    if(file.is_open()){
        std::cout << "getting image data...\n";
        int lineNum = 1;
        int imageDataIdx = 0;
        
        while(std::getline(file, line)){
            if(lineNum == 1 && line != "P3"){
                // 1st line with P3 should indicate PPM 
                // (but of course there could be all sorts of other things wrong with the file)
                return 1;
            }
            
            if(lineNum == 2){
                // get width and height
                std::vector<std::string> dimensions = getTokens(line);
                width = atoi(dimensions[0].c_str());
                height = atoi(dimensions[1].c_str());
                
                // allocate mem for image data
                imageData = new uint8_t[width*height*3]; // 3 bytes per pixel (RGB)
            }
            
            if(lineNum > 3){
                // get color of pixel
                std::vector<std::string> color = getTokens(line);
                uint8_t r = (uint8_t)atoi(color[0].c_str());
                uint8_t g = (uint8_t)atoi(color[1].c_str());
                uint8_t b = (uint8_t)atoi(color[2].c_str());
                
                // add info to image data
                imageData[imageDataIdx++] = r;
                imageData[imageDataIdx++] = g;
                imageData[imageDataIdx++] = b;
            }
            
            lineNum++;
        }
        
        gotImageData = true;
        std::cout << "finished collecting image data\n";
        
        file.close(); 
    }
    
    if(gotImageData){
        // display image
        if(SDL_Init(SDL_INIT_VIDEO) != 0){
            return 1;
        }
        
        // create a window 
        SDL_Window* window = SDL_CreateWindow(
            "ppm_viewer", 
            SDL_WINDOWPOS_CENTERED, 
            SDL_WINDOWPOS_CENTERED, 
            width, 
            height, 
            SDL_WINDOW_SHOWN
        );
        if(window == nullptr){
            SDL_Quit();
            return 1;
        }
        
        // create the renderer to render the window with 
        SDL_Renderer* renderer = SDL_CreateRenderer(
            window, 
            -1, 
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        );
        if(renderer == nullptr){
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
        
        // set up the SDL texture to display the image
        SDL_Texture* texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGB24, // lol https://stackoverflow.com/questions/33962331/sdl2-difference-between-rgb888-and-rgb24
            SDL_TEXTUREACCESS_STREAMING,
            width,
            height
        );
        
        // draw the image
        int pitch = width*3; // 3 bytes per pixel
        uint8_t* texBuf = new uint8_t[width*height*3]; // empty texture buffer to write to
        SDL_LockTexture(texture, nullptr, (void**)&texBuf, &pitch);
        std::memcpy(texBuf, (uint8_t *)imageData, width*height*3); // copy image data to texture
        SDL_UnlockTexture(texture);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        
        // delete allocated memory for image data
        delete[] texBuf;
        delete[] imageData;
        
        SDL_Event event;
        bool quit = false;
        while(!quit){
            while(SDL_PollEvent(&event)){
                if(event.type == SDL_QUIT){
                    quit = true;
                }
            }
            
            // show image
            SDL_RenderPresent(renderer);
        }
    
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    return 0;
}
