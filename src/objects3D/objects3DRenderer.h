#pragma once

#include "../renderer.h"
#include "../objectBase3D.h"
#include "ofxVectorGraphics.h"
//#include <list>
#include <vector>
#include "ofMath.h"
#include "ofxGui.h"


class Objects3DRenderer : public RendererBase {
public:
	std::list<ObjectBase3D<VectorObjSettings>> lstObjSettings;
	ObjectBase3D<VectorObjSettings> objTemporaire;
	ObjectBase3D<VectorObjSettings>* selectedObj;

	void setup(const std::string& name);
	void updateCustom();

	ofxGuiGroup parameters3D;
	ofParameter<ofColor> fillColor;
	ofxButton bDrawCube;
	ofxButton bDrawSphere;
	ofxButton bDrawCone;
	ofxButton bDrawCylinder;

	ofxGuiGroup selectedObjParams;
	ofParameter<float> sliderXpos;
	ofParameter<float> sliderYpos;
	ofParameter<float> sliderZpos;
	ofParameter<float> sliderRadiusPos;
	ofParameter<float> sliderWidthPos;
	ofParameter<float> sliderHeightPos;
	ofParameter<float> sliderLengthPos;

	ofParameter<string> idSelected;
	ofParameter<string> typeSelected;
	ofParameter<ofColor> fillColorSelected;

	void cube(float x, float y, float z, float width, float height, float length);
	void sphere(float x, float y, float z, float r);

	void selectObject();

protected:
	void generateDraw();
	void render();
private:
	const std::string NO_ITEM_SELECTED = "NULL";

	ofxVectorGraphics graphics;

	std::list<VectorObj> lstObjs;
	bool updateShapeObj3D = false;
	bool saveShape = true;

	void drawObjects(const ObjectBase3D<VectorObjSettings>& forme);

	VectorObj createNewObj();
	VectorObj createCube(float x, float y, float z, float width, float height, float length);
	void updateCube(VectorObj& obj, float x, float y, float z, float width, float height, float length);
	VectorObj createSphere(float x, float y, float z, float r);
	void updateSphere(VectorObj& obj, float x, float y, float z, float r);
	VectorObj createCone(float x, float y, float z, float r, float height);
	void updateCone(VectorObj& obj, float x, float y, float z, float r, float height);
	VectorObj createCylinder(float x, float y, float z, float r, float height);
	void updateCylinder(VectorObj& obj, float x, float y, float z, float r, float height);

	void buttonCubePressed();
	void buttonSpherePressed();
	void buttonConePressed();
	void buttonCylinderPressed();

	//bool isPointOnLine(const ofVec2f& start, const ofVec2f& end, const ofVec2f& find) const;
	//bool isPointOnEllipse(const ofVec2f& center, const ofVec2f& radiusXY, const ofVec2f& find) const;
};

