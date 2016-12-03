// Using SDL and standard IO
#include <SDL2/SDL.h>
// Using SDL2_image
#include <SDL2/SDL_image.h>
#include <stdio.h>
// tmx c lib
#include <tmx.h>
#include "utils.h"

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct Character {
    int x_pos;
    int y_pos;
    float angle;
    // the flip of the texture.
    SDL_RendererFlip flip;
    // center of character texture
    SDL_Point center;
    bool is_moving;
};

int main( int argc, char* args[] )
{
    // The window we'll be rendering to
    SDL_Window* window = NULL;
    // SDL Event handler
    SDL_Event event;
    //The surface contained by the window
    SDL_Surface* image = NULL;
    // 2D Rendering Context for Window
    SDL_Renderer * renderer = NULL;
    // Create Textture from existing Surface
    SDL_Texture * texture = NULL;
    // load tmx-tiles
    tmx_map *map = tmx_load("tiles/base.tmx");
    SDL_Texture *map_bmp;

    if (!map) {
        tmx_perror("tmx_load");
        return 1;
    }

    // init map pos
    SDL_Rect map_rect;
    map_rect.w = map->width  * map->tile_width;
    map_rect.h = map->height * map->tile_height;
    map_rect.x = 0;  map_rect.y = 0;

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
    // turn right by default
    character.angle = 0.0f;
    character.flip = SDL_FLIP_NONE;
    // the center where the texture will be rotated.
    character.center = {16, 32};
    character.is_moving = false;

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

    if (!(map_bmp = render_map(map, renderer)))
        SDL_GetError();

    Uint32 sprite;
    while (!quit)
    {
        // SDL_GetTicks() gives us the number of milliseconds
        // that passed since the program started

        if (character.is_moving) {
            Uint32 ticks = SDL_GetTicks();
            sprite = (ticks / 200) % 4;
        } else {
            sprite = 0;
        }

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
                    character.is_moving = true;
                    // key press handler
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_LEFT:
                            character.angle = 180.0f;
                            character.flip = SDL_FLIP_VERTICAL;
                            character.x_pos -= 5;
                            break;
                        case SDLK_RIGHT:
                            character.angle = 0.0f;
                            character.flip = SDL_FLIP_NONE;
                            character.x_pos += 5;
                            break;
                        case SDLK_UP:
                            break;
                        case SDLK_DOWN:
                            break;
                    }
                    break;
            }

            if (event.type != SDL_KEYDOWN)
            {
                character.is_moving = false;
            }
        }

        // Clearing context we drew before
        //SDL_RenderClear(renderer);
        // Copy a portion of the texture
        // to the current rendering target.
        //SDL_RenderCopyEx(
        //    renderer, texture, &srcrect, &dstrect,
        //    character.angle, &character.center, character.flip
        //);
        // Update the screen
        //SDL_RenderPresent(renderer);
        // Sleep 20 ms
        SDL_Delay(20);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, map_bmp, NULL, &map_rect);
        SDL_RenderPresent(renderer);
    }
    
    // Free memory
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // free tmx map
    tmx_map_free(map);

    //Quit SDL, IMG subsystems
    IMG_Quit();
    SDL_Quit();

    return 0;
}
