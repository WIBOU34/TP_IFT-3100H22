/**
 * \IFT3100H21
 * \file tessellationsRenderer.h
 * \author Stéphane Boulanger
 * \brief Classe responsable de la tessellation de l'application
 * \version 0.1
 * \date 2022-04-05
 */

#pragma once

#include "ofMain.h"
#include "../renderer.h"
#include "../objectBase3D.h"
#include "ofxGui.h"

class TessellationRenderer : public RendererBase {
public : 
	ofParameterGroup parameters;	
	ofxToggle tessellation_toggle;
		
	ofParameterGroup parameters_tessellation;
	ofxButton box_tess_button;
	ofxButton sphere_tess_button;
	ofParameter<int> tess_level_slider;
	ofParameter<int> tess_resolution_slider;
	ofParameter<int> patch_vertice_slider;

	ofEasyCam cam_tess;
	ofShader shader_tess;
	
	// sphere
	ofVbo sphereVbo;
	ofVboMesh sphereMesh;
	ofSpherePrimitive sphere;
	float radius;
	
	// box
	ofVbo boxVbo;
	ofVboMesh boxMesh;
	ofBoxPrimitive box;

	// plane 
	ofVbo planeVbo;
	ofVboMesh planeMesh;
	ofPlanePrimitive plane;

	bool sphere_active;
	bool box_active;

	void buttonBoxTessellation();
	void buttonSphereTessellation();

protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();

};