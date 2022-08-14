#if !defined(SOUND)
	typedef int (Mix_Chunk)(int);
	typedef int (Mix_Music)(int);
#endif
 
 
#ifdef _TINSPIRE
#include <os.h>
#endif

#include <SDL/SDL.h>
#if defined(SOUND) && !defined(DREAMCAST)
#include <SDL/SDL_mixer.h>
#endif

#include "sound.h"
#include "engine.h"

#ifdef DREAMCAST
#include <kos.h>
#include <string.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>
sfxhnd_t laser, short_laser, rocket_shot, cannon_shot, explosions[3], contact, elevator, bonus, move;
#elif defined(SOUND)
Mix_Chunk *laser, *short_laser, *rocket_shot, *cannon_shot, *explosions[3],
	*contact, *elevator, *bonus, *move;
Mix_Music *music_intro, *music_game;
#endif
	
int moveChannel = -1, elevatorChannel = -1;
int musicChannel = -1;

int isInitialized = 0;

void LoadSoundFiles()
{
#if defined(SOUND)

	if (isInitialized)
		return;

	isInitialized = 1;

#ifdef DREAMCAST
	bonus = snd_sfx_load("/cd/sound/bonus.wav");
	laser = snd_sfx_load("/cd/sound/laser.wav");
	short_laser = snd_sfx_load("/cd/sound/short_laser.wav");
	rocket_shot = snd_sfx_load("/cd/sound/rocket_shot.wav");
	cannon_shot = snd_sfx_load("/cd/sound/cannon_shot.wav");
	explosions[0] = snd_sfx_load("/cd/sound/explode0.wav");
	explosions[1] = snd_sfx_load("/cd/sound/explode1.wav");
	explosions[2] = snd_sfx_load("/cd/sound/explode2.wav");
	contact = snd_sfx_load("/cd/sound/contact.wav");
	/*move = snd_sfx_load("/cd/sound/move.wav");
	elevator = snd_sfx_load("/cd/sound/elevator.wav");*/
#else
	music_intro = Mix_LoadMUS("sound/music_intro.ogg");
	music_game = Mix_LoadMUS("sound/music.ogg");

	bonus = Mix_LoadWAV("sound/bonus.ogg");
	laser = Mix_LoadWAV("sound/laser.ogg");
	short_laser = Mix_LoadWAV("sound/short_laser.ogg");
	rocket_shot = Mix_LoadWAV("sound/rocket_shot.ogg");
	cannon_shot = Mix_LoadWAV("sound/cannon_shot.ogg");
	explosions[0] = Mix_LoadWAV("sound/explode0.ogg");
	explosions[1] = Mix_LoadWAV("sound/explode1.ogg");
	explosions[2] = Mix_LoadWAV("sound/explode2.ogg");
	contact = Mix_LoadWAV("sound/contact.ogg");
	move = Mix_LoadWAV("sound/move.ogg");
	elevator = Mix_LoadWAV("sound/elevator.ogg");
#endif	

#endif
}

