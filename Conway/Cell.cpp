
#include "Application.h"
#include "Game.h"

Cell::Cell(int &p_x, int &p_y)
{
	static int number = 0;
	m_xCellNumber = p_x;
	m_yCellNumber = p_y;
	m_aliveNeighbours = 0;
	m_cellNumber = number;

	++number;

	m_cellEntity.m_entityTexture = Application::loadTexture("EmptySpace.png");
	m_isAlive = false;

	m_cellEntity.m_destRect.w = Game::m_cellSizeInPixels;
	m_cellEntity.m_destRect.h = Game::m_cellSizeInPixels;
	m_cellEntity.m_destRect.x = Game::m_cellSizeInPixels * m_xCellNumber;
	m_cellEntity.m_destRect.y = Game::m_cellSizeInPixels * m_yCellNumber;
}

void Cell::setLivingState(bool p_boolValue)
{
	SDL_DestroyTexture(m_cellEntity.m_entityTexture);
	m_aliveNeighbours = 0;
	m_isAlive = p_boolValue;
	if (p_boolValue == true)
	{
		m_cellEntity.m_entityTexture = Application::loadTexture("CellAlive.png");
	}
	else
	{
		m_cellEntity.m_entityTexture = Application::loadTexture("EmptySpace.png");
	}
}

void Cell::increaseAliveNeighbourNo()
{
	++m_aliveNeighbours;
}

Cell::~Cell()
{
	SDL_DestroyTexture(m_cellEntity.m_entityTexture);
}