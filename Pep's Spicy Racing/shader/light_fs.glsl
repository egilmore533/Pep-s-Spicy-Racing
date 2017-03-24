#version 400
out vec4 color;

uniform vec4 object_color;		/**< color of the light itself */

void main()
{
    color = object_color; // Set all 4 vector values to 1.0f
}
