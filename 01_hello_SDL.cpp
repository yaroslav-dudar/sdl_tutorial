// Using SDL and standard IO
#include <SDL2/SDL.h>
// Using SDL2_image
#include <SDL2/SDL_image.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct Character {
    int x_pos;
    int y_pos;
};

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

    // initialzie character
    struct Character character;
    character.x_pos = 10;
    character.y_pos = 10;

    if( window == NULL )
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return 1;
    }

    image = IMG_Load("sprites/spritesheet.png");
    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTextureFromSurface(renderer, image);

    // Set the color used for drawing operations
    SDL_SetRenderDrawColor(renderer, 168, 230, 255, 255);
    SDL_RenderClear(renderer);

    while (!quit)
    {
        // SDL_GetTicks() gives us the number of milliseconds
        // that passed since the program started
        Uint32 ticks = SDL_GetTicks();
        Uint32 sprite = (ticks / 100) % 4;

        SDL_Rect srcrect = { sprite * 32, 0, 32, 64 };
        SDL_Rect dstrect = {
            character.x_pos,
            character.y_pos,
            32, 64
        };

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_KEYDOWN:
                    // key press handler
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_LEFT:
                            character.x_pos -= 5;
                            break;
                        case SDLK_RIGHT:
                            character.x_pos += 5;
                            break;
                        case SDLK_UP:
                            break;
                        case SDLK_DOWN:
                            break;
                    }
                    break;
            }
        }

        // Clearing context we drew before
        SDL_RenderClear(renderer);
        // Copy a portion of the texture
        // to the current rendering target.
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
        // Update the screen
        SDL_RenderPresent(renderer);
        // Sleep 20 ms
        SDL_Delay(20);
        //SDL_Log("SDL_Log: Hello World\n");
        // Flush user events
        SDL_FlushEvents(SDL_USEREVENT, SDL_LASTEVENT);
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
