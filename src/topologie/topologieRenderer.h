/**
 * \IFT3100H21
 * \file topologieRenderer.h
 * \author Stéphane Boulanger
 * \brief Classe responsable de la topologie de l'application
 * \version 0.1
 * \date 2022-03-27
 */


#pragma once

#include "ofMain.h"
#include "../renderer.h"
#include "../objectBase3D.h"
#include "ofxGui.h"


class TopologieRenderer : public RendererBase {
public : 

	ofParameterGroup parameters;
	ofxToggle bezier_toggle;
	
public:

	// première fonction d'évaluation d'une courbe de Bézier cubique 
	inline void bezier_cubic(
		float t,
		float p1x, float p1y, float p1z,
		float p2x, float p2y, float p2z,
		float p3x, float p3y, float p3z,
		float p4x, float p4y, float p4z,
		float& x, float& y, float& z)
	{
		float u = 1 - t;
		float uu = u * u;
		float uuu = uu * u;
		float tt = t * t;
		float ttt = tt * t;

		x = uuu * p1x + 3 * uu * t * p2x + 3 * u * tt * p3x + ttt * p4x;
		y = uuu * p1y + 3 * uu * t * p2y + 3 * u * tt * p3y + ttt * p4y;
		z = uuu * p1z + 3 * uu * t * p2z + 3 * u * tt * p3z + ttt * p4z;
	}

	// deuxième fonction d'évaluation d'une courbe de Bézier cubique 
	inline void bezier_cubic_2(
		float t,
		float p5x, float p5y, float p5z,
		float p6x, float p6y, float p6z,
		float p7x, float p7y, float p7z,
		float p8x, float p8y, float p8z,
		float& x, float& y, float& z)
	{
		float u = 1 - t;
		float uu = u * u;
		float uuu = uu * u;
		float tt = t * t;
		float ttt = tt * t;

		x = uuu * p5x + 3 * uu * t * p6x + 3 * u * tt * p7x + ttt * p8x;
		y = uuu * p5y + 3 * uu * t * p6y + 3 * u * tt * p7y + ttt * p8y;
		z = uuu * p5z + 3 * uu * t * p6z + 3 * u * tt * p7z + ttt * p8z;
	}




	string  curve_name;

	ofPolyline line_renderer;
	ofPolyline line_renderer_2;

	ofVec3f* selected_ctrl_point;
	

	ofVec3f ctrl_point1;
	ofVec3f ctrl_point2;
	ofVec3f ctrl_point3;
	ofVec3f ctrl_point4;
	ofVec3f ctrl_point5;
	ofVec3f ctrl_point6;
	ofVec3f ctrl_point7;
	ofVec3f ctrl_point8;


	ofVec3f initial_position1;
	ofVec3f initial_position2;
	ofVec3f initial_position3;
	ofVec3f initial_position4;
	ofVec3f initial_position5;

	ofVec3f initial_position6;
	ofVec3f initial_position7;
	ofVec3f initial_position8;
	ofVec3f initial_position9;
	ofVec3f initial_position10;

	ofVec3f position;
	ofVec3f position_2;

	ofVec3f tangent1;
	ofVec3f tangent2;

	float line_width_outline;
	float line_width_curve;

	float radius;
	float scale;
	float offset;

	float delta_x;
	float delta_y;

	float motion_speed;

	int framebuffer_width;
	int framebuffer_height;

	int line_resolution;

	int index;
	int index_2;

	float time_current;
	float time_last;
	float time_elapsed;

	bool is_key_press_up_topo;
	bool is_key_press_down_topo;
	bool is_key_press_left_topo;
	bool is_key_press_right_topo;

	bool same_point_4;
	bool same_point_5;

	bool hide_control_line;
	bool noeud;

	ofEasyCam cam;

	void reset();
	void keyPressed(int key);
	void keyReleased(int key);

protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();

};