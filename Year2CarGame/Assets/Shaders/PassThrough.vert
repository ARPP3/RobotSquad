#version 420

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;

uniform mat4 modelMatrix = mat4(1.0);

out vec2 texCoord;

void main()
{
	texCoord = inUV;
	gl_Position = modelMatrix * vec4(inPosition, 1.0f);
}