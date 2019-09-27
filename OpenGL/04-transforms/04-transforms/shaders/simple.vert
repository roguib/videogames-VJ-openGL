#version 330

uniform mat4 projection, modelview;
uniform float scale;

in vec2 position;

void main()
{
	// Transform position from pixel coordinates to clipping coordinates
	gl_Position = projection * modelview * vec4(position*scale, 0.0, 1.0);
}
