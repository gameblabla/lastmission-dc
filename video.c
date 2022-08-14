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

/*
 * Note: define __DINGUX__ to build for low resolutions like 320x240
 */
#include <SDL/SDL.h>

#include "video.h"
#include "input.h"
#include "engine.h"


int LM_GFX_Init();
void LM_GFX_Deinit();
void LM_GFX_SetScale(int scale);

SDL_Surface *small_screen = NULL;

int LM_Init()
{
	if (LM_GFX_Init() == 0)
		return 0;

	return 1;
}

void LM_Deinit()
{
	LM_GFX_Deinit();
}

int LM_GFX_Init()
{

#if defined(SOUND)
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);
#else
	SDL_Init(SDL_INIT_VIDEO);
#endif

#ifdef _TINSPIRE
	small_screen = SDL_SetVideoMode(320, 200, has_colors ? 16 : 8, SDL_SWSURFACE);
#elif defined(DREAMCAST)
	small_screen = SDL_SetVideoMode(320, 200, 16, SDL_SWSURFACE);
#else
	small_screen = SDL_SetVideoMode(320, 200, 16, SDL_SWSURFACE);
#endif
	
	SDL_ShowCursor(0);

	//Set the window caption
	SDL_WM_SetCaption("The Last Mission SDL remake", NULL);

	return 1;
}

void LM_GFX_Deinit()
{
	extern SDL_Surface* tiles;
	extern SDL_Surface* sprites;
	extern SDL_Surface* logo;
	extern SDL_Surface* small_screen;
	SDL_FreeSurface(sprites);
	SDL_FreeSurface(tiles);
	SDL_FreeSurface(logo);
	SDL_FreeSurface(small_screen);
}

void LM_GFX_Flip()
{
	SDL_Flip(small_screen);
}

void LM_GFX_SetScale(int scale)
{
}
