/*!
 * @file timer.c
 * @brief 调整帧率稳定fps
 */
#include "kuro.h"

#define FRAMES_PER_SECOND 60

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
	if(timer_get_ticks() < 1000 / FRAMES_PER_SECOND)
	{
		Uint32 t = (1000 / FRAMES_PER_SECOND) - timer_get_ticks();
		if (t > 0 && t < 1000)
			SDL_Delay(t);
	}
}