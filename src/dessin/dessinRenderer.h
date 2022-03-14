#pragma once

#include "../renderer.h"
#include "../objectBase.h"
#include "ofxVectorGraphics.h"
#include <list>
#include "ofMath.h"
#include "ofxGui.h"


class DessinRenderer : public RendererBase {
public:
	std::list<ObjectBase2D<VectorForme>> lstFormes;
	ObjectBase2D<VectorForme> formeTemporaire;
	ObjectBase2D<VectorForme>* selectedForme;


	ofParameterGroup parameters;
	ofParameter<int> strokeWidth;
	ofParameter<ofColor> strokeColor;
	ofParameter<ofColor> fillColor;
	ofxButton btnDrawPoint;
	ofxButton btnDrawSquare;
	ofxButton btnDrawRectangle;
	ofxButton btnDrawCircle;
	ofxButton btnDrawEllipse;
	ofxButton btnDrawLine;
	ofxButton btnDrawTriangle;
	ofxButton btnStopDrawing;
	ofxButton btnDeleteSelected;
	bool bDrawPoint;
	bool bDrawSquare;
	bool bDrawRectangle;
	bool bDrawCircle;
	bool bDrawEllipse;
	bool bDrawLine;
	bool bDrawTriangle;

	ofParameter<string> idSelected;
	ofParameter<string> typeSelected;
	ofParameter<int> strokeWidthSelected;
	ofParameter<ofColor> strokeColorSelected;
	ofParameter<ofColor> fillColorSelected;

	//ofEventListener listenerDelete;

	void point(float x, float y);
	void square(float x1, float y1, float x2, float y2);
	void rect(float x1, float y1, float x2, float y2);
	//void triangle(float x1, float y1, float x2, float y2, float x3, float y3);
	void circle(float xc, float yc, float x2, float y2);
	void ellipse(float x1, float y1, float w, float h);
	void line(float x1, float y1, float x2, float y2);
	//void arc(float x, float y, float radius, float offsetAngleDegrees, float internalAngleDegrees);
	//void bezier(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
	//void curve(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

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
	const ofColor DEFAULT_COLOR = ofColor(0);
	const ofColor SELECTED_COLOR = ofColor::darkGrey;

	const std::string NO_ITEM_SELECTED = "NULL";
	const std::string DRAW_POINT_NAME = "Dessiner un point";
	const std::string DRAW_SQUARE_NAME = "Dessiner un carre";
	const std::string DRAW_RECTANGLE_NAME = "Dessiner un rectangle";
	const std::string DRAW_CIRCLE_NAME = "Dessiner un cercle";
	const std::string DRAW_ELLIPSE_NAME = "Dessiner une ellipse";
	const std::string DRAW_LINE_NAME = "Dessiner une ligne";
	const std::string DRAW_TRIANGLE_NAME = "Dessiner un triangle";

	ofxVectorGraphics graphics;

	std::list<VectorPrimitive> lstPrimitives;
	bool updateShape = false;
	bool saveShape = false;

	void drawPrimitives(const ObjectBase2D<VectorForme>& forme);

	VectorPrimitive createPrimitive();
	VectorPrimitive createPoint(float x, float y);
	void updatePoint(VectorPrimitive& primitive, float x, float y);
	VectorPrimitive createSquare(float x1, float y1, float x2, float y2);
	void updateSquare(VectorPrimitive& primitive, float x1, float y1, float x2, float y2);
	VectorPrimitive createRectangle(float x1, float y1, float x2, float y2);
	void updateRectangle(VectorPrimitive& primitive, float x1, float y1, float x2, float y2);
	VectorPrimitive createLine(float x1, float y1, float x2, float y2);
	void updateLine(VectorPrimitive& primitive, float x1, float y1, float x2, float y2);
	VectorPrimitive createCircle(float xc, float yc, float x2, float y2);
	void updateCircle(VectorPrimitive& primitive, float xc, float yc, float x2, float y2);
	VectorPrimitive createEllipse(float xc, float yc, float x2, float y2);
	void updateEllipse(VectorPrimitive& primitive, float xc, float yc, float x2, float y2);

	bool isPointOnLine(const ofVec2f& start, const ofVec2f& end, const ofVec2f& find) const;
	bool isPointOnEllipse(const ofVec2f& center, const ofVec2f& radiusXY, const ofVec2f& find) const;
	bool isPointInRectangle(const ofVec2f& start, const ofVec2f& end, const ofVec2f& find) const;


	void deleteSelected();
	void deselectButtonsPoint();
	void deselectButtonsSquare();
	void deselectButtonsRectangle();
	void deselectButtonsCircle();
	void deselectButtonsEllipse();
	void deselectButtonsLine();
	void deselectButtonsTriangle();
	void deselectButtons();
};
