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
	std::list<ObjectBase3D<VectorObjs3D>> lstObj;
	ObjectBase3D<VectorObjs3D> objTemporaire;
	ObjectBase3D<VectorObjs3D>* selectedObj;

	void setup(const std::string& name);
	void updateCustom();

	ofxGuiGroup parameters3D;
	ofParameter<ofColor> fillColor;
	ofxButton bDrawCube;
	ofxButton bDrawSphere;
	ofxButton bDrawCone;
	ofxButton bDrawCylinder;

	ofParameter<string> idSelected;
	ofParameter<string> typeSelected;
	ofParameter<ofColor> fillColorSelected;

	void cube(float x, float y, float z, float width, float height, float length);

	void selectObject();

protected:
	void generateDraw();
	void render();
private:
	const std::string NO_ITEM_SELECTED = "NULL";

	ofxVectorGraphics graphics;

	std::list<VectorObjectSettings> lstObjSettings;
	bool updateShapeObj3D = false;
	bool saveShape = true;

	void drawObjects(const ObjectBase3D<VectorObjs3D>& forme);

	VectorObjectSettings createNewObj();
	VectorObjectSettings createCube(float x, float y, float z, float width, float height, float length);
	void updateCube(VectorObjectSettings& obj, float x, float y, float z, float width, float height, float length);
	VectorObjectSettings createSphere(float x, float y, float z, float r);
	void updateSphere(VectorObjectSettings& obj, float x, float y, float z, float r);
	VectorObjectSettings createCone(float x, float y, float z, float r, float height);
	void updateCone(VectorObjectSettings& obj, float x, float y, float z, float r, float height);
	VectorObjectSettings createCylinder(float x, float y, float z, float r, float height);
	void updateCylinder(VectorObjectSettings& obj, float x, float y, float z, float r, float height);

	void buttonCubePressed();
	void buttonSpherePressed();
	void buttonConePressed();
	void buttonCylinderPressed();

	//bool isPointOnLine(const ofVec2f& start, const ofVec2f& end, const ofVec2f& find) const;
	//bool isPointOnEllipse(const ofVec2f& center, const ofVec2f& radiusXY, const ofVec2f& find) const;
};

