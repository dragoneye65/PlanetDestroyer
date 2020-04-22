#version 330 core

in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

uniform bool wireframe_enable;
uniform vec3 wireframeColor;

void main()
{
    if ( wireframe_enable)
        FragColor = vec4(wireframeColor,1.f);
    else {
	    FragColor = texture( texture_diffuse1, TexCoords);
    }
}


