#include "core\engine.h"
#include "Scenes/scene01.h"
#include "Scenes/scene02.h"
#include "Scenes/scene03.h"
#include "Scenes/scene04.h"
#include "Scenes/scene05.h"
#include "Scenes/scene06.h"
#include "Scenes/scene07.h"
// "Scenes/scene08.h"
//#include "Scenes/scene09.h"
#include "Scenes/scene10.h"
#include "Scenes/scene11.h"
#include "Scenes/scene12.h"
#include "Scenes/scene16.h"

int main(int argc, char** argv)
{
	int result = SDL_Init(SDL_INIT_VIDEO);
  
	if (result != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
	}

	Engine engine;
	engine.Initialize();
	engine.Get<FileSystem>()->SetPathname("resources\\");

	Scene* scene = new Scene11(&engine); // change scene number to view different scenes
	scene->Initialize();

	while (!engine.ForceQuit())
	{
		engine.Update();
		scene->Update();

		scene->Render();
	}
	scene->Shutdown();
	engine.Shutdown();

	return 0;
}