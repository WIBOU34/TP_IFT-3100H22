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

protected:
	virtual void generateDraw() = 0;
	virtual void render() = 0;

private:
	bool needsRedraw;
	unsigned long currentFrame;
};
