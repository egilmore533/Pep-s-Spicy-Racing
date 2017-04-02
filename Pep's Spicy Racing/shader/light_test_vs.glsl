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

	//multiply the normals by the model matrix, removing the translation bits to ensure proper scaling
	//this is aparently costly and it should be faster to do it on the CPU side
	Normal = vec3(transpose(inverse(model)) * vec4(normal, 0));
	FragPos = vec3(model * vec4(vertCoord, 1));
}
