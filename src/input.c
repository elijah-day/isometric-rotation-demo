#include "input.h"

#include <SDL3/SDL.h>
#include <stdbool.h>
#include <string.h>

static SDL_Event event;

void initialize_input(input_t *input)
{
	input->program_running = true;
	memset(input->keys_pressed, 0, INPUT_KEY_COUNT * sizeof(bool));
}

void poll_input(input_t *input)
{
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_EVENT_KEY_DOWN:
				input->keys_pressed[event.key.scancode] = true;
				break;
				
			case SDL_EVENT_KEY_UP:
				input->keys_pressed[event.key.scancode] = false;
				break;
		
			case SDL_EVENT_QUIT:
				input->program_running = false;
				break;
		}
	}
}
