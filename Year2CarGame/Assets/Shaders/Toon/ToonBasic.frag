#version 420

in vec2 texCoord;
smooth in vec3 vNormal;
out vec4 outputColor;

uniform float ambient;
uniform sampler2D gSampler;
uniform sampler2D toonSampler;

vec3 vDirection = vec3(1.0, 3.0, -1.0);

void main()
{
	vec4 texColor = texture2D(gSampler, texCoord);

	float fDiffuseIntensity = max(ambient, dot(normalize(vNormal), normalize(vDirection)));
	
	vec4 texIntensity = texture2D(toonSampler, normalize(vec2(fDiffuseIntensity, 0.5)));

	outputColor = vec4(texColor.rgb * fDiffuseIntensity, 1.0);
	//outputColor = vec4(texColor.rgb * texIntensity.rgb, 1.0);
}