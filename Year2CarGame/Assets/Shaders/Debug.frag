#version 420

in vec2 texCoord;
smooth in vec3 vNormal;
out vec4 outputColor;
in vec3 pos;

void main()
{
	outputColor = vec4(vNormal.rgb, 1.0);
}