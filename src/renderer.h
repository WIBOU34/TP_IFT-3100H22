// IFT3100H21_AlmostEmptyProject/renderer.h
// Classe responsable du rendu de l'application.

#pragma once

#include "ofMain.h"

class RendererBase {
public:
	//RendererBase();
	float time_current;
	float time_last;
	float time_elapsed;

	void setup(const std::string& name);
	void update();
	void draw();

	void mousButtonPressed(int x, int y);
	void mouseButtonReleased(int x, int y);
	void setMousePos(int x, int y);

	const ofVec3f origin = ofVec3f(0.0f, 0.0f, 0.0f);
	const ofVec3f centerScreen = ofVec3f(ofGetWidth()/2.0f, ofGetHeight()/2.0f, 0.0f);

protected:
	virtual void setupRenderer(const std::string& name) = 0;
	virtual void updateRenderer() = 0;
	virtual void generateDraw() = 0;
	virtual void render() = 0;
	bool needsRedraw;
	int mouse_press_x;
	int mouse_press_y;
	int mouse_press_x_updated;
	int mouse_press_y_updated;
	int mouse_current_x;
	int mouse_current_y;
	int mouse_released_x;
	int mouse_released_y;
	bool is_mouse_button_pressed;

private:
	unsigned long currentFrame;
};