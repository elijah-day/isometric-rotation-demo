#include "gfx.h"
#include "input.h"
#include "world.h"
#include <SDL3/SDL.h>
#include <math.h>

/* TODO: Have array listed of "core textures" that must be loaded often (or kept
loaded) for common things like menu, UI, etc.  Keep separate from map asset
textures...

We *could* simply have two different texture arrays for each set of textures,
but it may be more advantageous to simply use a single array for all textures,
where the first half is the core textures, and the second half is the variable
textures. */

const char *texture_path_array[] =
{
	"data/tile1.png",
	"data/tile2.png",
	"data/wall1.png",
	"data/wall2.png",
	"data/red_barrel.png",
	"data/g013m.png"
};

int main(/* int argc, char *argv[] */)
{
	input_t input;
	gfx_t gfx;
	
	/* Tiles should have normalized spacing... not by pixels: "32, 64, etc..."
	
	We should be able to go up by increments of "1, 2, 3..." */
	
	world_t world =
	{
		{
			{{0, 0, 0, 0}, {0, 0, 0, 0}, 0, 36, 18, 4},
			{{0, 0, 0, 0}, {0, 0, 0, 0}, 0, 50, 16, 4},
			{{0, 0, 0, 0}, {0, 0, 0, 0}, 0, 40, 48, 5},
		},
		{
			{{0, 0, 0, 0}, {0, 0, 0, 0}, 0, 0, 0, 0},
		},
		{
			{{0, 0, 0, 0}, 0, 0, 1}, 
			{{0, 0, 0, 0}, 32, 0, 0},
			{{0, 0, 0, 0}, 64, 0, 0}, 
			{{0, 0, 0, 0}, 96, 0, 1},
			{{0, 0, 0, 0}, 0,  32, 1}, 
			{{0, 0, 0, 0}, 32, 32, 0},
			{{0, 0, 0, 0}, 64, 32, 1}, 
			{{0, 0, 0, 0}, 96, 32, 1},
			{{0, 0, 0, 0}, 64, 128, 1}, 
			{{0, 0, 0, 0}, 96, 128, 0},
			{{0, 0, 0, 0}, 128, 128, 0}, 
			{{0, 0, 0, 0}, 128, 96, 1},
			{{0, 0, 0, 0}, 96, 96, 1}, 
			{{0, 0, 0, 0}, 64, 96, 1},
			{{0, 0, 0, 0}, 32, 96, 1}, 
			{{0, 0, 0, 0}, 0, 96, 1},
		},
		{
			{{0, 0, 0, 0}, 64, -16, -8, 2},
			{{0, 0, 0, 0}, 32, 48, -8, 3},
			{{0, 0, 0, 0}, 96, 80, -8, 2},
		},
	};
	
	initialize_input(&input);
	initialize_gfx(&gfx);
	update_camera_dimensions(&gfx);
	load_texture_array(texture_path_array, &gfx);
	initialize_rect_array(&gfx, &world);
	
	/* This is needed to initialize the z buffer for the first time before any
	camera rotation is activated. */
	sort_entity_z_buffer(&gfx, &world);
	
	while(input.program_running)
	{
		poll_input(&input);
		render_world(&gfx, &world);
		
		/* FIXME: Temporary camera control... fix this later! */
		
		/* Where "rm" indicates "rotation matrix." */
		float rm_cos = cos(GFX_DEGREES_TO_RADIANS * gfx.camera.angle);
		float rm_sin = sin(GFX_DEGREES_TO_RADIANS * gfx.camera.angle);
		
		if(input.keys_pressed[SDL_SCANCODE_W])
		{
			gfx.camera.x -= 1 * rm_sin;
			gfx.camera.y -= 1 * rm_cos;
		}
		if(input.keys_pressed[SDL_SCANCODE_A])
		{
			gfx.camera.x -= 1 * rm_cos;
			gfx.camera.y += 1 * rm_sin;
		}
		if(input.keys_pressed[SDL_SCANCODE_S])
		{
			gfx.camera.x += 1 * rm_sin;
			gfx.camera.y += 1 * rm_cos;
		}
		if(input.keys_pressed[SDL_SCANCODE_D])
		{
			gfx.camera.x += 1 * rm_cos;
			gfx.camera.y -= 1 * rm_sin;
		}
		
		if(input.keys_pressed[SDL_SCANCODE_LEFT])
		{
			gfx.camera.angle -= 1;
			sort_entity_z_buffer(&gfx, &world);
			update_z_buffer_texture_directions(&gfx, &world);
		}
		if(input.keys_pressed[SDL_SCANCODE_RIGHT])
		{
			gfx.camera.angle += 1;
			sort_entity_z_buffer(&gfx, &world);
			update_z_buffer_texture_directions(&gfx, &world);
		}
		
		if(gfx.camera.angle > 359)
		{
			gfx.camera.angle = 0;
		}
		if(gfx.camera.angle < 0)
		{
			gfx.camera.angle = 359;
		}
		
		SDL_Delay(4);
	}
	
	unload_texture_array(&gfx);
	terminate_gfx(&gfx);
	return 0;
}
