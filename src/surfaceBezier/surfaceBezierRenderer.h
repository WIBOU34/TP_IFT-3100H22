/**
 * \IFT3100H21
 * \file topologieRenderer.h
 * \author Stéphane Boulanger
 * \brief Classe responsable de la surface de Bezier de l'application
 * \version 0.1
 * \date 2022-04-05
 */


#pragma once

#include "ofMain.h"
#include "../renderer.h"
#include "../objectBase3D.h"
#include "ofxGui.h"


class SurfaceBezierRenderer : public RendererBase {
public : 
	ofParameterGroup parameters;
	
	void reset();
	void keyPressed(int key);
	void keyReleased(int key);

protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();

};