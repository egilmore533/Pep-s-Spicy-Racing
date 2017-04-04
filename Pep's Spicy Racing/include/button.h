#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "sprite_manager.h"
#include "graphics.h"
#include "camera.h"

class Button {
public:
	Button(std::string button_def_file);
	~Button();

	void update();
	void draw(Camera *camera);

	Sprite *background;
	Sprite *text;
	int state;
	bool clicked;

	void (*callback)();

};



#endif
