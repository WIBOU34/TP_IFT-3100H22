#pragma once

#include "../renderer.h"
#include "../objectBase.h"
#include "ofxVectorGraphics.h"
//#include <list>
#include <vector>
#include "ofMath.h"


class DessinRenderer : public RendererBase {
public:
	std::list<ObjectBase2D<VectorForme>> lstFormes;
	ObjectBase2D<VectorForme> formeTemporaire;
	ObjectBase2D<VectorForme>* selectedForme;

	ofParameterGroup parameters;
	ofParameter<int> strokeWidth;
	ofParameter<ofColor> strokeColor;
	ofParameter<ofColor> fillColor;
	ofParameter<bool> bDrawRectangle;
	ofParameter<bool> bDrawEllipse;
	ofParameter<bool> bDrawLine;
	ofParameter<bool> bDrawTriangle;

	ofParameter<string> idSelected;
	ofParameter<string> typeSelected;
	ofParameter<int> strokeWidthSelected;
	ofParameter<ofColor> strokeColorSelected;
	ofParameter<ofColor> fillColorSelected;

	void rect(float x1, float y1, float x2, float y2);
	void triangle(float x1, float y1, float x2, float y2, float x3, float y3);
	void circle(float x1, float y1, float radius);
	void ellipse(float x1, float y1, float w, float h);
	void line(float x1, float y1, float x2, float y2);
	void arc(float x, float y, float radius, float offsetAngleDegrees, float internalAngleDegrees);
	void bezier(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
	void curve(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

	void selectPrimitive();
	void beginShapeDraw();
	void drawAtMouse();
	void completeShapeDrawn();
protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();
private:
	const std::string NO_ITEM_SELECTED = "NULL";

	ofxVectorGraphics graphics;

	std::list<VectorPrimitive> lstPrimitives;
	bool updateShape = false;
	bool saveShape = false;

	void drawPrimitives(const ObjectBase2D<VectorForme>& forme);

	VectorPrimitive createPrimitive();
	VectorPrimitive createRectangle(float x1, float y1, float x2, float y2);
	void updateRectangle(VectorPrimitive& primitive, float x1, float y1, float x2, float y2);
	VectorPrimitive createLine(float x1, float y1, float x2, float y2);
	void updateLine(VectorPrimitive& primitive, float x1, float y1, float x2, float y2);
	VectorPrimitive createEllipse(float xc, float yc, float x2, float y2);
	void updateEllipse(VectorPrimitive& primitive, float xc, float yc, float x2, float y2);

	bool isPointOnLine(const ofVec2f& start, const ofVec2f& end, const ofVec2f& find) const;
	bool isPointOnEllipse(const ofVec2f& center, const ofVec2f& radiusXY, const ofVec2f& find) const;
};

