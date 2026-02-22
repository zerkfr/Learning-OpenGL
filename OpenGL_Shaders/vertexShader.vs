#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float xPos;
uniform float yPos;
uniform float alphaValue;
uniform float scale;

// out vec3 ourColor;
out vec4 vertexPosition;

void main()
{
	gl_Position = vec4(aPos.x + xPos, aPos.y + yPos, aPos.z, scale);

	// ourColor = aColor;
	vertexPosition = vec4(gl_Position.x, gl_Position.y, 0.5f, alphaValue);
}