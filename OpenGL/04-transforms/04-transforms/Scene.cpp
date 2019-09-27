#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"


Scene::Scene()
{
	quad = NULL;
}

Scene::~Scene()
{
	if(quad != NULL)
		delete quad;
}


void Scene::init()
{
	initShaders();
	quad = Quad::createQuad(0.f, 0.f, 128.f, 128.f, program);
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
}

void Scene::computeX(glm::mat4 &modelview, bool needsScaling) {
	program.use();
	if (goingLeft) x += 2.f;
	else x -= 2.f;
	modelview = glm::translate(modelview, glm::vec3(x, 0.f, 0.f));
	if (needsScaling) {
		if (goingLeft) scaleFactor -= 0.01f;
		else scaleFactor += 0.01f;
		program.setUniform1f("scale", scaleFactor);
	}
	else {
		program.setUniform1f("scale", 1.0f);
	}
	
}

void Scene::render()
{
	glm::mat4 modelview;

	// We can now, using matrices, draw four quads at different screen locations
	// using a single Quad object.
	program.use();
	program.setUniformMatrix4f("projection", projection);
	program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(128.f, 48.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	freopen("output.txt", "a+", stdout);
	cout << "Current Time: " << currentTime << " Current position: " << x << " Current viewport: " << m_viewport[2] << endl;
	if (x + 128.f + 64.f > m_viewport[2] / 2 || x == -128.f) goingLeft = !goingLeft;
	computeX(modelview, false);
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	program.setUniformMatrix4f("modelview", modelview);
	quad->render();

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(384.f, 48.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	computeX(modelview, true);
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	program.setUniformMatrix4f("modelview", modelview);
	quad->render();

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(128.f, 304.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	computeX(modelview, false);
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	program.setUniformMatrix4f("modelview", modelview);
	quad->render();

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(384.f, 304.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	computeX(modelview, true);
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	program.setUniformMatrix4f("modelview", modelview);
	quad->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	program.init();
	program.addShader(vShader);
	program.addShader(fShader);
	program.link();
	if(!program.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << program.log() << endl << endl;
	}
	program.bindFragmentOutput("outColor");
}

