#include <iostream>
#include "Scene.h"



Scene::Scene()
{
	for(int i = 0; i < 4; i++)
		quads[i] = NULL;
	triangle = NULL;
}

Scene::~Scene()
{
	for(int i = 0; i < 4; i++)
		if(quads[i] != NULL)
			delete quads[i];
	if (triangle != NULL) delete triangle;
}


void Scene::init()
{
	//OutputDebugStringA("My output string."); //include windows.h
	initShaders();
	for(int i = 0; i < 4; i++)
		quads[i] = Quad::createQuad(-0.75f + (i % 2), -0.75f + (i / 2), 0.5f, 0.5f, program);
	triangle = Triangle::createTriangle(1.0f, 1.0f, 0.0f, program);
}

void Scene::update(int deltaTime)
{
}

void Scene::render()
{
	program.use();
	for(int i = 0; i < 4; i++)
		quads[i]->render();
	triangle->render();
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

