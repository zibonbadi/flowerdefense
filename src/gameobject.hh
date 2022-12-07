#pragma once
class GameObject {
private:
	int x, y, w, h;

public:
	GameObject();
	~GameObject();
	// Int types for error codes
	virtual int init();
	virtual int tick();
	virtual int render();
	virtual bool is_collide();
};