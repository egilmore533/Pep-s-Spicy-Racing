#version 400
layout(location = 0) in vec3 vertexPostion_modelspace;

uniform mat4 model_view_projection;

void main()
{
	gl_Position = model_view_projection * vec4(vertexPostion_modelspace, 1);
}
