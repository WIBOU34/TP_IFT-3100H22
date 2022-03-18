// IFT3100H21_AlmostEmptyProject/renderer.h
// Classe responsable du rendu de l'application.

#pragma once

#include "ofMain.h"
#include "../renderer.h"


class TextureRenderer : public RendererBase {
public:
	ofParameterGroup parameters;

	ofMesh mesh;
	ofEasyCam cam;
	ofImage image;
	ofLight light;

	ofShader shader;

protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();
};