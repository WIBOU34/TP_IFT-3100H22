#pragma once

//#include "renderer.h"
#include "../renderer.h"
#include "../objectBase3D.h"
#include <list>


// Tests de rendu des spheres
class SphereRenderer : public RendererBase {
public:
	void createSphere();

	//vec3 v = vec3(1.0, 2.0, 3.0);
	const ofVec3f position;

	float radiusSphere;

	// options pour la sphere
	ofParameter<float> radiusSphereSlider;
	ofParameter<float> xSphereSlider;
	ofParameter<float> ySphereSlider;
	ofParameter<float> zSphereSlider;
	ofParameterGroup sphereParameters;

	ofSpherePrimitive sphere1;
	ofSpherePrimitive sphere2;

protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();
};
