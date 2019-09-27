#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include "Text.h"


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
	void update(int deltaTime, bool movementLeft, bool movementRight, bool movementUp, bool movementDown);
	void render();

private:
	void initShaders();

private:
	float x;
	float y;
	bool isJumpingLeft = false, isJumpingRight = false;
	bool goingUp = false, goingDown = false;
	float MAX_HEIGHT = 240.f;
	float MIN_HEIGHT = 282.f;
	int bounce = 0;
	bool hasBounced = false;
	Quad *quad;
	Quad *cel;
	TexturedQuad *terra;
	Texture texs[2];
	TexturedQuad *texQuad[3];
	ShaderProgram simpleProgram, texProgram;
	float currentTime;
	glm::mat4 projection;
	Text text;

};


#endif // _SCENE_INCLUDE

