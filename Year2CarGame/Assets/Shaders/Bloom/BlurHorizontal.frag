#version 420

uniform sampler2D uTex; //source image
uniform float uPixelSize; // 1.0 / window_width

in vec2 texCoord;

out vec3 outColor;

void main()
{
	//sample pixels in a horizontal row
	//the weights add up to 1

	outColor = vec3(0.0f, 0.0f, 0.0f);
	
	outColor += texture(uTex, vec2(texCoord.x - 4.0f *	uPixelSize, texCoord.y)).rgb * 0.06f;
	outColor += texture(uTex, vec2(texCoord.x - 3.0f *	uPixelSize, texCoord.y)).rgb * 0.09f;
	outColor += texture(uTex, vec2(texCoord.x - 2.0f *	uPixelSize, texCoord.y)).rgb * 0.12f;
	outColor += texture(uTex, vec2(texCoord.x -			uPixelSize, texCoord.y)).rgb * 0.15f;
	outColor += texture(uTex, vec2(texCoord.x,						texCoord.y)).rgb * 0.16f;
	outColor += texture(uTex, vec2(texCoord.x +			uPixelSize, texCoord.y)).rgb * 0.15f;
	outColor += texture(uTex, vec2(texCoord.x +	2.0f *	uPixelSize, texCoord.y)).rgb * 0.12f;
	outColor += texture(uTex, vec2(texCoord.x + 3.0f *	uPixelSize, texCoord.y)).rgb * 0.09f;
	outColor += texture(uTex, vec2(texCoord.x + 4.0f *	uPixelSize, texCoord.y)).rgb * 0.06f;
}