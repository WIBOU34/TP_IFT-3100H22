#pragma once

#include "../renderer.h"
#include "../objects3D/objects3DRenderer.h"
#include <list>
#include "ofMath.h"
#include "ofxGui.h"
#include "gpu/smallptGPU.h"
#include "raytracer.h"

class RaytracingManager : public RendererBase {
public:
	Objects3DRenderer* obj3Drenderer;
	ofCamera* camera;
	ofParameterGroup parameters;
	ofxButton btnGenerateRaytracedImage;
	ofxLabel warningNbrThreads;
	ofxLabel lblSceneName;
	ofParameter<int> nbThreads;
	ofParameter<int> rayPerPixel;
	ofParameter<int> depthMax;
	ofParameter<int> imageWidth;
	ofParameter<int> imageHeight;
	ofParameter<int> sceneIndex;
	Raytracer raytracer;
	RaytracerGpu raytracerGpu;

protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();

private:
	void generateRaytracedImage();
	std::vector<CubeCpu> cubeVector = std::vector<CubeCpu>();
	std::vector<SphereCpu> sphereVector = std::vector<SphereCpu>();
	std::vector<std::string> sceneNames = std::vector<std::string>();
};
