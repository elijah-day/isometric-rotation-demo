#ifndef GFX_H
#define GFX_H

#include "world.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

#define GFX_DEGREES_TO_RADIANS 3.14159 / 180
#define GFX_TEXTURE_ARRAY_LENGTH 8

typedef struct camera_t
{
	float angle;
	float x;
	float y;
	float w;
	float h;
	float hw; /* Half of the camera width */
	float hh; /* Half of the camera height */
}
camera_t;

/* TODO: Should rects be bundled with world objects or gfx/textures?

On one hand, world objects have the x and y coordinates of where we need to
draw.  We don't want to have to set those in the render function many times...

On the other, textures are closely related to rects with rendering...

We could always just implement adding or subtracting x and y values in movement
functions. */

typedef struct gfx_t
{
	SDL_Texture *texture_array[GFX_TEXTURE_ARRAY_LENGTH];
	SDL_Renderer *renderer;
	SDL_Window *window;
	camera_t camera;
}
gfx_t;

bool initialize_gfx(gfx_t *gfx);
void initialize_dstrect_array(gfx_t *gfx, world_t *world);
void load_texture_array(const char *texture_path_array[], gfx_t *gfx);
void render_world(gfx_t *gfx, world_t *world);
void terminate_gfx(gfx_t *gfx);
void unload_texture_array(gfx_t *gfx);
void update_camera_dimensions(gfx_t *gfx);

#endif /* GFX_H */
