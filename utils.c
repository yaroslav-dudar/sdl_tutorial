#include <tmx.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define fatal_error(str) { fputs(str, stderr); return NULL; }
#define str_bool(b) (b==0? "false": "true")

void set_color(int color, SDL_Renderer *renderer) {
    unsigned char r, g, b;

    r = (color >> 16) & 0xFF;
    g = (color >>  8) & 0xFF;
    b = (color)       & 0xFF;

    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
}

void* sdl_img_loader(const char *path, SDL_Renderer *renderer) {
    return IMG_LoadTexture(renderer, path);
}

void draw_polyline(double **points, double x, double y, int pointsc, SDL_Renderer *renderer) {
    int i;
    for (i=1; i<pointsc; i++) {
        SDL_RenderDrawLine(
            renderer, x+points[i-1][0], y+points[i-1][1],
            x+points[i][0], y+points[i][1]
        );
    }
}

void draw_polygon(double **points, double x, double y, int pointsc, SDL_Renderer *renderer) {
    draw_polyline(points, x, y, pointsc, renderer);
    if (pointsc > 2) {
        SDL_RenderDrawLine(
            renderer, x+points[0][0], y+points[0][1],
            x+points[pointsc-1][0], y+points[pointsc-1][1]
        );
    }
}

void draw_objects(tmx_object_group *objgr, SDL_Renderer *renderer) {
    SDL_Rect rect;
    set_color(objgr->color, renderer);
    tmx_object *head = objgr->head;
    /* FIXME line thickness */
    while (head) {
        if (head->visible) {
            if (head->shape == S_SQUARE) {
                rect.x =     head->x;  rect.y =      head->y;
                rect.w = head->width;  rect.h = head->height;
                SDL_RenderDrawRect(renderer, &rect);
            } else if (head->shape  == S_POLYGON) {
                draw_polygon(head->points, head->x, head->y, head->points_len, renderer);
            } else if (head->shape == S_POLYLINE) {
                draw_polyline(head->points, head->x, head->y, head->points_len, renderer);
            } else if (head->shape == S_ELLIPSE) {
                /* FIXME: no function in SDL2 */
            }
        }
        head = head->next;
    }
}

unsigned int gid_clear_flags(unsigned int gid) {
    return gid & TMX_FLIP_BITS_REMOVAL;
}

void draw_layer(tmx_map *map, tmx_layer *layer, SDL_Renderer *renderer) {
    unsigned long i, j;
    unsigned int gid;
    float op;
    tmx_tileset *ts;
    tmx_image *im;
    SDL_Rect srcrect, dstrect;
    SDL_Texture* tileset;
    op = layer->opacity;
    for (i=0; i<map->height; i++) {
        for (j=0; j<map->width; j++) {
            gid = gid_clear_flags(layer->content.gids[(i*map->width)+j]);
            if (map->tiles[gid] != NULL) {
                ts = map->tiles[gid]->tileset;
                im = map->tiles[gid]->image;
                srcrect.x = map->tiles[gid]->ul_x;
                srcrect.y = map->tiles[gid]->ul_y;
                srcrect.w = dstrect.w = ts->tile_width;
                srcrect.h = dstrect.h = ts->tile_height;
                dstrect.x = j*ts->tile_width;  dstrect.y = (i-1)*ts->tile_height;
                /* TODO Opacity and Flips */
                if (im) {
                    tileset = (SDL_Texture*)im->source;
                }
                else {
                    tileset = IMG_LoadTexture(renderer, ts->image->source);
                    printf("\n\t" "source=%s", ts->image->source);
                }
                SDL_RenderCopy(renderer, tileset, &srcrect, &dstrect);
            }
        }
    }
}

void draw_image_layer(tmx_image *img, SDL_Renderer *renderer) {
    SDL_Rect dim;

    dim.x = dim.y = 0;
    SDL_QueryTexture((SDL_Texture*)img->resource_image, NULL, NULL, &(dim.w), &(dim.h));

    SDL_RenderCopy(renderer, (SDL_Texture*)img->resource_image, NULL, &dim);

}

SDL_Texture* render_map(tmx_map *map, SDL_Renderer *renderer) {
    SDL_Texture *res;
    tmx_layer *layers = map->ly_head;
    int w, h;

    w = map->width  * map->tile_width;  /* Bitmap's width and height */
    h = map->height * map->tile_height;

    if (!(res = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h)))
        fatal_error(SDL_GetError());
    SDL_SetRenderTarget(renderer, res);

    set_color(map->backgroundcolor, renderer);
    SDL_RenderClear(renderer);

    while (layers) {
        if (layers->visible) {
            //printf("%d (%d,%d,%d)\n", layers->type, L_OBJGR, L_IMAGE, L_LAYER);
            if (layers->type == L_OBJGR) {
                draw_objects(layers->content.objgr, renderer);
            } else if (layers->type == L_IMAGE) {
                draw_image_layer(layers->content.image, renderer);
            } else if (layers->type == L_LAYER) {
                draw_layer(map, layers, renderer);
            }
        }
        layers = layers->next;
    }

    SDL_SetRenderTarget(renderer, NULL);
    return res;
}
