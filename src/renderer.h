// IFT3100H21_AlmostEmptyProject/renderer.h
// Classe responsable du rendu de l'application.

#pragma once

#include "ofMain.h"
//#include "image/image.h"

class RendererBase {
public:
	//RendererBase();
	float time_current;
	float time_last;
	float time_elapsed;

	void setup();
	void update();
	void draw();

	//static void mousButtonPressed(int x, int y);
	//static void mouseButtonReleased(int x, int y);
	//static void setMousePos(int x, int y);
	void mousButtonPressed(int x, int y);
	void mouseButtonReleased(int x, int y);
	void setMousePos(int x, int y);

protected:
	virtual void generateDraw() = 0;
	virtual void render() = 0;
	bool needsRedraw;
	//static int mouse_press_x;
	//static int mouse_press_y;
	//static int mouse_current_x;
	//static int mouse_current_y;
	//static int mouse_released_x;
	//static int mouse_released_y;
	//static bool is_mouse_button_pressed;
	int mouse_press_x;
	int mouse_press_y;
	int mouse_current_x;
	int mouse_current_y;
	int mouse_released_x;
	int mouse_released_y;
	bool is_mouse_button_pressed;

private:
	unsigned long currentFrame;
};
