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
	ofxToggle surface_bezier_toggle;
	
	void reset();
	void keyPressed(int key);
	void keyReleased(int key);

	// make a new mesh object
	ofMesh mainMesh;
	ofEasyCam mainCam;
	int width, height;
	bool b_messyMesh, b_perlinMesh, b_drawWireFrame;
	float perlinRange, perlinHeight;

	ofImage image;
	ofTexture imageTexture;


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


	ofPolyline line_renderer;
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

	ofVec3f position;
	ofVec3f position_2;
	ofVec3f position_3;
	ofVec3f position_4;
	ofVec3f position_5;
	ofVec3f position_6;
	ofVec3f position_7;
	ofVec3f position_8;

	ofVec3f tangent1;

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
	int index_3;
	int index_4;
	int index_5;
	int index_6;
	int index_7;
	int index_8;

	float time_current;
	float time_last;
	float time_elapsed;

	bool is_key_press_up;
	bool is_key_press_down;
	bool is_key_press_left;
	bool is_key_press_right;

	bool same_point_4;
	bool same_point_5;

	bool control_line;


	std::vector<float> vertex_bezier_1;
	std::vector<float> vertex_bezier_2;
	std::vector<float> vertex_bezier_3;
	std::vector<float> vertex_bezier_4;
	std::vector<float> vertex_bezier_5;
	std::vector<float> vertex_bezier_6;
	std::vector<float> vertex_bezier_7;
	std::vector<float> vertex_bezier_8;

	ofLight light;


protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();

};