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
	for(int i=0; i<3; i++)
		if(texQuad[i] != NULL)
			delete texQuad[i];
}


void Scene::init()
{
	//mida del terra (punt on comença i punt on acaba)
	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(CAMERA_WIDTH - 1, (CAMERA_HEIGHT - 1) / 3.f)};
	glm::vec2 texCoords[2] = {glm::vec2(0.f, 0.f), glm::vec2(5.f, 1.f)};
	x = 0.0f;

	initShaders();
	/*quad = Quad::createQuad(0.f, 0.f, 128.f, 128.f, simpleProgram);
	texCoords[0] = glm::vec2(0.f, 0.f); 
	texCoords[1] = glm::vec2(0.5f, 0.5f);
	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.5f, 0.5f); 
	texCoords[1] = glm::vec2(1.f, 1.f);
	texQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.f, 0.f);*/ 
	// repliquem el texture quad 2x2
	/*texCoords[1] = glm::vec2(2.f, 2.f);
	texQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);*/
	//cel
	cel = Quad::createQuad(0.f, 0.f, CAMERA_WIDTH - 1, 2.f*(CAMERA_HEIGHT - 1) / 3.f, simpleProgram);
	//terra
	texQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	//bolet
	geom[0] = glm::vec2(0.f, 0.f);
	geom[1] = glm::vec2(50.f, 50.f);
	texCoords[0] = glm::vec2(0.f, 0.5f);
	texCoords[1] = glm::vec2(0.5f, 1.f);
	texQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	// Load textures
	texs[0].loadFromFile("images/varied.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[1].loadFromFile("images/rocks.jpg", TEXTURE_PIXEL_FORMAT_RGB);
	//una coord del mon virtual és un pixel, perque fem que la camara ortho tingui el mateix tamany que la view
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime, bool movementLeft, bool movementRight, bool movementUp, bool movementDown)
{
	currentTime += deltaTime;
	if ((movementLeft || movementRight) && movementUp && !isJumpingLeft && !isJumpingRight) {
		if (movementLeft) isJumpingLeft = true;
		else isJumpingRight = true;
		goingUp = true;
	}
	if (isJumpingLeft || isJumpingRight) {
		if (goingUp && y >= MAX_HEIGHT) {
			if (y > 250) {
				if (isJumpingLeft) x -= 1.5f;
				else x += 1.5f;
				y -= 1.5f;
			}
			else {
				if (isJumpingLeft) x -= 1.f;
				else x += 1.f;
				y -= 1.f;
			}
		}
		else if (goingUp && y < MAX_HEIGHT) {
			if (isJumpingLeft) x -= 1.f;
			else x += 1.f;
			goingUp = false;
			goingDown = true;
		}
		else if (goingDown && y < MIN_HEIGHT) {
			if (isJumpingLeft) x -= 1.5f;
			else x += 1.5f;
			y += 1.5f;
		}
		else if (goingDown && y > MIN_HEIGHT) {
			goingUp = false;
			goingDown = false;
			isJumpingLeft = false;
			isJumpingRight = false;
		}
	}
	else {
		y = (2.f*(CAMERA_HEIGHT - 1) / 3.f) - 38.f;
		if (movementLeft) x -= 3.f;
		else if (movementRight) x += 3.f;
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	//si el personatge s'ha mogut x implementar una càmara que es mogui -x
	simpleProgram.use();
	simpleProgram.setUniformMatrix4f("projection", projection);
	simpleProgram.setUniform4f("color", 0.2f, 0.2f, 0.8f, 1.0f);
	
	modelview = glm::translate(modelview, glm::vec3(0.f, 0.f, 0.f));
	simpleProgram.setUniformMatrix4f("modelview", modelview);
	cel->render();

	//terra
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, (2.f*(CAMERA_HEIGHT - 1) / 3.f), 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[2]->render(texs[1]);

	//bolet
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[1]->render(texs[0]);

	//en videojocs no portem el objecte al zero zero, el transofrmem i el tornem a portar. en els objectes el zero zero esta al centre baix de l'eix y
	/*modelview = glm::translate(glm::mat4(1.0f), glm::vec3(128.f, 48.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	modelview = glm::rotate(modelview, -currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	simpleProgram.setUniformMatrix4f("modelview", modelview);
	quad->render();

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(384.f, 48.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	modelview = glm::rotate(modelview, currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[0]->render(texs[0]);

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(128.f, 304.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	modelview = glm::rotate(modelview, currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[1]->render(texs[0]);

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(384.f, 304.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	modelview = glm::rotate(modelview, -currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[2]->render(texs[1]);*/

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
	simpleProgram.init();
	simpleProgram.addShader(vShader);
	simpleProgram.addShader(fShader);
	simpleProgram.link();
	if(!simpleProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleProgram.log() << endl << endl;
	}
	simpleProgram.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();
	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
}

