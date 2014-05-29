/*!
 * @file timer.c
 * @brief 调整帧率稳定fps
 */
#include "kuro.h"
#include "script.h"

#define FRAMES_PER_SECOND 15

static Uint32 time;

void timer_start ()
{
	time = SDL_GetTicks();
}

Uint32 timer_get_ticks()
{
	return SDL_GetTicks() - time;
}

void RFPS_Start()
{
	timer_start();
}

void RFPS_End()
{
    const int fps = (config.fps == 0) ? FRAMES_PER_SECOND : config.fps;
	if(timer_get_ticks() < 1000 / fps)
	{
		Uint32 t = (1000 / fps) - timer_get_ticks();
		if (t > 0 && t < 1000)
			SDL_Delay(t);
	}
}