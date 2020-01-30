#include "Application.h"
#include "Game.h"

int Game::m_cellSizeInPixels, Application::m_windowHeight, Application::m_windowWidth;

void Game::setSizeInPixels(int p_size)
{
	if (Application::m_windowHeight % p_size != 0 || Application::m_windowWidth % p_size != 0)
	{
		SDL_Log("Erorr: Invalid window size. Height or width do not match cell size in pixels. Try using different window resolution or pixel size.");
		exit(2);
	}
	else
	{
		Game::m_cellSizeInPixels = p_size;
	}
}

void Game::locateAllCells()
{
	for (int y = 0; y < Application::m_windowHeight / Game::m_cellSizeInPixels; ++y)
	{
		for (int x = 0; x < Application::m_windowWidth / Game::m_cellSizeInPixels; ++x)
		{
			Application::m_cellVector.emplace_back(new Cell(x, y));
		}
	}
}

void Game::connectCells()
{
	int widthInCells(0), heightInCells(0);
	for (Cell *currentCell : Application::m_cellVector)
	{
		if (currentCell->yCellNumber() == 0)
		{
			widthInCells += 1;
		}
		heightInCells = currentCell->yCellNumber();
	}

	for (Cell *currentCell : Application::m_cellVector)
	{
		if (currentCell->yCellNumber() > 0) // Top 
		{
			currentCell->neighbours().push_back(Application::m_cellVector.at(currentCell->currentCellNumber() - widthInCells));
		}
		if (currentCell->yCellNumber() < heightInCells - 1)  // Bottom
		{
			currentCell->neighbours().push_back(Application::m_cellVector.at(currentCell->currentCellNumber() + widthInCells));
		}
		if (currentCell->xCellNumber() > 0) // Left
		{
			currentCell->neighbours().push_back(Application::m_cellVector.at(currentCell->currentCellNumber() - 1));
		}
		if (currentCell->xCellNumber() < widthInCells - 1)  // Right
		{
			currentCell->neighbours().push_back(Application::m_cellVector.at(currentCell->currentCellNumber() + 1));
		}
		if (currentCell->xCellNumber() > 0 && currentCell->yCellNumber() > 0) // Diagonal top left
		{
			currentCell->neighbours().push_back(Application::m_cellVector.at((currentCell->currentCellNumber() - widthInCells) - 1));
		}
		if (currentCell->xCellNumber() < widthInCells - 1 && currentCell->yCellNumber() > 0) // Diagonal top right
		{
			currentCell->neighbours().push_back(Application::m_cellVector.at((currentCell->currentCellNumber() - widthInCells) + 1));
		}
		if (currentCell->yCellNumber() < heightInCells - 1 && currentCell->xCellNumber() > 0) // Diagonal bottom left
		{
			currentCell->neighbours().push_back(Application::m_cellVector.at((currentCell->currentCellNumber() + widthInCells) - 1));
		}
		if (currentCell->yCellNumber() < heightInCells - 1 && currentCell->xCellNumber() < widthInCells - 1) // Diagonal bottom right
		{
			currentCell->neighbours().push_back(Application::m_cellVector.at((currentCell->currentCellNumber() + widthInCells) + 1));
		}
	}
}

void Game::prepare()
{
	bool playerIsReady = false; 
	int cursorX, cursorY;
	while (playerIsReady == false && Application::m_isRunning == true)
	{
		while (SDL_PollEvent(&Application::m_appEvent))
		{
			switch (Application::m_appEvent.type)
			{
			case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState(&cursorX, &cursorY);
				for (Cell *currentCell : Application::m_cellVector)
				{
					if (!(cursorX < currentCell->cellEntity()->m_destRect.x) &&
						!(cursorX > currentCell->cellEntity()->m_destRect.x + currentCell->cellEntity()->m_destRect.w) &&
						!(cursorY < currentCell->cellEntity()->m_destRect.y) &&
						!(cursorY > currentCell->cellEntity()->m_destRect.y + currentCell->cellEntity()->m_destRect.h))
					{
						currentCell->setLivingState(true);
					}
				}
				break;
			case SDL_KEYDOWN:
				switch (Application::m_appEvent.key.keysym.sym)
				{
				case SDLK_SPACE:
					playerIsReady = true;
					break;
				default:
					break;
				}
				break;
			case SDL_QUIT:
				playerIsReady = true;
				Application::m_isRunning = false;
				break;
			default:
				break;
			}
		}
		Application::display();
	}
}

void Game::gameLogic()
{
	for (Cell *currentCell : Application::m_cellVector)
	{
		for (Cell *neighbouringCell : currentCell->neighbours())
		{
			if (neighbouringCell->isAlive())
			{
				currentCell->increaseAliveNeighbourNo();
			}
		}
	}

	for (Cell *currentCell : Application::m_cellVector)
	{
		if (currentCell->isAlive() == true && (currentCell->aliveNeighbours() == 2 || currentCell->aliveNeighbours() == 3))
		{
			currentCell->setLivingState(true);
		}
		else if (currentCell->isAlive() == false && currentCell->aliveNeighbours() == 3)
		{
			currentCell->setLivingState(true);
		}
		else
		{
			currentCell->setLivingState(false);
		}
	}
}