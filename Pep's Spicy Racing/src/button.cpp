#include "button.h"

sf::Font button_font1;
bool font_loaded = false;

void load_my_font()
{
	if (!font_loaded)
	{
		if (!button_font1.loadFromFile("fonts/Spicy.ttf"))
		{

		}
		font_loaded = true;
	}
}

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
	clicked = false;

	glm::vec2 pos = glm::vec2(button_def["position"][0], button_def["position"][1]);
	glm::vec2 size = glm::vec2(button_def["size"][0], button_def["size"][1]);
	float rot = button_def["rotation"];
	background->set_data(pos, size, rot, background->color);

	json text_def = get_element_data(button_def, "Text");
	
	if (text_def == NULL)
		return;

	load_my_font();
	text.setFont(button_font1);

	std::string temp_string = text_def["text"];
	rot = text_def["rotation"];
	text.setString(temp_string);
	text.setRotation(rot);

	int text_size = text_def["text_size"];
	text.setCharacterSize(text_size);

	glm::vec4 out_col = glm::vec4(text_def["text_outline_color"][0], text_def["text_outline_color"][1], text_def["text_outline_color"][2], text_def["text_outline_color"][3]);
	sf::Color out_color = sf::Color(out_col.r * 255.0f, out_col.g * 255.0f, out_col.b * 255.0f, out_col.a * 255.0f);
	glm::vec4 fill_col = glm::vec4(text_def["text_fill_color"][0], text_def["text_fill_color"][1], text_def["text_fill_color"][2], text_def["text_fill_color"][3]);
	sf::Color fill_color = sf::Color(fill_col.r * 255.0f, fill_col.g * 255.0f, fill_col.b * 255.0f, fill_col.a * 255.0f);

	text.setFillColor(fill_color);
	text.setOutlineColor(out_color);
	text.setPosition((pos.x + background->size.x / 2.0f) - text.getLocalBounds().width / 2.0f, (pos.y + background->size.y / 2.0f) - text.getLocalBounds().height);
}

void Button::draw_background(Camera *camera)
{
	Sprite_Manager::draw(camera, background->id);
}

void Button::draw_text()
{
	Graphics::draw_text(text);
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
