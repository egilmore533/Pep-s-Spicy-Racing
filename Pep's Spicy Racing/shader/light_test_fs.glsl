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
	
	//add our ambient, diffuse, and specular values to make whats called a Phong lightning model
	color = (ambient + diffuse) * object_color;
}
