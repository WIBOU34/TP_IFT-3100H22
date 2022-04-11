/**
 * \IFT3100H21
 * \file surfaceBezierRenderer.h
 * \author Stéphane Boulanger
 * \brief Classe responsable de la surface de la tessellation de l'application
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
	

protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();

};