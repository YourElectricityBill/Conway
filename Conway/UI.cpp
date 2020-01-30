#include "UI.h"
#include "Application.h"
#include "Game.h"

SDL_Texture* Button::m_buttonTexture;
TTF_Font* Button::m_buttonFont;
Entity Application::m_appName;
std::vector <Button*> Application::m_buttonVector;

Button::Button()
{
	static int number = 0;
	if (number == 0)
	{
		m_buttonType = Enter;
		m_buttonText = "Enter";
	}
	else if (number == 1)
	{
		m_buttonType = About;
		m_buttonText = "About";
	}
	else if (number == 2)
	{
		m_buttonType = Exit;
		m_buttonText = "Exit";
	}
	
	if(Application::m_showAbout == true)
	{
		m_buttonType = Back;
		m_buttonText = "";
		Button::m_buttonTexture = Application::loadTexture("Arrow.png");
	}

	SDL_Surface* fontSurface = TTF_RenderText_Blended(Button::m_buttonFont, m_buttonText.c_str(), { 255, 255, 255, 255 });
	m_buttonInscription = SDL_CreateTextureFromSurface(Application::m_appRenderer, fontSurface);
	SDL_FreeSurface(fontSurface);

	int inscriptionWidth = 0;
	int inscriptionHeight = 0;

	SDL_QueryTexture(m_buttonInscription, NULL, NULL, &inscriptionWidth, &inscriptionHeight);

	if (Application::m_showAbout == true)
	{ 
		m_inscriptionDimension.w = 0; // All zeroes since button in "About" section should have no inscription at all
		m_inscriptionDimension.h = 0;
		m_inscriptionDimension.x = 0;
		m_inscriptionDimension.y = 0;

		m_buttonBackgroundDimension.w = Application::m_windowWidth * 0.15f;
		m_buttonBackgroundDimension.h = Application::m_windowHeight * 0.3f;
		m_buttonBackgroundDimension.x = 0;
		m_buttonBackgroundDimension.y = Application::m_windowHeight - (m_buttonBackgroundDimension.h * 0.75f);
	}
	else
	{
		m_inscriptionDimension.w = inscriptionWidth;
		m_inscriptionDimension.h = inscriptionHeight;
		m_inscriptionDimension.x = ((Application::m_windowWidth / 2) - (m_inscriptionDimension.w / 2));
		m_inscriptionDimension.y = ((Application::m_windowHeight * 0.35f) + (140 * number));

		m_buttonBackgroundDimension.w = Application::m_windowWidth * 0.175f;
		m_buttonBackgroundDimension.h = Application::m_windowHeight * 0.075f;
		m_buttonBackgroundDimension.x = ((Application::m_windowWidth / 2) - (m_buttonBackgroundDimension.w / 2));
		m_buttonBackgroundDimension.y = m_inscriptionDimension.y - m_inscriptionDimension.h * 0.1f;
	}

	++number;
	if (number == 3)
	{
		number = 0;
	}
}

void Button::setUpButtons()
{
	Button::m_buttonTexture = Application::loadTexture("Button.png");
	Button::m_buttonFont = TTF_OpenFont("arial.ttf", 34);

	for (int number = 0; number < Application::m_numberOfButtons; ++number)
	{
		Application::m_buttonVector.emplace_back(new Button());
	}

	TTF_CloseFont(Button::m_buttonFont);

	Button::m_buttonFont = TTF_OpenFont("arial.ttf", 64); // I open it twice because I change the size of the font

	SDL_Surface* appName = TTF_RenderText_Blended(Button::buttonFont(), "Conway's Game of Life", { 0, 0, 0 });
	Application::m_appName.m_entityTexture = SDL_CreateTextureFromSurface(Application::m_appRenderer, appName);
	SDL_FreeSurface(appName);

	int inscriptionWidth, inscriptionHeight;

	SDL_QueryTexture(Application::m_appName.m_entityTexture, NULL, NULL, &inscriptionWidth, &inscriptionHeight);

	Application::m_appName.m_destRect.w = inscriptionWidth;
	Application::m_appName.m_destRect.h = inscriptionHeight;
	Application::m_appName.m_destRect.x = (Application::m_windowWidth / 2) - (Application::m_appName.m_destRect.w / 2);
	Application::m_appName.m_destRect.y = Application::m_windowHeight * 0.1f;

	TTF_CloseFont(Button::m_buttonFont);

}

void Button::cleanUpButtons()
{
	for (auto& currentButton : Application::m_buttonVector)
	{
		delete currentButton;
		currentButton = nullptr;
	}
	Application::m_buttonVector.clear();
	Application::m_buttonVector.shrink_to_fit();
	SDL_DestroyTexture(Button::m_buttonTexture);
}

void Button::doAction()
{
	if (m_buttonType == Enter)
	{
		Game::m_isPaused = false;
	}
	else if (m_buttonType == About)
	{
		Game::m_isPaused = false;
		Application::m_showAbout = true;
	}
	else if (m_buttonType == Exit)
	{
		Game::m_isPaused = false;
		Application::m_isRunning = false;
	}
	else if (m_buttonType == Back)
	{
		Application::m_showAbout = false;
		Game::m_isPaused = true;
	}
	else
	{
		SDL_Log("UNKNOWN BUTTON TYPE");
		exit(1);
	}
}

void Button::drawButton()
{
	for (auto& currentButton : Application::m_buttonVector)
	{
		SDL_RenderCopy(Application::m_appRenderer, Button::m_buttonTexture, NULL, &currentButton->m_buttonBackgroundDimension);
		SDL_RenderCopy(Application::m_appRenderer, currentButton->m_buttonInscription, NULL, &currentButton->m_inscriptionDimension);
	}
}

Button::~Button()
{
	SDL_DestroyTexture(m_buttonInscription);
}