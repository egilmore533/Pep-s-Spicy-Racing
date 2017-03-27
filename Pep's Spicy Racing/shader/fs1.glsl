#version 400

in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 sprite_color;

void main()
{
	color = sprite_color * texture(image, TexCoords);
}
