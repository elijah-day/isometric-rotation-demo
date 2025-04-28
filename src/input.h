#ifndef INPUT_H
#define INPUT_H

#define INPUT_KEY_COUNT 322

#include <stdbool.h>

typedef struct input_t
{
	bool keys_pressed[INPUT_KEY_COUNT];
	bool program_running;
}
input_t;

void initialize_input(input_t *input);
void poll_input(input_t *input);

#endif /* INPUT_H */
