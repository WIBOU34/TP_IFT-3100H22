#pragma once

#include "../renderer.h"
#include "../objects3D/objects3DRenderer.h"
#include "ofxAssimpModelLoader.h"
#include <list>
#include "ofMath.h"
#include "ofxGui.h"
#include "raytracer.h"

class RaytracingManager : public RendererBase {
public:
	Objects3DRenderer* obj3Drenderer;
	ofCamera* camera;
	ofParameterGroup parameters;
	ofxButton btnGenerateRaytracedImage;
	ofxButton btnSetupCornellBox;
	ofxButton btnSetupTestBox;
	ofxLabel warningNbrThreads;
	ofParameter<int> nbThreads;
	ofParameter<int> rayPerPixel;
	ofParameter<int> imageWidth;
	ofParameter<int> imageHeight;
	Raytracer raytracer;

protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();

private:
	void generateRaytracedImage();
	void setupBoiteCornell();
	void setupTestBoite();
	void transfer3DObjectsForRaytracing();
	std::vector<Cube> cubeVector = std::vector<Cube>();
	std::vector<Sphere> sphereVector = std::vector<Sphere>();
};
