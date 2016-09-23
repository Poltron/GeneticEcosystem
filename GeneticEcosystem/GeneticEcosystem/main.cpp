// main.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "Engine.h"
#include "Settings.h"

#include <iostream>

int main(int argc, char* argv[])
{
	bool initialized = Engine::Instance().initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!initialized)
	{
		std::cout << "Engine initialization failed." << std::endl;
		system("pause");
		return EXIT_FAILURE;
	}

	Engine::Instance().start();
	Engine::Instance().close();

	return EXIT_SUCCESS;
}