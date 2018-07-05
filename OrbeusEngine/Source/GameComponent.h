#pragma once


class Transform;
class GameObject;
class Shader;

class GameComponent
{
private:
	GameObject* owner = nullptr;

public:
	GameComponent();
	~GameComponent();

	void setOwner(GameObject* owner) { this->owner = owner; }
	GameObject* getOwner() const { return owner; }
	Transform* getTransform() const;

	virtual void init() { }
	virtual void update(float deltaTime) { }
	virtual void render(float deltaTime, Shader* shader) { }

};