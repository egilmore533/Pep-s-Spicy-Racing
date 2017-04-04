#include "button.h"

Button::Button(std::string button_def_file)
{
	json def = load_from_def(button_def_file);
	json button_def = get_element_data(def, "Button");

	//we cannot count on this def file to contain the proper data
	if (button_def == NULL)
	{
		return;
	}

	std::string sprite_def_filepath = button_def["sprite_def_filepath"];
	background = Sprite_Manager::create_sprite(sprite_def_filepath);
	std::string temp_string = button_def["text"];

	clicked = false;
}

void Button::draw(Camera *camera)
{
	Sprite_Manager::draw(camera, background->id);
}

void Button::update()
{
	glm::vec2 mouse_position = glm::vec2(sf::Mouse::getPosition(*Graphics::get_game_window()).x, sf::Mouse::getPosition(*Graphics::get_game_window()).y);

	if (mouse_position.x > background->screen_position.x && mouse_position.x < background->screen_position.x + background->size.x
		&& mouse_position.y > background->screen_position.y && mouse_position.y < background->screen_position.y + background->size.y)
	{
		background->color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !clicked)
		{
			this->callback();
			clicked = true;
		}
	}
	else
	{
		background->color = glm::vec4(1);
		clicked = false;
	}

	
}