void PlaySoundEffect(int sound)
{
#if defined(SOUND)

#ifdef DREAMCAST
	switch (sound) {
	case SND_LASER_SHOOT:
		snd_sfx_play(laser, 255, 0x80);
		break;
	case SND_CANNON_SHOOT:
		snd_sfx_play(cannon_shot, 255, 0x80);
		break;
	case SND_SHORT_LASER_SHOOT:
		snd_sfx_play(short_laser, 255, 0x80);
		break;
	case SND_ROCKET_SHOOT:
		snd_sfx_play(rocket_shot, 255, 0x80);
		break;
	case SND_EXPLODE:
		snd_sfx_play(explosions[rand() % 3], 255, 0x80);
		break;
	case SND_CONTACT:
		snd_sfx_play(contact, 255, 0x80);
		break;
	case SND_BONUS:
		snd_sfx_play(bonus, 255, 0x80);
		break;
	case SND_MOVE:
		/*if(moveChannel == -1 || !Mix_Playing(moveChannel)) {
			snd_sfx_play(move, 255, 0x80);
		}*/
		break;
	case SND_ELEVATOR:
		/*if(elevatorChannel == -1 || !Mix_Playing(elevatorChannel)) {
			snd_sfx_play(elevator, 255, 0x80);
		}*/
		break;
	}
#else
	switch (sound) {
	case SND_LASER_SHOOT:
		Mix_PlayChannel(-1, laser, 0);
		break;
	case SND_CANNON_SHOOT:
		Mix_PlayChannel(-1, cannon_shot, 0);
		break;
	case SND_SHORT_LASER_SHOOT:
		Mix_PlayChannel(-1, short_laser, 0);
		break;
	case SND_ROCKET_SHOOT:
		Mix_PlayChannel(-1, rocket_shot, 0);
		break;
	case SND_EXPLODE:
		Mix_PlayChannel(-1, explosions[rand() % 3], 0);
		break;
	case SND_CONTACT:
		Mix_PlayChannel(-1, contact, 0);
		break;
	case SND_BONUS:
		Mix_PlayChannel(-1, bonus, 0);
		break;
	case SND_MOVE:
		if(moveChannel == -1 || !Mix_Playing(moveChannel)) {
			moveChannel = Mix_PlayChannel(-1, move, -1);
		}
		break;
	case SND_ELEVATOR:
		if(elevatorChannel == -1 || !Mix_Playing(elevatorChannel)) {
			elevatorChannel = Mix_PlayChannel(-1, elevator, -1);
		}
		break;
	}
#endif

#endif
}

void StopSoundEffect(int sound)
{
#if defined(SOUND)

#ifdef DREAMCAST
	switch (sound) {
	case SND_MOVE:
		snd_sfx_stop_all();
		break;
	case SND_ELEVATOR:
		snd_sfx_stop_all();
		break;
	}
#else
	switch (sound) {
	case SND_MOVE:
		if(moveChannel > -1) {
			if(Mix_Playing(moveChannel)) Mix_FadeOutChannel(moveChannel, 250);
			moveChannel = -1;
		}
		break;
	case SND_ELEVATOR:
		if (elevatorChannel > -1) {
			if(Mix_Playing(elevatorChannel)) Mix_FadeOutChannel(elevatorChannel, 250);
			elevatorChannel = -1;
		}
		break;
	}
#endif
	
#endif
}

void PlayMusic(int music)
{
#if defined(SOUND)

#ifdef DREAMCAST
	switch (music) 
	{
	case MUSIC_STOP:
		//cdrom_cdda_pause();
		break;
	case MUSIC_INTRO:
		cdrom_cdda_play(1, 1, 0, CDDA_TRACKS);
		break;
	case MUSIC_GAME:
		cdrom_cdda_play(2, 2, 0, CDDA_TRACKS);
		break;
	}
#else
	if (musicChannel != -1) {
		Mix_HaltMusic();
		musicChannel = -1;
	}

	switch (music) {
	case MUSIC_STOP:
		break;
	case MUSIC_INTRO:
		musicChannel = Mix_PlayMusic(music_intro, -1);
		break;
	case MUSIC_GAME:
		musicChannel = Mix_PlayMusic(music_game, -1);
		break;
	}
#endif
	
#endif
}

int LM_SND_Init()
{
#if defined(SOUND)

#ifdef DREAMCAST
	snd_stream_init();
	spu_cdda_volume(15, 15);
	spu_cdda_pan(15, 15);
#else
	if ((Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG) != MIX_INIT_OGG) {
		printf("Failed to init OGG support\n");
	}
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
#endif
	LoadSoundFiles();
#endif
	return 1;
}

int LM_SND_Deinit()
{
#if defined(SOUND)
#ifdef DREAMCAST
	snd_sfx_stop_all();
    snd_stream_shutdown();
#else
	Mix_CloseAudio();
	Mix_Quit();
#endif
#endif
	return 1;
}
