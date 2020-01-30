#ifndef GAME_H
#define	GAME_H

#include <vector>
#include "Entity.h"

struct Game // Class for manipulating game's logic
{
	Game() = default;

	static void setSizeInPixels(int p_size);
	static void locateAllCells();
	static void connectCells();
	static void prepare();
	static void gameLogic();

	static int m_cellSizeInPixels;
	static bool m_isPaused;
};

#endif