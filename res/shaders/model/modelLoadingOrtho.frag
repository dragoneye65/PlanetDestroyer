/*
#version 440 core

in vec2 TexCoords;
out vec4 color;
uniform sampler2D texture_diffuse;

void main()
{
    color = texture( texture_diffuse, TexCoords);
}
 */

#version 330

in vec2 texCoord;
out vec4 outputColor;

uniform sampler2D gSampler;
uniform vec4 vColor;

void main()
{
	vec4 vTexColor = texture2D(gSampler, texCoord);
	outputColor = vTexColor*vColor;
}