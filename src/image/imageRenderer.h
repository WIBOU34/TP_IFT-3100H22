#pragma once

//#include "renderer.h"
#include "../renderer.h"
#include "../objectBase.h"
#include <list>


class ImageRenderer : public RendererBase {
public:
	std::list<ObjectBase2D<ofImage>> lstImages;

	void setup(const std::string& name);
	void importImage(const std::string& path);
	void importImage(const std::string& path, const int& x, const int& y);
	void importImage(const std::string& path, const int& x, const int& y, const int& width, const int& height);
	void importImageDialog();
	void exportImageDialog() const;
	void findImage(const int& x, const int& y);

	ofParameterGroup parameters;
protected:
	void generateDraw();
	void render();
private:
	void drawImage();
	void exportImage(const std::string& path, const std::string& fileName) const;
};
