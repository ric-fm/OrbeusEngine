#pragma once

#include <vector>
#include "Editor.h"


class EditorManager
{
public:
	static EditorManager& getInstance()
	{
		static EditorManager instance;

		return instance;
	}
private:
	EditorManager();

public:
	EditorManager(EditorManager const&) = delete;
	void operator=(EditorManager const&) = delete;

private:
	std::vector<Editor*> editors;
public:
	void addEditor(Editor* editor);
	//void removeEditor(Editor* editor);
	Editor* getEditor(unsigned int idx);
	std::vector<Editor*>& getEditors() { return editors; }

	void init();
	void destroy();
	void update(float deltaTime);
	void render();
};