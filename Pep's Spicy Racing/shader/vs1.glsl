#version 400
layout(location = 0) in vec3 vertexPostion_modelspace;
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

void main()
{
	gl_Position = vec4(vertexPostion_modelspace, 1);
	fragmentColor = vertexColor;
}
