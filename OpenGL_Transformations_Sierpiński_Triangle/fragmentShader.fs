#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in vec4 vertexPosition;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixScale;

void main()
{
	//FragColor = vec4(vertexPosition);
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixScale);
	//FragColor = vec4(TexCoord, 0.0, 1.0);
};
