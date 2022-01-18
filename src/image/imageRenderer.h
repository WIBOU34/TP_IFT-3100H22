#pragma once

//#include "renderer.h"
#include "../renderer.h"
class ImageRenderer : public RendererBase {
public:
	//ImageRenderer imageRenderer;
	ofImage image;

	void importImage(const std::string& path);
	void exportImage(const std::string& path);
protected:
	void generateDraw();
	void render();
private:
	void drawImage();
};
