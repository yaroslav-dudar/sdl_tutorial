#pragma once

void set_color(int color, SDL_Renderer *renderer);
void* sdl_img_loader(const char *path, SDL_Renderer *renderer);
void draw_polyline(double **points, double x, double y, int pointsc, SDL_Renderer *renderer);
void draw_polygon(double **points, double x, double y, int pointsc, SDL_Renderer *renderer);
void draw_objects(tmx_object_group *objgr, SDL_Renderer *renderer);
unsigned int gid_clear_flags(unsigned int gid);
void draw_layer(tmx_map *map, tmx_layer *layer, SDL_Renderer *renderer);
void draw_image_layer(tmx_image *img, SDL_Renderer *renderer);
SDL_Texture* render_map(tmx_map *map, SDL_Renderer *renderer);
