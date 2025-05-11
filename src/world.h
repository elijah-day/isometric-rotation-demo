#ifndef WORLD_H
#define WORLD_H

#include <SDL3/SDL.h>

#define WORLD_TILE_ARRAY_LENGTH 16
#define WORLD_WALL_ARRAY_LENGTH 4
#define WORLD_ENTITY_ARRAY_LENGTH 3

typedef struct entity_t
{
	SDL_FRect dstrect;
	SDL_FRect srcrect;
	float angle;
	float x;
	float y;
	int texture_id;
}
entity_t;

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

typedef struct wall_t
{
	SDL_FRect dstrect;
	float x;
	float y;
	int texture_id;
}
wall_t;

typedef struct world_t
{
	entity_t entity_array[WORLD_ENTITY_ARRAY_LENGTH];
	entity_t entity_z_buffer[WORLD_ENTITY_ARRAY_LENGTH];
	tile_t tile_array[WORLD_TILE_ARRAY_LENGTH];
	wall_t wall_array[WORLD_WALL_ARRAY_LENGTH];
}
world_t;

#endif /* WORLD_H */
