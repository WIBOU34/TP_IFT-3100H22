#pragma once

//#include "renderer.h"
#include "../renderer.h"
#include "../objectBase.h"
#include <list>


class SphereRenderer : public RendererBase {
public:
	void setup(const std::string& name);
	void updateCustom();
	void createSphere();

	//vec3 v = vec3(1.0, 2.0, 3.0);

	float radiusSphere;

	// options pour la sphere
	ofParameter<float> radiusSphereSlider;
	ofParameterGroup sphereParameters;

	ofSpherePrimitive sphere1;
	ofSpherePrimitive sphere2;

protected:
	void generateDraw();
	void render();
};
