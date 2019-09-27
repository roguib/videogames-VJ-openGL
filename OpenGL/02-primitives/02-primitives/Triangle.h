#ifndef _TRIANGLE_INCLUDE
#define _TRIANGLE_INCLUDE

#include "ShaderProgram.h"

class Triangle
{
public:
	static Triangle *createTriangle(float x, float y, float z, ShaderProgram &program);
	
	Triangle(float x, float y, float z, ShaderProgram &program);
	
	void render() const;
	void free();

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation;
};
#endif