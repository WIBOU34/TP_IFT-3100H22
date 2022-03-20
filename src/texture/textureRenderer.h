// IFT3100H21
// Classe responsable du rendu de l'application.

#pragma once

#include "ofMain.h"
#include "../renderer.h"
#include "../objectBase3D.h"
#include "ofxGui.h"

enum class ConvolutionKernel
{
	identity,
	emboss, 
	sharpen, 
	edge_detect
};

// kernel identit�
const std::array<float, 9> convolution_kernel_identity
{
  0.0f,  0.0f,  0.0f,
  0.0f,  1.0f,  0.0f,
  0.0f,  0.0f,  0.0f
};


// kernel emboss
const std::array<float, 9> convolution_kernel_emboss
{
	-2.0, -1.0f, 0.0f, 
	-1.0f, 1.0f, 1.0f, 
	 0.0f, 1.0f, 2.0f
};

// kernel sharpen
const std::array<float, 9> convolution_kernel_sharpen
{
	 0.0, -1.0f, 0.0f,
	-1.0f, 5.0f, -1.0f,
	 0.0f, -1.0f, 0.0f
};

// kernel edge_detect
const std::array<float, 9> convolution_kernel_edge_detect
{
	 0.0,  1.0f, 0.0f,
	 1.0f, -4.0f, 1.0f,
	 0.0f, -1.0f, 0.0f
};



class TextureRenderer : public RendererBase {
public:

	// menu gui 
	ofParameterGroup parameters;
	ofxToggle mesh_sphere_toggle; 	
	ofxLabel identite_label;
	ofxLabel emboss_label; 
	ofxLabel sharpen_label; 
	ofxLabel edge_detect_label;
	ofParameter<float> slider_exposure;

	// sphere de mesh
	ofMesh mesh;
	ofEasyCam cam;
	ofImage image;
	ofImage image_selection; 
	ofLight light;
	ofShader shader_geom;

	// filtre de convolution
	ConvolutionKernel kernel_type; 
	string kernel_name; 
	ofImage image_destination;
	int image_width;
	int image_height;
	int offset_vertical;
	int offset_horizontal;

	// mappage tonal 
	ofShader shader_tone_map; 
	int offset_vertical_ton; 
	int offset_horizontal_ton; 
	float tone_map_exposure;
	float tone_map_gamma;
	bool tone_map_toggle;


void keyReleased(int key);

protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();
	void filter();


};