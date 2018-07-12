#include "Core/Engine.h"
#include "TestGame.h"


int main()
{
	Engine::getInstance().setGame(new TestGame());
	Engine::getInstance().run();

	return 0;
}
