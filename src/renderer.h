// IFT3100H21_AlmostEmptyProject/renderer.h
// Classe responsable du rendu de l'application.

#pragma once

#include "ofMain.h"
//#include "image/image.h"

class RendererBase {
public:
	RendererBase();
	float time_current;
	float time_last;
	float time_elapsed;

	void setup();
	void update();
	void draw();


	int mouse_press_x;
	int mouse_press_y;
	int mouse_current_x;
	int mouse_current_y;
	bool is_mouse_button_pressed;

	const ofVec3f origin = ofVec3f(0.0f, 0.0f, 0.0f);

protected:
	virtual void generateDraw() = 0;
	virtual void render() = 0;
	bool needsRedraw;

private:
	unsigned long currentFrame;
};
