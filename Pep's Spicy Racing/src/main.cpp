#include <GL/glew.h>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

#include <simple_logger.h>

#include "graphics.h"
#include "camera.h"
#include "texture.h"
#include "mesh.h"

GLfloat test_vertices[] = {
	// Positions         // Colors
	0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Bottom Right
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Bottom Left
	0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // Top 
};

GLuint test_indices[] = {  // Note that we start from 0!
	0, 1, 3,   // First Triangle
	1, 2, 3    // Second Triangle
};

GLfloat texCoords[] = {
	0.0f, 0.0f,  // Lower-left corner  
	1.0f, 0.0f,  // Lower-right corner
	0.5f, 1.0f   // Top-center corner
};


void draw(GLuint shader, GLuint veterxArrayObject, GLuint colorArrayObject, Mesh *mesh)
{
	glUseProgram(shader);
	glBindVertexArray(veterxArrayObject);
	//glBindVertexArray(colorArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	glBindVertexArray(0);
}

int main()
{
	int running = 1;
	
	init_logger("game_log.log");

	Graphics *graphics = new Graphics;

	Texture *myTexture = new Texture("images/joe.png", true, true);

	Mesh *mesh = new Mesh("models/monkey.obj");

	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 model = glm::mat4(1.0f);
	Camera *camera = new Camera(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), cameraPosition);

	GLuint mvp_location;
	glm::mat4 model_view_projection; // = projectionMatrix * view * model;

	mvp_location = glGetUniformLocation(graphics->Graphics::get_shader_program(), "model_view_projection");
	//GLuint textureID = glGetUniformLocation(graphics->Graphics::get_shader_program(), "ourTexture");

	while(running)
	{
		sf::Event event;
		while (graphics->Graphics::get_game_window()->pollEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				graphics->Graphics::get_game_window()->close();
				running = 0;
			}
		}
		camera->Camera::get_mouse_input();
		camera->Camera::get_keyboard_input();
		camera->Camera::update_view_matrix();

		graphics->Graphics::frame_begin();

		/*Drawing Code Start*/

		model_view_projection = camera->Camera::get_projection_matrix() *
			camera->Camera::get_view_matrix() * 
			model;
		
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &model_view_projection[0][0]);
		

		mesh->draw(graphics->Graphics::get_shader_program());

		/*Drawing Code End*/

		graphics->Graphics::next_frame();
	}

	return 0;
}
