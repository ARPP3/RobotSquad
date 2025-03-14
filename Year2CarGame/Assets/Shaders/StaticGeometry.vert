#version 420

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

out vec2 texCoord;

smooth out vec3 vNormal;
out vec3 pos;

void main()
{
	gl_Position = projectionMatrix*viewMatrix*modelMatrix*vec4(inPosition, 1.0);
	pos = (viewMatrix*modelMatrix*vec4(inPosition, 1.0)).xyz;
	texCoord = inCoord;
	vec4 vRes = normalMatrix*vec4(inNormal, 0.0);
	vNormal = vRes.xyz;
}