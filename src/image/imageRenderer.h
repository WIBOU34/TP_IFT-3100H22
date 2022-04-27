#pragma once

#include "../renderer.h"
#include "../objectBase.h"
#include <list>
#include "ofxGui.h"

class ofImageExtended : public ofImage {
public:
	bool operator==(const ofImageExtended& image) const {
		return image.getWidth() == this->getWidth() && image.getHeight() == this->getHeight();
	}
};

class ImageRenderer : public RendererBase {
public:
	std::list<ObjectBase2D<ofImageExtended>> lstImages;
	ObjectBase2D<ofImageExtended>* imageSelected;

	void importImage(const std::string& path);
	void importImage(const std::string& path, const int& x, const int& y);
	void importImage(const std::string& path, const int& x, const int& y, const int& width, const int& height);
	void importImageDialog();
	void exportImageDialog() const;
	void findImage(const int& x, const int& y);

	ofParameter<float> positionX;
	ofParameter<float> positionY;
	ofParameter<float> positionXend;
	ofParameter<float> positionYend;
	ofxLabel lblDimensions;
	ofxLabel name;
	ofParameterGroup parameters;
	ofxButton btnDeleteSelected;
	ofxButton btnResetSelected;
protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();
private:
	const std::string NO_ITEM_SELECTED = "NULL";
	void drawImage();
	void exportImage(const std::string& path, const std::string& fileName) const;
	void deleteSelected();
	void resetSelected();
};
