#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vertexPosition;
out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0f);
	TexCoord = aTexCoord;
	ourColor = aColor;

	// vertexPosition = vec4(gl_Position.x, gl_Position.y, 0.5f, 1.0f);
}
