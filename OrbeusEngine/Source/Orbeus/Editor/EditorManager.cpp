#include "EditorManager.h"


EditorManager::EditorManager()
{

}

void EditorManager::addEditor(Editor* editor)
{
	editors.push_back(editor);
}

//void EditorManager::removeEditor(Editor* editor)
//{
//	
//}

Editor * EditorManager::getEditor(unsigned int idx)
{
	if (idx >= 0 && idx < editors.size())
		return editors[idx];
	return nullptr;
}

void EditorManager::init()
{
	for (Editor* editor : editors)
	{
		editor->init();
	}
}

void EditorManager::destroy()
{
	for (Editor* editor : editors)
	{
		editor->destroy();
	}
}

void EditorManager::update(float deltaTime)
{
	for (Editor* editor : editors)
	{
		editor->update(deltaTime);
	}
}

void EditorManager::render()
{
	for (Editor* editor : editors)
	{
		editor->render();
	}
}
