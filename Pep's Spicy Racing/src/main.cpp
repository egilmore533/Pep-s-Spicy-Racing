#include "graphics.h"

int main()
{
	int running = 1;
	Graphics *graphics = new Graphics;
	

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

		graphics->Graphics::get_game_window()->clear();

		graphics->Graphics::get_game_window()->display();
	}

	return 0;
}