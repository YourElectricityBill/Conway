#ifndef UI_H
#define	UI_H

#include <string>
#include "SDL.h"
#include "SDL_ttf.h"

typedef short unsigned int suint;

enum ActionType
{
	Enter,
	About,
	Exit,
	Back,
};

class Button
{
public:
	Button();

	static void setUpButtons(), cleanUpButtons();

	void doAction();
	void drawButton();

	SDL_Rect buttonBackgroundDimension() { return m_buttonBackgroundDimension; }
	static TTF_Font* buttonFont() { return Button::m_buttonFont; }

	~Button();
private:
	static SDL_Texture* m_buttonTexture;
	static TTF_Font* m_buttonFont;

	SDL_Texture* m_buttonInscription;
	SDL_Rect m_buttonBackgroundDimension, m_inscriptionDimension;
	std::string m_buttonText;
	suint m_buttonType;
};

#endif