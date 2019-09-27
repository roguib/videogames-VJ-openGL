#version 330

in vec2 position;
uniform float scale;

void main()
{
	gl_Position = vec4(position*scale, 0.0, 1.0);
}
