#include "SDL.h"
#include "Application.h"

//****************************************************
// NAMING CONVENTION
//****************************************************
// p_ for parameters (function arguments)
// tp_ for template parameters
// m_ for class/struct member variables
//****************************************************
//****************************************************
// This is one of my projects developed outside of uni time 
// Created by Deine Stromrechnung (Yourelectricitybill) 2019
// ALL RIGHTS RESERVED
// YOU CAN USE THE CODE AS LONG AS IT'S FOR NON-COMMERCIAL PURPOSE AND YOU CREDIT THE CREATOR
//****************************************************

int main(int argc, char* argv[])
{
	Application* myApp = new Application(1216, 800);

	myApp->run();

	delete myApp;

	return 0;
}