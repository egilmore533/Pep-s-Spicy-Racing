#version 400
in vec2 TexCoord;

out vec4 color;

uniform vec4 object_color;
uniform vec4 light_color;

uniform sampler2D ourTexture;

void main()
{
    color = texture(ourTexture, TexCoord);
	color = object_color;
} 
