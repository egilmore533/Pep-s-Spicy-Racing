#ifndef __GUI_H__
#define __GUI_H__

#include <vector>

#include "button.h"

class GUI {

public:
	GUI();

	void draw_all(Camera *cam);
	void update_all();

private:
	void draw_all_buttons(Camera *cam);

	void draw_all_sprites();

	std::vector<Sprite *> sprite_list;
	std::vector<Button *> button_list;

	int num_buttons;
	int num_sprites;
};

#endif