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

		graphics->Graphics::graphics_frame_begin();

		/*Drawing Code Start*/

		/*Drawing Code Start*/

		graphics->Graphics::graphics_next_frame();


	}

	return 0;
}