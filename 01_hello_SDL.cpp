// Using SDL and standard IO
#include <SDL2/SDL.h>
// Using SDL2_image
#include <SDL2/SDL_image.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main( int argc, char* args[] )
{
    //The window we'll be rendering to
    SDL_Window* window = NULL;
    // SDL Event handler
    SDL_Event event;
    //The surface contained by the window
    SDL_Surface* image = NULL;
    // 2D Rendering Context for Window
    SDL_Renderer * renderer = NULL;
    // Create Textture from existing Surface
    SDL_Texture * texture = NULL;

    // Application state
    bool quit = false;

    //Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    // Initialize SDL_image
    IMG_Init(IMG_INIT_PNG);

    //Create window
    window = SDL_CreateWindow(
        "SDL Tutorial",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if( window == NULL )
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return 1;
    }

    image = IMG_Load("sprites/spritesheet.png");
    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTextureFromSurface(renderer, image);

    while (!quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
        }

        // Copy a portion of the texture
        // to the current rendering target.
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        // Update the screen
        SDL_RenderPresent(renderer);
    }
    
    // Free memory
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    //Quit SDL, IMG subsystems
    IMG_Quit();
    SDL_Quit();

    return 0;
}
