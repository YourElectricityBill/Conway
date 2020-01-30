#ifndef ENTITY_H
#define	ENTITY_H

#include "SDL.h"

struct Entity
{
	SDL_Texture* m_entityTexture;
	SDL_Rect m_destRect;
};

#endif