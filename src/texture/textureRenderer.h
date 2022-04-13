/**
 * \IFT3100H21
 * \file textureRenderer.h
 * \author Stéphane Boulanger
 * \brief Classe responsable des textures de l'application
 * \version 0.1
 * \date 2022-03-27
 */

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

// kernel identité
const std::array<float, 9> convolution_kernel_identity {
  0.0f,  0.0f,  0.0f,
  0.0f,  1.0f,  0.0f,
  0.0f,  0.0f,  0.0f
};


// kernel emboss
const std::array<float, 9> convolution_kernel_emboss {
	-2.0, -1.0f, 0.0f, 
	-1.0f, 1.0f, 1.0f, 
	 0.0f, 1.0f, 2.0f
};

// kernel sharpen
const std::array<float, 9> convolution_kernel_sharpen {
	 0.0, -1.0f, 0.0f,
	-1.0f, 5.0f, -1.0f,
	 0.0f, -1.0f, 0.0f
};

// kernel edge_detect
const std::array<float, 9> convolution_kernel_edge_detect {
	 0.0,  1.0f, 0.0f,
	 1.0f, 0.0f, 1.0f,
	 0.0f, -1.0f, 0.0f
};




class TextureRenderer : public RendererBase {
public:

	// menu gui 
	ofParameterGroup parameters;
	ofxToggle mesh_sphere_toggle; 
	ofxToggle mesh_square_toggle;	
	ofxLabel identite_label;
	ofxLabel emboss_label; 
	ofxLabel sharpen_label; 
	ofxLabel edge_detect_label;
	ofParameter<float> slider_exposure;
	ofParameter<float> slider_gamma;
	ofxToggle tone_map_toggle;
	ofxToggle display;
	
	// menu gui planet 
	ofParameterGroup parameters_planet;
	ofxButton mars_button; 
	ofxButton venus_button;
	ofxButton terre_button;	
	ofxButton jupiter_button;	
	ofxButton saturn_button;	
	ofxButton mercure_button;	
	ofxButton uranus_button;	
	ofxButton neptune_button;	
	ofxButton pluton_button;	

	// sphere de mesh
	ofMesh sphere_mesh;
	ofEasyCam cam_tex;
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

	// normal map 
	ofShader shader_normal_map;
	ofImage heightmap;
	ofPlanePrimitive plane;	
	int plane_width;
	int plane_height;
	int plane_grid_size;
	int plane_columns;
	int plane_rows;
		
	void keyReleased(int key);
	void buttonMarsPicker();
	void buttonVenusPicker();
	void buttonTerrePicker();
	void buttonJupiterPicker();
	void buttonSaturnPicker();
	void buttonMercurePicker();
	void buttonUranusPicker();
	void buttonNeptunePicker();
	void buttonPlutonPicker();
	

protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();
	void filter();
	


};