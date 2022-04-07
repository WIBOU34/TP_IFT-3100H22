/**
 * \IFT3100H21
 * \file surfaceBezierRenderer.h
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


	void reset();
	void keyPressed(int key);
	void keyReleased(int key);
	void buttonSelectionPointControle_1();
	void buttonSelectionPointControle_2();
	void buttonSelectionPointControle_3();
	void buttonSelectionPointControle_4();
	void buttonSelectionPointControle_5();
	void buttonSelectionPointControle_6();
	void buttonSelectionPointControle_7();
	void buttonSelectionPointControle_8();
	void buttonSelectionPointControle_9();
	void buttonSelectionPointControle_10();
	void buttonSelectionPointControle_11();
	void buttonSelectionPointControle_12();
	void buttonSelectionPointControle_13();
	void buttonSelectionPointControle_14();
	void buttonSelectionPointControle_15();
	void buttonSelectionPointControle_16();

	// menu gui 
	ofParameterGroup parameters;
	ofxToggle surface_bezier_toggle;

	ofParameterGroup parameters_point_controle;
	ofxButton point_1; 
	ofxButton point_2; 
	ofxButton point_3; 
	ofxButton point_4; 
	ofxButton point_5; 
	ofxButton point_6; 
	ofxButton point_7; 
	ofxButton point_8; 
	ofxButton point_9; 
	ofxButton point_10; 
	ofxButton point_11; 
	ofxButton point_12; 
	ofxButton point_13; 
	ofxButton point_14; 
	ofxButton point_15; 
	ofxButton point_16; 	
		
	ofMesh mainMesh;
	ofEasyCam mainCam;
	ofImage image;
	ofTexture texture;

	ofPolyline line_renderer_1;
	ofPolyline line_renderer_2;
	ofPolyline line_renderer_3;
	ofPolyline line_renderer_4;
	ofPolyline line_renderer_5;
	ofPolyline line_renderer_6;
	ofPolyline line_renderer_7;
	ofPolyline line_renderer_8;	

	ofVec3f* selected_ctrl_point;	
	ofVec3f ctrl_point1;
	ofVec3f ctrl_point2;
	ofVec3f ctrl_point3;
	ofVec3f ctrl_point4;
	ofVec3f ctrl_point5;
	ofVec3f ctrl_point6;
	ofVec3f ctrl_point7;
	ofVec3f ctrl_point8;
	ofVec3f ctrl_point9;
	ofVec3f ctrl_point10;
	ofVec3f ctrl_point11;
	ofVec3f ctrl_point12;
	ofVec3f ctrl_point13;
	ofVec3f ctrl_point14;
	ofVec3f ctrl_point15;
	ofVec3f ctrl_point16;
	ofVec3f ctrl_point17;
	ofVec3f ctrl_point18;
	ofVec3f ctrl_point19;
	ofVec3f ctrl_point20;
	ofVec3f ctrl_point21;
	ofVec3f ctrl_point22;
	ofVec3f ctrl_point23;
	ofVec3f ctrl_point24;
	ofVec3f ctrl_point25;
	ofVec3f ctrl_point26;
	ofVec3f ctrl_point27;
	ofVec3f ctrl_point28;
	ofVec3f ctrl_point29;
	ofVec3f ctrl_point30;
	ofVec3f ctrl_point31;
	ofVec3f ctrl_point32;
	ofVec3f ctrl_point33;
	ofVec3f ctrl_point34;
	ofVec3f ctrl_point35;
	ofVec3f ctrl_point36;

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
	ofVec3f initial_position11;
	ofVec3f initial_position12;
	ofVec3f initial_position13;
	ofVec3f initial_position14;
	ofVec3f initial_position15;
	ofVec3f initial_position16;
	ofVec3f initial_position17;
	ofVec3f initial_position18;
	ofVec3f initial_position19;
	ofVec3f initial_position20;
	ofVec3f initial_position21;
	ofVec3f initial_position22;
	ofVec3f initial_position23;
	ofVec3f initial_position24;
	ofVec3f initial_position25;
	ofVec3f initial_position26;
	ofVec3f initial_position27;
	ofVec3f initial_position28;
	ofVec3f initial_position29;
	ofVec3f initial_position30;
	ofVec3f initial_position31;
	ofVec3f initial_position32;
	ofVec3f initial_position33;
	ofVec3f initial_position34;
	ofVec3f initial_position35;
	ofVec3f initial_position36;

	ofVec3f position_1;
	ofVec3f position_2;
	ofVec3f position_3;
	ofVec3f position_4;
	ofVec3f position_5;
	ofVec3f position_6;
	ofVec3f position_7;
	ofVec3f position_8;	
	ofVec3f position_9;

	int line_resolution;
	float radius;	
	
	float delta_x;
	float delta_y;
	float motion_speed;	

	int index;
	int width;
	int height;

	float time_current;
	float time_last;
	float time_elapsed;

	bool is_key_press_up;
	bool is_key_press_down;
	bool is_key_press_left;
	bool is_key_press_right;		

	bool draw_wireframe;
	bool control_line;
	bool bez_line;
	bool affiche_image; 

	std::vector<float> vertex_bezier_1;
	std::vector<float> vertex_bezier_2;
	std::vector<float> vertex_bezier_3;
	std::vector<float> vertex_bezier_4;
	std::vector<float> vertex_bezier_5;
	std::vector<float> vertex_bezier_6;
	std::vector<float> vertex_bezier_7;
	std::vector<float> vertex_bezier_8;

protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();

};