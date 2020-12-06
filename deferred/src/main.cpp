#include "Log.h"
#include "DeferredShade.h"

void main()
{
	Log::Intialize(spdlog::level::trace);
	CORE_INFO("Hello!");
	Application app;
	app.Initialize();
	app.DrawFrame();
	CORE_INFO("Quit!");
}