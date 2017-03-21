#version 400
layout(location = 0) in vec3 vertCoord;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 TexCoord;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(vertCoord, 1);
	TexCoord = texCoord;
	Normal = normal;
	FragPos = vec3(model * vec4(vertCoord, 1));
}
