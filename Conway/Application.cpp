#include "Application.h"
#include "Game.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

SDL_Window* Application::m_appWindow;
SDL_Renderer* Application::m_appRenderer;
SDL_Event Application::m_appEvent;
SDL_Texture* Application::m_menuTexture;
bool Application::m_isRunning, Game::m_isPaused, Application::m_showAbout;
std::vector <Cell*> Application::m_cellVector;

//int m_windowWidth, m_windowHeight

Application::Application(int p_windowWidth, int p_windowHeight)
{
	bool errorSpotted = false;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		errorSpotted = true;
	}
	if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
	{
		errorSpotted = true;
	}  
	if (TTF_Init() != 0)
	{
		errorSpotted = true;
	}

	Application::m_appWindow = SDL_CreateWindow("Conway's Game Of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, p_windowWidth, p_windowHeight, SDL_WINDOW_SHOWN);
	Application::checkForError(Application::m_appWindow);

	Application::m_appRenderer = SDL_CreateRenderer(Application::m_appWindow, NULL, SDL_RENDERER_ACCELERATED);
	Application::checkForError(Application::m_appRenderer);

	if (errorSpotted == false)
	{
		Game::m_isPaused = true;
		m_isRunning = true;
		Application::m_showAbout = false;
	}
	else
	{
		SDL_Log("Something went wrong during the initialization of program. Terminating the execution");
		exit(-1);
	}

	SDL_Surface* appIcon = IMG_Load("BM.png");
	SDL_SetWindowIcon(Application::m_appWindow, appIcon);
	SDL_FreeSurface(appIcon);

	m_windowWidth = p_windowWidth;
	m_windowHeight = p_windowHeight;
}

SDL_Texture* Application::loadTexture(std::string p_path)
{
	SDL_Surface* tempSurface = IMG_Load(p_path.c_str());

	SDL_Texture* resultingTexture = SDL_CreateTextureFromSurface(Application::m_appRenderer, tempSurface);
	if (resultingTexture == nullptr)
	{
		exit(1);
	}

	SDL_FreeSurface(tempSurface);

	return resultingTexture;
}

void Application::run()
{
	Game::setSizeInPixels(32);
	Game::locateAllCells();
	Game::connectCells();

	Application::runMenu();
	while (Application::m_isRunning)
	{
		display();

		if (Game::m_isPaused == true)
		{
			runMenu();
		}
		Game::gameLogic();

		handleEvents();
	}
}

void Application::runMenu()
{
	SDL_RenderClear(Application::m_appRenderer);
	for (Cell* currentCell : Application::m_cellVector)
	{
		if (currentCell->isAlive())
		{
			currentCell->setLivingState(false);
		}
		else
		{
			continue;
		}
	}

	Button::setUpButtons();
	Application::m_menuTexture = Application::loadTexture("Menu.png");

	const int frameRate = 60;
	const int delayFrame = 1000 / frameRate; // Maxixmum time between frames, assuming no lags occuring

	Uint32 frameStart;
	int timePassedSinceFrameStart;

	while (Game::m_isPaused)
	{
		frameStart = SDL_GetTicks();

		Application::handleEvents();

		SDL_RenderCopy(Application::m_appRenderer, Application::m_menuTexture, NULL, NULL);
		SDL_RenderCopy(Application::m_appRenderer, Application::m_appName.m_entityTexture, NULL, &Application::m_appName.m_destRect);
		for (auto& currentButton : Application::m_buttonVector)
		{
			currentButton->drawButton();
		}

		SDL_RenderPresent(Application::m_appRenderer);
		SDL_RenderClear(Application::m_appRenderer);

		timePassedSinceFrameStart = SDL_GetTicks() - frameStart;
		if (delayFrame > timePassedSinceFrameStart)
		{
			SDL_Delay(delayFrame - timePassedSinceFrameStart);
		}
	}

	SDL_DestroyTexture(Application::m_appName.m_entityTexture);
	Button::cleanUpButtons();
	SDL_DestroyTexture(Application::m_menuTexture);
	if (Application::m_showAbout == true)
	{
		Application::about();
	}
	else
	{
		Game::prepare();
	}
}

void Application::about()
{
	Application::m_buttonVector.emplace_back(new Button());
	SDL_Texture* backgroundTexture = Application::loadTexture("About.png");

	const int frameRate = 60;
	const int delayFrame = 1000 / frameRate;

	Uint32 frameStart;
	int timePassedSinceFrameStart;


	while (Application::m_showAbout == true)
	{
		frameStart = SDL_GetTicks();
	
		SDL_RenderClear(Application::m_appRenderer);

		Application::handleEvents();

		SDL_RenderCopy(Application::m_appRenderer, backgroundTexture, NULL, NULL);
		for (auto& currentButton : Application::m_buttonVector) // Normally there should be one button, allowing to go back
		{
			currentButton->drawButton();
		}

		SDL_RenderPresent(Application::m_appRenderer);

		timePassedSinceFrameStart = SDL_GetTicks() - frameStart;
		if (delayFrame > timePassedSinceFrameStart)
		{
			SDL_Delay(delayFrame - timePassedSinceFrameStart);
		}
	}
	Button::cleanUpButtons();
	SDL_DestroyTexture(backgroundTexture);
	runMenu();
}

void Application::display()
{
	SDL_RenderClear(Application::m_appRenderer);
	for (Cell* currentCell : Application::m_cellVector)
	{
		SDL_RenderCopy(Application::m_appRenderer, currentCell->cellEntity()->m_entityTexture, NULL, &currentCell->cellEntity()->m_destRect);
	}
	SDL_RenderPresent(Application::m_appRenderer);
}

void Application::handleEvents()
{
	int cursorX, cursorY;
	while (SDL_PollEvent(&Application::m_appEvent))
	{
		switch (m_appEvent.type)
		{
		case SDL_QUIT:
			Application::m_showAbout = false;
			Game::m_isPaused = false;
			Application::m_isRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			SDL_GetMouseState(&cursorX, &cursorY);
			if (Game::m_isPaused == true || Application::m_showAbout == true)
			{
				for (auto& button : Application::m_buttonVector)
				{
					if (!(cursorX < button->buttonBackgroundDimension().x) &&
						!(cursorX > button->buttonBackgroundDimension().x + button->buttonBackgroundDimension().w) &&
						!(cursorY < button->buttonBackgroundDimension().y) &&
						!(cursorY > button->buttonBackgroundDimension().y + button->buttonBackgroundDimension().h))
					{
						button->doAction();
					}
				}
			}
			break;
		case SDL_KEYDOWN:
			switch (Application::m_appEvent.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				Game::m_isPaused = true;
				Application::m_showAbout = false;
				break;
			default:
				break;
			}
		default:
			break;
		}
	}
}

template <typename T>
void Application::checkForError(T tp_pointer)
{
	if (tp_pointer == nullptr)
	{
		SDL_Log("SDL error. Code: %s\n", SDL_GetError());
		IMG_Quit();
		SDL_Quit();
		exit(0);
	}
}

Application::~Application()
{
	IMG_Quit();
	SDL_Quit();
}