#include <GL/glew.h>
#include <GL/gl.h>
#include "Triangle.h"


Triangle *Triangle::createTriangle(float x, float y, float z, ShaderProgram &program)
{
	Triangle *triangle = new Triangle(x, y, z, program);

	return triangle;
}

Triangle::Triangle(float x, float y, float z, ShaderProgram &program)
{
	float vertices[6] = { -0.2, -0.2, 0.0, 0.35, 0.2, -0.2 };


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2);
}

void Triangle::render() const
{
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Triangle::free()
{
	glDeleteBuffers(1, &vbo);
}

