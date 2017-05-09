#include <simple_logger.h>

#include "graphics.h"
#include "json_helper.h"
#include "player.h"

Player::Player(std::string racer_def_file, glm::vec3 position, float rotation) : Racer(racer_def_file, position, rotation)
{
	racer_type = PlayerRacer;
	glm::vec3 cameraPosition = glm::vec3(position.x, position.y, position.z - 1.0f);
	player_cam = new Camera(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT), cameraPosition);
	
	entity_component->think = &think;

	//HUD components load
	leader_1 = Sprite_Manager::create_sprite("json/GUI/sprites/joe_sprite.json");
	leader_2 = Sprite_Manager::create_sprite("json/GUI/sprites/enemy_icon_sprite.json");
	item_backdrop = Sprite_Manager::create_sprite("json/GUI/sprites/wood_sprite.json");

	if (!hud_font.loadFromFile("fonts/Spicy.ttf"))
	{
		printf("dun goofed\n");
	}

	mph.setFont(hud_font);
	mph.setString("MPH");
	mph.setCharacterSize(32);
	mph.setFillColor(sf::Color::Red);
	mph.setOutlineColor(sf::Color::Black);
	mph.setOutlineThickness(2.0f);
	mph.setPosition(WINDOW_WIDTH - 500, WINDOW_HEIGHT - 42);

	speed.setFont(hud_font);
	speed.setCharacterSize(64);
	speed.setFillColor(sf::Color::Red);
	speed.setOutlineColor(sf::Color::Black);
	speed.setOutlineThickness(4.0f);
	speed.setPosition(mph.getPosition().x + speed.getLocalBounds().width + 10.0f, WINDOW_HEIGHT - 74);

	lap_num_text.setFont(hud_font);
	lap_num_text.setString(std::to_string(lap_number) + "/3");
	lap_num_text.setCharacterSize(64);
	lap_num_text.setFillColor(sf::Color::Red);
	lap_num_text.setOutlineColor(sf::Color::Black);
	lap_num_text.setOutlineThickness(4.0f);
	lap_num_text.setPosition(WINDOW_WIDTH - (lap_num_text.getLocalBounds().width + 10.0f), WINDOW_HEIGHT - (lap_num_text.getLocalBounds().height + 10.0f));

	lap_text.setFont(hud_font);
	lap_text.setString("LAP");
	lap_text.setCharacterSize(32);
	lap_text.setFillColor(sf::Color::Red);
	lap_text.setOutlineColor(sf::Color::Black);
	lap_text.setOutlineThickness(2.0f);
	lap_text.setPosition(WINDOW_WIDTH - (lap_text.getLocalBounds().width + 50.0f), WINDOW_HEIGHT - (lap_num_text.getLocalBounds().height + 42));

	in_lead = false;
}

Player::~Player()
{
	Entity_Manager::delete_entity(entity_component->id);
	Sprite_Manager::delete_sprite(leader_1->id);
	Sprite_Manager::delete_sprite(item_backdrop->id);

}

void Player::draw_player_hud_sprites()
{
	Sprite_Manager::draw(player_cam, item_backdrop->id);
	if(in_lead)
		Sprite_Manager::draw(player_cam, leader_1->id);
	else
		Sprite_Manager::draw(player_cam, leader_2->id);
	
}

void Player::draw_player_hud_text()
{
	speed.setString(std::to_string((int)std::round(entity_component->current_speed / 10.0f)));
	speed.setPosition(mph.getPosition().x - speed.getLocalBounds().width - 10.0f, WINDOW_HEIGHT - 74);
	Graphics::draw_text(speed);
	Graphics::draw_text(mph);
	Graphics::draw_text(lap_text);
	lap_num_text.setString(std::to_string(lap_number) + "/3");
	Graphics::draw_text(lap_num_text);
}

void Player::update_player_cam()
{
	player_cam->follow_entity(entity_component);
	player_cam->view_matrix_look_at_target();
}

void Player::update(float delta_time)
{
	int forward;
	forward = 0;
	bool turn_slow = false;
	float idle_slow = 3.0f;

	//uniform removal of some speed every frame if player is holding forward or backward this shouldn't matter
	if(entity_component->current_speed > 0)
		entity_component->current_speed = glm::max(entity_component->current_speed - entity_component->deacceleration_rate / idle_slow, 0.0f);
	else if (entity_component->current_speed < 0)
		entity_component->current_speed = glm::min(entity_component->current_speed + entity_component->acceleration_rate / idle_slow, 0.0f);


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		forward = 1;
		entity_component->current_speed += entity_component->acceleration_rate;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		forward = -1;
		entity_component->current_speed -= entity_component->deacceleration_rate;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		entity_component->current_rotation -= entity_component->handling * 20 * delta_time;
		turn_slow = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		entity_component->current_rotation += entity_component->handling * 20 * delta_time;
		turn_slow = true;
	}
	if (turn_slow)
	{
		if (entity_component->current_speed > entity_component->top_speed - (entity_component->top_speed * 1.0f / (1.0f + (entity_component->handling / 50.0f))))
		{
			entity_component->current_speed = glm::max(entity_component->top_speed - (entity_component->top_speed * 1.0f / (1.0f + (entity_component->handling / 50.0f))), entity_component->current_speed - entity_component->deacceleration_rate);
		}
		if (entity_component->current_speed < -entity_component->top_speed * 0.5f -(entity_component->top_speed * 0.5f * 1.0f / (1.0f + (entity_component->handling / 50.0f))))
		{
			entity_component->current_speed = glm::min(-entity_component->top_speed * 0.5f - (entity_component->top_speed * 0.5f * 1.0f / (1.0f + (entity_component->handling / 50.0f))), entity_component->current_speed + entity_component->deacceleration_rate);
		}
	}
	else if (!turn_slow)
	{
		if (entity_component->current_speed > entity_component->top_speed)
		{
			entity_component->current_speed = glm::max(entity_component->top_speed, (entity_component->current_speed + 50.0f) * delta_time);
		}
		if (entity_component->current_speed < -entity_component->top_speed * 0.5f)
		{
			entity_component->current_speed = glm::min(-entity_component->top_speed * 0.5f, (entity_component->current_speed - 50.0f) * delta_time);
		}
	}

	racer_base_update(entity_component);
}

void think(Entity *ent)
{

}
