#include "gfx.h"
#include "world.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

bool initialize_gfx(gfx_t *gfx)
{
	if(SDL_InitSubSystem(SDL_INIT_VIDEO) == false)
	{
		printf("SDL_InitSubSystem error: %s\n", SDL_GetError());
		return false;
	}
	
	gfx->window = NULL;
	gfx->window = SDL_CreateWindow("", 1280, 720, SDL_WINDOW_OPENGL);
	
	if(gfx->window == NULL)
	{
		printf("SDL_CreateWindow error: %s\n", SDL_GetError());
		return false;
	}
	
	gfx->renderer = NULL;
	gfx->renderer = SDL_CreateRenderer(gfx->window, NULL);
	
	if(gfx->renderer == NULL)
	{
		printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
		return false;
	}
	
	gfx->camera.angle = 0;
	gfx->camera.x = 0;
	gfx->camera.y = 0;
	
	SDL_SetRenderDrawColor(gfx->renderer, 63, 127, 127, 0);
	
	return true;
}

void initialize_dstrect_array(gfx_t *gfx, world_t *world)
{
	for(int i = 0; i < WORLD_TILE_ARRAY_LENGTH; i++)
	{
		if(gfx->texture_array[world->tile_array[i].texture_id] == NULL)
		{
			continue;
		}
	
		world->tile_array[i].dstrect.x = world->tile_array[i].x;
		world->tile_array[i].dstrect.y = world->tile_array[i].y;
		
		world->tile_array[i].dstrect.w = 
			gfx->texture_array[world->tile_array[i].texture_id]->w;
			
		world->tile_array[i].dstrect.h = 
			gfx->texture_array[world->tile_array[i].texture_id]->h;
	}
	
	for(int i = 0; i < WORLD_WALL_ARRAY_LENGTH; i++)
	{
		if(gfx->texture_array[world->wall_array[i].texture_id] == NULL)
		{
			continue;
		}
	
		world->wall_array[i].dstrect.x = world->wall_array[i].x;
		world->wall_array[i].dstrect.y = world->wall_array[i].y;
		
		world->wall_array[i].dstrect.w = 
			gfx->texture_array[world->wall_array[i].texture_id]->w / GFX_TILE_W;
			
		world->wall_array[i].dstrect.h = 
			gfx->texture_array[world->wall_array[i].texture_id]->h;
	}
	
	for(int i = 0; i < WORLD_ENTITY_ARRAY_LENGTH; i++)
	{
		if(gfx->texture_array[world->entity_array[i].texture_id] == NULL)
		{
			continue;
		}
	
		world->entity_array[i].dstrect.x = world->wall_array[i].x;
		world->entity_array[i].dstrect.y = world->wall_array[i].y;
		
		world->entity_array[i].dstrect.w = 
			gfx->texture_array[world->entity_array[i].texture_id]->w;
			
		world->entity_array[i].dstrect.h = 
			gfx->texture_array[world->entity_array[i].texture_id]->h;
	}
}

void load_texture_array(const char *texture_path_array[], gfx_t *gfx)
{
	/* TODO: The texture path array needs to be in a struct or something with a
	listed length... */
	int texture_path_array_length = 5;

	for(int i = 0; i < GFX_TEXTURE_ARRAY_LENGTH; i++)
	{
		if(i > texture_path_array_length - 1)
		{
			gfx->texture_array[i] = NULL;
		}
		else
		{
			gfx->texture_array[i] = IMG_LoadTexture
			(
				gfx->renderer,
				texture_path_array[i]
			);
			
			if(gfx->texture_array[i] == NULL)
			{
				printf("IMG_LoadTexture error: %s\n", SDL_GetError());
			}
		}
	}
}

