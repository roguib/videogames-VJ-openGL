#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Quad.h"


#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480


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
	void computeX(glm::mat4 &modelview, bool needsScaling);

private:
	float x = 0.0f;
	Quad *quad;
	ShaderProgram program;
	float currentTime;
	glm::mat4 projection;
	bool goingLeft = true;
	float scaleFactor = 1.0f;

};


#endif // _SCENE_INCLUDE

