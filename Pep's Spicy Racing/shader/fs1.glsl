#version 400
in vec2 TexCoord;

in vec3 Normal;
in vec3 FragPos;  

out vec4 color;

uniform vec4 object_color;
uniform vec4 light_color;
uniform vec3 light_position; 

uniform sampler2D ourTexture;

void main()
{
	float ambient_strength = 0.1f;
    vec4 ambient = ambient_strength * light_color;

	//the light's direction vector is a result of the light_position subtracted by the fragment's position (which is the fragment's position in world space)
	//this value needs to be normalized to be usable
	vec3 norm = normalize(Normal);
	vec3 light_direction = normalize(light_position - FragPos);  

	// calculate the actual diffuse impact the light has on the current fragment
	//if the angle between both vectors is greater than 90 then the result will be negative so we make the minimum 0.0 by using max
	float diff = max(dot(norm, light_direction), 0.0);
	vec4 diffuse = diff * light_color;

    //color = texture(ourTexture, TexCoord);
	color = (ambient + diffuse) * object_color;
} 
