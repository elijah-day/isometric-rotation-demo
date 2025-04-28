#ifndef WORLD_H
#define WORLD_H

#include <SDL3/SDL.h>

#define WORLD_TILE_ARRAY_LENGTH 16

/* TODO: Should probably just store tile data in an array and have a shared rect 
for most/all tiles... */
typedef struct tile_t
{
	SDL_FRect dstrect;
	float x;
	float y;
	int texture_id;
}
tile_t;

typedef struct world_t
{
	tile_t tile_array[WORLD_TILE_ARRAY_LENGTH];
}
world_t;

#endif /* WORLD_H */
