#pragma once

class Game;
class Input;
class Shader;
class RenderingEngine;
class Window;

struct GLFWwindow;

class Engine
{
public:
	static Engine& getInstance()
	{
		static Engine instance;

		return instance;
	}
private:
	Engine() {}

public:
	Engine(Engine const&) = delete;
	void operator=(Engine const&) = delete;

private:
	Window* window;
	Game* game;
	Input* input;
	RenderingEngine* renderingEngine;

	bool isRunning;

	float deltaTime = 0.0f;
	double lastFrame = 0.0f;

	void finish();

public:
	void setGame(Game* game) { this->game = game; }

	Window* getWindow() const { return window; }
	Game* getGame() const { return game; }
	Input* getInput() const { return input; }
	RenderingEngine* getRenderingEngine() const { return renderingEngine; }

	void run();
	void init();
	void update(float deltaTime);
	void render(float deltaTime);

	void quitGame();

};