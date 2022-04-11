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

	// menu gui tessellation 
	ofParameterGroup parameters_tessellation;
	ofxButton shader_tess_button;
	ofParameter<int> tess_level_slider;
	ofParameter<int> tess_resolution_slider;

	ofEasyCam cam_tex;

	ofShader shader_tess;
	ofVbo sphereVbo;
	ofVboMesh sphereMesh;
	ofSpherePrimitive sphere;
	float radius;
	bool load_shader;

	void buttonShaderTessellation();

protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();

};