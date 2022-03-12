#pragma once

#include "../renderer.h"

#include "ofxGui.h"
#include <vector>
//#include "../application.h"

//#include <glm/vec3.hpp>
//#include <glm/mat4x4.hpp>
//#include <glm/gtc/matrix_transform.hpp>

enum class Camera { front = 0, back = 1, left = 2, right = 3, top = 4, down = 5 };

class CameraRenderer : public RendererBase {
public:
	std::vector<std::shared_ptr<ofAppBaseWindow>> vecWindow = std::vector<std::shared_ptr<ofAppBaseWindow>>(6);
	std::vector<ofCamera*> vecCamera = std::vector<ofCamera*>(6);

	Camera camera_active;

	ofCamera camera_front;
	ofCamera camera_back;
	ofCamera camera_left;
	ofCamera camera_right;
	ofCamera camera_top;
	ofCamera camera_down;

	ofCamera* camera;

	ofQuaternion camera_orientation;

	ofParameterGroup parameters;

	ofxFloatSlider ui_speed_delta;
	ofxFloatSlider ui_camera_fov_h;
	ofxFloatSlider ui_camera_zoom_h;
	ofxFloatSlider ui_camera_n;
	ofxFloatSlider ui_camera_f;

	ofxLabel mouvementsFleches;
	ofxLabel mouvementsFlechesCtrl;
	ofxLabel mouvementsHautBas;
	ofxLabel agencementCamera;
	ofxButton btnReset;

	float camera_n;
	float camera_f;

	float camera_fov;

	float camera_zoom;

	float offset_camera;
	float offset_color;
	float offset_scene;
	float offset_cube;

	float speed_delta;
	float speed_translation;
	float speed_rotation;

	float time_current;
	float time_last;
	float time_elapsed;

	float cube_size;

	int cube_count;

	int index_x;
	int index_y;
	int index_z;

	ofVec3f camera_position;
	ofVec3f camera_target;

	string camera_name;
	string camera_projection;

	bool is_visible_axes;
	bool is_visible_grid;
	bool is_visible_camera;
	bool is_visible_box;
	bool is_visible_locator;
	bool is_visible_text;

	bool is_key_press_left;
	bool is_key_press_up;
	bool is_key_press_right;
	bool is_key_press_down;
	bool is_key_press_ctrl;
	bool is_key_press_shift;
	bool is_key_press_space;

	bool is_camera_move_left;
	bool is_camera_move_right;
	bool is_camera_move_up;
	bool is_camera_move_down;
	bool is_camera_move_forward;
	bool is_camera_move_backward;

	bool is_camera_tilt_up;
	bool is_camera_tilt_down;
	bool is_camera_pan_left;
	bool is_camera_pan_right;
	bool is_camera_roll_left;
	bool is_camera_roll_right;

	ofxToggle is_camera_perspective;
	bool old_is_camera_perspective;

	bool draw_as_checkerboard;

	void keyPressed(int key);
	void keyReleased(int key);
	void reset();
	void setupCamera();
	std::string getCameraName(const Camera& type);
protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();
private:
	void perspectiveChanged();
};
