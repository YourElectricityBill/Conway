#ifndef APP_H
#define APP_H

#include <string>
#include <vector>
#include "SDL.h"
#include "Cell.h"
#include "UI.h"

struct Application
{
	Application(int p_windowWidth, int p_windowHeight);
	
	static SDL_Texture* loadTexture(std::string p_path);
	static void run(), runMenu(), about(), display(), handleEvents();

	template <typename T>
	static void checkForError(T tp_pointer);

	static SDL_Window* m_appWindow;
	static SDL_Renderer* m_appRenderer;
	static SDL_Event m_appEvent;
	static SDL_Texture* m_menuTexture;
	static bool m_isRunning, m_showAbout;
	static int m_windowWidth, m_windowHeight;
	static const int m_numberOfButtons = 3;
	static Entity m_appName;

	static std::vector <Entity*> m_entityVector; 
	static std::vector <Cell*> m_cellVector;
	static std::vector <Button*> m_buttonVector;

	~Application();
};

#endif  