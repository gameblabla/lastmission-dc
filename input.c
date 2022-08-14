/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS for(int A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
 
#if defined(DREAMCAST)
#include <kos.h>
#include <string.h>
long button_to_press;
struct button_dc
{
	unsigned char time;
	unsigned char state;
} buttons[7];
maple_device_t* cont;
cont_state_t* state;
#endif

#include <SDL/SDL.h>

#include "video.h"
#include "input.h"
#include "engine.h"

/* game keys */
unsigned char GKeys[7]; // left, right, up, down, fire, pause, quit

/* system keys */
unsigned char Keys[128] = {0};

void LM_ResetKeys()
{
	memset(&Keys[0], 0, 128);
}

int LM_AnyKey()
{
	for (int i = 0; i < 127; i++) {
		if (Keys[i] == 1)
			return 1;
	}

	return 0;
}

char LM_PollEvents()
{
#ifdef DREAMCAST
	int key_scan;
	int key_value;
	unsigned char j;
	
	key_scan = -1;
	key_value = 0;
	
	Keys[SC_UP] = 0;
	Keys[SC_DOWN] = 0;
	Keys[SC_LEFT] = 0;
	Keys[SC_RIGHT] = 0;
	Keys[SC_SPACE] = 0;
	Keys[SC_ENTER] = 0;
	
	cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
	if (cont) state = (cont_state_t *) maple_dev_status(cont);
	
	for (j=0;j<6;j++)
	{	
		if (j==0) button_to_press = CONT_DPAD_UP;
		else if (j==1) button_to_press = CONT_DPAD_DOWN;
		else if (j==2) button_to_press = CONT_DPAD_LEFT;
		else if (j==3) button_to_press = CONT_DPAD_RIGHT;
		else if (j==4) button_to_press = CONT_START;
		else button_to_press = CONT_A;
				
		switch (buttons[j].state)
		{
			case 0:
				if (state->buttons & button_to_press)
				{
					buttons[j].state = 1;
					buttons[j].time = 0;
				}
			break;
				
			case 1:
				buttons[j].time++;
					
				if (buttons[j].time > 0)
				{
					buttons[j].state = 2;
					buttons[j].time = 0;
				}
			break;
				
			case 2:
				if (!(state->buttons & button_to_press))
				{
					buttons[j].state = 3;
					buttons[j].time = 0;
				}
			break;
				
			case 3:
				buttons[j].time++;
					
				if (buttons[j].time > 0)
				{
					buttons[j].state = 0;
					buttons[j].time = 0;
				}
			break;
		}   
	}
	
	if (buttons[0].state == 2)
	{
		key_scan = SC_UP;
		key_value = 1;
	}
	if (buttons[1].state == 2)
	{
		key_scan = SC_DOWN;
		key_value = 1;
	}  

	if (buttons[3].state == 2)
	{
		key_scan = SC_RIGHT;
		key_value = 1;
	}
	else if (buttons[2].state == 2)
	{
		key_scan = SC_LEFT;
		key_value = 1;
	}
	if (buttons[3].state == 2)
	{
		key_scan = SC_RIGHT;
		key_value = 1;
	}
	
	if (buttons[5].state == 2)
	{
		Keys[SC_SPACE] = 1;
	}
	
	if (buttons[4].state == 1)
	{
		key_scan = SC_ENTER;
		key_value = 1;
	}
	
	if (buttons[2].state == 2 && buttons[0].state == 2)
	{
		Keys[SC_LEFT] = 1;
		Keys[SC_UP] = 1;
	}
	else if (buttons[3].state == 2 && buttons[0].state == 2)
	{
		Keys[SC_RIGHT] = 1;
		Keys[SC_UP] = 1;
	}
	
	if (buttons[2].state == 2 && buttons[1].state == 2)
	{
		Keys[SC_LEFT] = 1;
		Keys[SC_DOWN] = 1;
	}
	else if (buttons[3].state == 2 && buttons[1].state == 2)
	{
		Keys[SC_RIGHT] = 1;
		Keys[SC_DOWN] = 1;
	}
	
	if (key_scan != -1)
	{
		Keys[key_scan] = key_value;
	}
			
#else
	SDL_Event event;

	while (SDL_PollEvent(&event) != 0) {
		int key_scan = -1;
		unsigned char key_value = 0;

		if (event.type == SDL_QUIT) {
			Keys[SC_ESCAPE] = 1;
			return 1;
		}

		// unix and dingoo sdl don't have scancodes, so remap usual keys
		if (event.type == SDL_KEYDOWN)
			key_value = 1;
		if (event.type == SDL_KEYUP)
			key_value = 0;

		// Emulate x86 scancodes
		if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
			switch (event.key.keysym.sym) {
				case SDLK_UP:
					key_scan = SC_UP;
					break;
				case SDLK_DOWN:
					key_scan = SC_DOWN;
					break;
				case SDLK_LEFT:
					key_scan = SC_LEFT;
					break;
				case SDLK_RIGHT:
					key_scan = SC_RIGHT;
					break;
				case SDLK_ESCAPE:
					key_scan = SC_ESCAPE;
					break;
				case SDLK_RETURN: // ENTER
					key_scan = SC_ENTER;
					break;
				case SDLK_LCTRL:
					key_scan = SC_SPACE;
					break;
				case SDLK_LALT:
				case SDLK_LSHIFT:
				case SDLK_SPACE:
					key_scan = SC_SPACE;
					break;
				case SDLK_s:
					key_scan = SC_S;
					break;
				case SDLK_f:
					key_scan = SC_F;
					break;
				case SDLK_TAB: // LEFT SHOULDER
					key_scan = SC_TAB;
					break;
				case SDLK_BACKSPACE: // RIGHT SHOULDER
					key_scan = SC_BACKSPACE;
					break;
				default:; // maybe use in future
					break;
			}
			if (key_scan != -1)
				Keys[key_scan] = key_value;
		}
	}
#endif

	return 0;
}
