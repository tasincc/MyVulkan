#include "Log.h"
#include "sceneLoad.h"

void main()
{
	Log::Intialize(spdlog::level::trace);
	CORE_INFO("Hello!");
	Application app;
	app.Initialize();
	app.DrawFrame();
	CORE_INFO("Quit!");
}