// IFT3100H21_AlmostEmptyProject/renderer.h
// Classe responsable du rendu de l'application.

#pragma once

#include "ofMain.h"


class TextureRenderer : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	ofMesh mesh;
	ofEasyCam cam;
	ofImage image;
	ofLight light;

	ofShader shader;
};