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
		world->tile_array[i].dstrect.x = world->tile_array[i].x;
		world->tile_array[i].dstrect.y = world->tile_array[i].y;
		
		world->tile_array[i].dstrect.w = 
			gfx->texture_array[world->tile_array[i].texture_id]->w;
			
		world->tile_array[i].dstrect.h = 
			gfx->texture_array[world->tile_array[i].texture_id]->h;
	}
}

void load_texture_array(const char *texture_path_array[], gfx_t *gfx)
{
	/* TODO: The texture path array needs to be in a struct or something with a
	listed length... */
	int texture_path_array_length = 2;

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
			0.25 * gfx->camera.hw - 16;
			
		/* 16 needed to account for tile width... 0.25 multiplier to account for
		the render scaling of 2:1... */
			
		world->tile_array[i].dstrect.y =
			(world->tile_array[i].x - gfx->camera.x) * rm_sin +
			(world->tile_array[i].y - gfx->camera.y) * rm_cos +
			0.5 * gfx->camera.hh - 16;
	
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
