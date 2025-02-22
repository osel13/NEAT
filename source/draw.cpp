
//#pragma once
#include <SDL2/SDL.h>
#include <cstdio>
#include <vector>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void init();
void close();

//something read_input();

int main(int argc, char *argv[])
{
  
    
    
//read_input();
    int firstLayerNeuronCount = 2;
    int lastLayerNeuronCount = 1;
    int maxVerticalNeurons = 3;
    int maxHorizontalNeurons = 3;

//Select the best network
 
//calculate node position
    
//calculate node size
    
//Draw window 1280x720 (scalable?)
    
    init();
    bool quit = false;
    SDL_Event event;
    while(!quit){
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                    quit = true;
            }
        }
    SDL_UpdateWindowSurface(window);
    
    //Clear screen
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear(renderer );

    /*
    //Render red filled quad
    SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF );
    SDL_RenderFillRect(renderer, &fillRect );

                            //Render green outlined quad
    SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF );
    SDL_RenderDrawRect(renderer, &outlineRect );

                            //Draw blue horizontal line
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF );
    SDL_RenderDrawLine(renderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 );

                            //Draw vertical line of yellow dots
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF );
    for(int i = 0; i < SCREEN_HEIGHT; i += 4){
        SDL_RenderDrawPoint(renderer, SCREEN_WIDTH / 2, i );
    }
    */
            
    


//Margins 1 in the middle, 2 in thirds, 10px is the limit
    SDL_Rect outlineRect = { 10, 10, SCREEN_WIDTH-20, SCREEN_HEIGHT-20 };
    SDL_SetRenderDrawColor(renderer, 0x10, 0x10, 0x10, 0xFF );
    SDL_RenderDrawRect(renderer, &outlineRect );

//Neurons has to fit max is 2 pixels more than number height, square box (panning?)
//count neurons, count sizes
    

//draw input nodes
    //enabled node
    SDL_Rect input_enabled = { SCREEN_WIDTH*1/3-10 , SCREEN_HEIGHT*2/3-10, 20, 20 };
    SDL_SetRenderDrawColor(renderer, 0xDD, 0x00, 0x00, 0xFF );
    SDL_RenderFillRect(renderer, &input_enabled );
    
    //disabled node
    SDL_Rect input_disabled = { SCREEN_WIDTH*1/3-10, SCREEN_HEIGHT*1/3-10, 20, 20 };
    SDL_SetRenderDrawColor(renderer, 0x50, 0x00, 0x00, 0xFF );
    SDL_RenderFillRect(renderer, &input_disabled );
    
//draw output nodes
    
    SDL_Rect output_enabled = { SCREEN_WIDTH*2/3-10, SCREEN_HEIGHT*2/3-10, 20, 20 };
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xDD, 0xFF );
    SDL_RenderFillRect(renderer, &output_enabled );
    
    //disabled node
    SDL_Rect output_disabled = { SCREEN_WIDTH*2/3-10, SCREEN_HEIGHT*1/3-10, 20, 20 };
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x50, 0xFF );
    SDL_RenderFillRect(renderer, &output_disabled );
    
//draw nodes in between
    //enabled node
    SDL_Rect middle_enabled = { SCREEN_WIDTH*1/2-10, SCREEN_HEIGHT*1/3-10, 20, 20 };
    SDL_SetRenderDrawColor(renderer, 0x00, 0xDD, 0x00, 0xFF );
    SDL_RenderFillRect(renderer, &middle_enabled );
    
    //disabled node
    SDL_Rect middle_disabled = { SCREEN_WIDTH*1/2-10, SCREEN_HEIGHT*2/3-10, 20, 20 };
    SDL_SetRenderDrawColor(renderer, 0x00, 0x50, 0x00, 0xFF );
    SDL_RenderFillRect(renderer, &middle_disabled );
//draw edges (must be behind nodes)
    //enabled
    SDL_SetRenderDrawColor(renderer, 0xDD, 0xDD, 0x00, 0xFF );
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH*1/3+10, SCREEN_HEIGHT*2/3, SCREEN_WIDTH*1/2-10, SCREEN_HEIGHT*1/3 );
    
    //disabled
    SDL_SetRenderDrawColor(renderer, 0x50, 0x50, 0x00, 0xFF );
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH*1/2+10, SCREEN_HEIGHT*1/3, SCREEN_WIDTH*2/3-10, SCREEN_HEIGHT*2/3 );
//draw numbers (must be on top of nodes
    
    SDL_RenderPresent(renderer);
    }
    close();  
  return 0;  
}

void init() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(1);
    }
    else{
        window = SDL_CreateWindow("NEAT",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window == NULL) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            exit(1);
        }
        else {
             renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
                        if( renderer == NULL )
                        {
                                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                                exit(1);
                        }
                        else
                        {
                                //Initialize renderer color
                                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF );
                        }
        }

    }
}

void close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;
    SDL_Quit();
}

/*
something read_input(path);{
    
}
*/


