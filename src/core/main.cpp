#include "Application.hpp"


int main(int argc, char* argv[]) 
{
	constexpr Fseconds fps{ std::chrono::milliseconds(16) };
	constexpr Fseconds maxDelay{ std::chrono::milliseconds(100) };
	constexpr Fseconds slowWarring{ std::chrono::milliseconds(33) };

	Application app;
	if (app.initialize("Opengl test", 640, 480))
	{
		app.run(fps, maxDelay, slowWarring);
	}
	return 0;
}