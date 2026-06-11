#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0)); // position in world coordinates
	TexCoords = aTexCoords;
	Normal = mat3(transpose(inverse(model))) * aNormal; // note: inverses are very costly, in best practice calculate the normal matrix on the CPU first, then send using a shader uniform
}