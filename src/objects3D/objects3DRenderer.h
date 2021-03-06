#pragma once

#include "../renderer.h"
#include "../objectBase3D.h"
#include "ofxAssimpModelLoader.h"
#include <list>
#include "ofMath.h"
#include "ofxGui.h"

class ofxAssimpModelLoaderExtended : public ofxAssimpModelLoader {
public:
	bool operator==(const ofxAssimpModelLoaderExtended& model) const {
		return model.pos == this->pos;
	}
};

class Objects3DRenderer : public RendererBase {
public:
	std::list<ObjectBase3D<VectorObjSettings>> lstObjSettings;
	ObjectBase3D<VectorObjSettings>* selectedObj;
	ObjectBase3D<ofxAssimpModelLoaderExtended*>* selectedModelObj;
	ofLight light;

	// import 3D model
	std::list<ObjectBase3D<ofxAssimpModelLoaderExtended*>> listObjImport;
	std::list<ofxAssimpModelLoaderExtended> listModelImport;
	ofxButton importObjButton;
	void importObj(const std::string& path, const int& x, const int& y);
	void buttonImportObjPressed();

	ofxGuiGroup parameters3D;
	ofParameter<ofColor> fillColor;
	ofxButton bDrawCube;
	ofxButton bDrawSphere;
	ofxButton bDrawCone;
	ofxButton bDrawCylinder;
	ofxButton btnDeleteSelected;
	ofParameter<bool> boundingBox;

	ofxGuiGroup selectedObjParams;
	ofParameter<float> sliderXpos;
	ofParameter<float> sliderYpos;
	ofParameter<float> sliderZpos;
	ofParameter<float> sliderRadiusPos;
	ofParameter<float> sliderWidthPos;
	ofParameter<float> sliderHeightPos;
	ofParameter<float> sliderLengthPos;

	ofParameter<int> sliderIdObjs;
	ofParameter<int> sliderIdModelObjs;
	int oldSelectedId;
	int oldSelectedModelId;
	ofxLabel idSelected;
	ofParameter<string> typeSelected;
	ofParameter<ofColor> fillColorSelected;

	void cube(float x, float y, float z, float width, float height, float length);
	void sphere(float x, float y, float z, float r);
	void cone(float x, float y, float z, float r, float height);
	void cylinder(float x, float y, float z, float r, float height);

	void selectObject();

protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();
private:
	const std::string NO_ITEM_SELECTED = "NULL";

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
	void updateOutline(VectorOutline& outline, float x, float y, float z, float width, float height, float length);

	void buttonCubePressed();
	void buttonSpherePressed();
	void buttonConePressed();
	void buttonCylinderPressed();
	void deleteSelected();

	void getBoundingBox(ofxAssimpModelLoaderExtended& model, glm::vec3& cornerMin, glm::vec3& cornerMax);
	void getBoundingBox(ofxAssimpModelLoaderExtended& model, glm::vec4& cornerMin, glm::vec4& cornerMax);
};
