#version 400
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 TexCoord;
uniform mat4 model_view_projection;

void main()
{
	gl_Position = model_view_projection * vec4(position, 1);
	TexCoord = texCoord;
}
