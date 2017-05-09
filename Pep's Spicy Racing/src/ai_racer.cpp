#include "graphics.h"
#include "ai_racer.h"

AI_Racer::AI_Racer(std::string racer_def_file, glm::vec3 position, float rotation) : Racer(racer_def_file, position, rotation)
{
	racer_type = Bot;
	entity_component->think = &think;
	entity_component->current_rotation = glm::abs(entity_component->current_rotation);
}

void AI_Racer::update(float delta_time)
{
	int forward;
	forward = 0;
	bool turn_slow = false;
	float idle_slow = 3.0f;

	
	//uniform removal of some speed every frame
	if (entity_component->current_speed > 0)
		entity_component->current_speed = glm::max(entity_component->current_speed - entity_component->deacceleration_rate / idle_slow, 0.0f);
	else if (entity_component->current_speed < 0)
		entity_component->current_speed = glm::min(entity_component->current_speed + entity_component->acceleration_rate / idle_slow, 0.0f);

	//get the target roationf for reaching the next node at the cars current rotation
	float z_diff = check_this_node.position.z - entity_component->world_position.z;
	float x_diff = check_this_node.position.x - entity_component->world_position.x;

	target_rotation = -glm::degrees(glm::atan(z_diff, x_diff));

	//determine what driving state the AI should be in
	update_driving_state();

	if (target_rotation != entity_component->current_rotation)
		turn_slow = true;

	//set the car's rotation to increment towards that rotation
	entity_component->current_rotation = target_rotation;

	/*if (entity_component->current_rotation > target_rotation)
	{
		entity_component->current_rotation = glm::max(target_rotation, entity_component->current_rotation - (entity_component->handling * 20 * delta_time));
		turn_slow = true;
	}
	if (entity_component->current_rotation < target_rotation)
	{
		entity_component->current_rotation = glm::min(target_rotation, entity_component->current_rotation - (entity_component->handling * 20 * delta_time));
		turn_slow = true;
	}*/

	if (state == Stop)
	{
		forward = -1;
	}
	else if (state == Gas)
	{
		forward = 1;
	}
	else if (state == Brakes)
	{
		forward = 0;
	}

	if (forward == 1)
	{
		entity_component->current_speed += entity_component->acceleration_rate;
	}
	else if (forward == -1)
	{
		entity_component->current_speed = glm::max(0.0f, entity_component->current_speed - entity_component->deacceleration_rate);
	}
	else
	{
		entity_component->current_speed += entity_component->acceleration_rate * 0.5f;
	}

	if (turn_slow)
	{
		if (entity_component->current_speed > entity_component->top_speed - (entity_component->top_speed * 1.0f / (1.0f + (entity_component->handling / 50.0f))))
		{
			entity_component->current_speed = glm::max(entity_component->top_speed - (entity_component->top_speed * 1.0f / (1.0f + (entity_component->handling / 50.0f))), entity_component->current_speed - entity_component->deacceleration_rate);
		}
		if (entity_component->current_speed < -entity_component->top_speed * 0.5f - (entity_component->top_speed * 0.5f * 1.0f / (1.0f + (entity_component->handling / 50.0f))))
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

void AI_Racer::update_driving_state()
{
	float difference_rotation_needed = glm::abs(target_rotation) - glm::abs(entity_component->current_rotation);



	//if the next node is to the right or left stop
	if (glm::abs(difference_rotation_needed) > 60.0f)
	{
		state = Stop;
	}

	//if the next node is at a 45 degree angle slow down to half speed
	else if (glm::abs(difference_rotation_needed) > 1.0f)
	{
		state = Brakes;
	}

	//if the next node is straight ahead gas
	else
	{
		state = Gas;
	}
}

