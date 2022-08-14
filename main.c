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
#ifdef _TINSPIRE
#include <os.h>
#include <libndls.h>
#endif

#include <SDL/SDL.h>
#include "video.h"
#include "sound.h"
#include "engine.h"

int main(int argc, char* argv[])
{
#ifdef _TINSPIRE
	enable_relative_paths(argv);
#endif
	
	if (LM_Init() == 0)
		return -1;
#ifdef SOUND
	LM_SND_Init();
#endif
	GameLoop();

#ifdef SOUND
	LM_SND_Deinit();
#endif
	LM_Deinit();

	SDL_Quit();
	return 0;
}