void render_world(gfx_t *gfx, world_t *world)
{
	SDL_RenderClear(gfx->renderer);
	SDL_SetRenderScale(gfx->renderer, 4, 2);
	
	/* Where "rm" indicates "rotation matrix." */
	float rm_cos = cos(GFX_DEGREES_TO_RADIANS * gfx->camera.angle);
	float rm_sin = sin(GFX_DEGREES_TO_RADIANS * gfx->camera.angle);
	
	for(int i = 0; i < WORLD_TILE_ARRAY_LENGTH; i++)
	{
		/* NOTE: Cannot effectively do rotation on variably-sized objects at the
		moment... stick with fixed tiles instead?  How will this affect entity
		rendering?  (Do we need fixed-size entities??) */
		
		world->tile_array[i].dstrect.x =
			(world->tile_array[i].x - gfx->camera.x) * rm_cos -
			(world->tile_array[i].y - gfx->camera.y) * rm_sin +
			0.25 * gfx->camera.hw - GFX_TILE_HW;
			
		/* 16 needed to account for tile width... 0.25 multiplier to account for
		the render scaling of 2:1. */
		world->tile_array[i].dstrect.y =
			(world->tile_array[i].x - gfx->camera.x) * rm_sin +
			(world->tile_array[i].y - gfx->camera.y) * rm_cos +
			0.5 * gfx->camera.hh - GFX_TILE_HW;
	
		SDL_RenderTextureRotated
		(
			gfx->renderer,
			gfx->texture_array[world->tile_array[i].texture_id],
			NULL,
			&world->tile_array[i].dstrect,
			gfx->camera.angle,
			NULL,
			SDL_FLIP_NONE
		);
	}
	
	SDL_SetRenderScale(gfx->renderer, 4, 4);
	
	for(int i = 0; i < WORLD_WALL_ARRAY_LENGTH; i++)
	{
		SDL_FRect srcrect = {0, 0, 1, GFX_WALL_H};
		
		/* Render "slivers" of wall so that we can do it at an angle. */
		for(int j = 0; j < GFX_TILE_W; j++)
		{		
			world->wall_array[i].dstrect.x =
				(world->wall_array[i].x - gfx->camera.x) * rm_cos -
				(world->wall_array[i].y - gfx->camera.y) * rm_sin +
				0.25 * gfx->camera.hw - GFX_TILE_HW -
				
				/* Subtract this line to account for different distances taken
				by rotating objects of different sizes. */
				world->wall_array[i].y * rm_sin +
				
				/* Render slivers of wall with horizontal offset. */
				j * rm_cos +
				
				/* Offset to center wall for rotation. */
				GFX_TILE_HW;

			world->wall_array[i].dstrect.y =
				/* Multiply gfx->camera by 2 to account for tiles being half the
				height of everything else and needing to compensate in
				distance. */
				(world->wall_array[i].x - 0.5 * gfx->camera.x) * rm_sin +
				(world->wall_array[i].y - 0.5 * gfx->camera.y) * rm_cos +
				0.25 * gfx->camera.hh - GFX_WALL_HH -
				
				/* Subtract this line to account for different distances taken
				by rotating objects of different sizes. */
				0.5 * world->wall_array[i].x * rm_sin +
				
				/* Render slivers of wall with vertical offset.  It is
				multiplied by 0.5 to account for tile height being half of the
				wall height. */
				0.5 * j * rm_sin -
				
				/* Offset to center wall for rotation. */
				GFX_WALL_HH;
				
			SDL_RenderTexture
			(
				gfx->renderer,
				gfx->texture_array[world->wall_array[i].texture_id],
				&srcrect,
				&world->wall_array[i].dstrect
			);
			
			/* This needs to be incremented to move to the next "sliver" of wall
			to be rendered. */
			srcrect.x += 1;
		}
	}
	
	for(int i = 0; i < WORLD_ENTITY_ARRAY_LENGTH; i++)
	{
		world->entity_array[i].dstrect.x =
			(world->entity_array[i].x - gfx->camera.x) * rm_cos -
			(world->entity_array[i].y - gfx->camera.y) * rm_sin +
			0.25 * gfx->camera.hw - GFX_TILE_HW -
			world->entity_array[i].y * rm_sin;
	
		world->entity_array[i].dstrect.y =
			/* Multiply gfx->camera by 2 to account for tiles being half the
			height of everything else and needing to compensate in
			distance. */
			(world->entity_array[i].x - 0.5 * gfx->camera.x) * rm_sin +
			(world->entity_array[i].y - 0.5 * gfx->camera.y) * rm_cos +
			0.25 * gfx->camera.hh - GFX_TILE_W -
			0.5 * world->entity_array[i].x * rm_sin;
			
		SDL_RenderTexture
		(
			gfx->renderer,
			gfx->texture_array[world->entity_array[i].texture_id],
			NULL,
			&world->entity_array[i].dstrect
		);
	}
	
	SDL_RenderPresent(gfx->renderer);
}

void terminate_gfx(gfx_t *gfx)
{
	SDL_DestroyRenderer(gfx->renderer);
	SDL_DestroyWindow(gfx->window);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void unload_texture_array(gfx_t *gfx)
{
	for(int i = 0; i < GFX_TEXTURE_ARRAY_LENGTH; i++)
	{	
		SDL_DestroyTexture(gfx->texture_array[i]);
	}
}

void update_camera_dimensions(gfx_t *gfx)
{
	int window_w;
	int window_h;
	SDL_GetWindowSize(gfx->window, &window_w, &window_h);
	
	gfx->camera.w = (float)window_w;
	gfx->camera.h = (float)window_h;
	
	gfx->camera.hw = gfx->camera.w / 2;
	gfx->camera.hh = gfx->camera.h / 2;
}
