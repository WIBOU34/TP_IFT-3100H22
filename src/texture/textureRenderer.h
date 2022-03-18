// IFT3100H21
// Classe responsable du rendu de l'application.

#pragma once

#include "ofMain.h"
#include "../renderer.h"

enum class ConvolutionKernel
{
	identity,
	emboss, 
	sharpen, 
	edge_detect
};

// kernel identité
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
	ofParameterGroup parameters;

	ofMesh mesh;
	ofEasyCam cam;
	ofImage image;
	ofLight light;

	ofShader shader;

	ConvolutionKernel kernel_type; 
	string kernel_name; 

	ofImage image_destination;
	int image_width;
	int image_height;
	int offset_vertical;
	int offset_horizontal;


protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();
	void filter();
};