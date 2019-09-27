#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include "ShaderProgram.h"
#include "Quad.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	Quad *quads[4];
	ShaderProgram program;
	float currentTime;
	float SCALE_FACTOR = 0.5f;
	float INITIAL_COLOR_VALUE = 1.0f / 2.0f;

};


#endif // _SCENE_INCLUDE

