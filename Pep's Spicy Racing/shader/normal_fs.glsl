#version 400
in vec2 TexCoord;				/**< UV of the vertex to color */

in vec3 Normal;					/**< the normal of the vertex to color, calculated by the mesh's data and the model matrix */
in vec3 FragPos;				/**< position of the vertex to color in world space */

out vec4 color;					/**< the output color of this vertex */

uniform vec4 object_color;		/**< color of the object itself */
uniform vec4 light_color;		/**< color of the light itself */
uniform vec3 light_position;	/**< position of the actual light in world space */
uniform vec3 view_position;		/**< position of the eye's or camera in world space */

uniform sampler2D ourTexture;	/**< the texture being used to color this mesh */

void main()
{
	//AMBIENT CALCULATION
	float ambient_strength = 0.2f;
    vec4 ambient = ambient_strength * light_color;

	//DIFFUSE CALCULATION
	//the light's direction vector is a result of the light_position subtracted by the fragment's position (which is the fragment's position in world space)
	//this value needs to be normalized to be usable
	vec3 norm = normalize(Normal);
	vec3 light_direction = normalize(light_position - FragPos);  

	// calculate the actual diffuse impact the light has on the current fragment
	//if the angle between both vectors is greater than 90 then the result will be negative so make the minimum 0.0 by using max
	float diff = max(dot(norm, light_direction), 0.0);
	vec4 diffuse = diff * light_color;

	//SPECULAR CALCULATION
	float specular_strength = 0.1f;								/**< this is the impact of the highlight */
	int shininess_strength = 32;								/**< the higher the shininess the more it properly reflects the light instead of scattering it around (needs to be powers of 2) */
	vec3 view_direction = normalize(view_position - FragPos);	

	//reflect expects the first var to point from the light source towards the fragment's position so negate view_direction
	vec3 reflect_direction = reflect(-view_direction, norm); 
	
	//first get the dot product of view_direction and reflect direction, then raise to the power of our shininess variable
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess_strength);
	vec4 specular = specular_strength * spec * light_color; 

    //color = (ambient + diffuse + specular) * texture(ourTexture, TexCoord);

	//add our ambient, diffuse, and specular values to make whats called a Phong lightning model
	//color = floor(((ambient + diffuse + specular) * object_color) * 4) / 4;
	color = vec4(norm, 1.0);
} 
