#version 420

in vec2 texCoord;
smooth in vec3 vNormal;
in vec3 pos;

out vec4 outputColor;

uniform mat4 viewToShadowMap;

uniform float ambient;
uniform sampler2D gSampler;
uniform sampler2D gShadowMap;

vec3 vDirection = vec3(1.0, 3.0, -1.0);

void main()
{
	vec4 texColor = texture2D(gSampler, texCoord);
	float fDiffuseIntensity = max(ambient, dot(normalize(vNormal), normalize(vDirection)));

	outputColor = vec4(texColor.rgb * fDiffuseIntensity, texColor.a);

	//determine if we are shadowed
	vec4 shadowCoord = viewToShadowMap * vec4(pos, 1.0);

	//Where is the point in space in the Shadow Camera?
	//What is the depth of that location?
	float shadowDepth = texture(gShadowMap, shadowCoord.xy).r;

	//Is there an occluder?
	if (shadowDepth < shadowCoord.z)
	{
		//The we're in shadow
		outputColor.rgb *= vec3(0.5, 0.5, 0.9);
	}

	if (ambient == -1.0)
	{
		outputColor.rgb = vec3(0.0, 0.0, 0.0);
	}
}