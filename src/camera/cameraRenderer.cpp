#include "cameraRenderer.h"

void CameraRenderer::setupRenderer(const std::string& name) {
	parameters.clear();
	parameters.setName(name);

	vecCamera.at(0) = &camera_front;
	vecCamera.at(1) = &camera_back;
	vecCamera.at(2) = &camera_left;
	vecCamera.at(3) = &camera_right;
	vecCamera.at(4) = &camera_top;
	vecCamera.at(5) = &camera_down;


	// attributs de la caméra
	camera_position = { 0.0f, 0.0f, 0.0f };
	camera_target = { 0.0f, 0.0f, 0.0f };
	camera_n = 50.0f;
	camera_f = 1750.0f;

	camera_fov = 60.0f;

	speed_delta = 250.0f;

	offset_cube = 64.0f;
	cube_count = 7;

	camera_zoom = 1.0f / tanf(glm::radians(camera_fov) / 2.0f);

	//debug
	is_visible_axes = false;
	is_visible_grid = false;
	is_visible_camera = false;
	is_visible_box = false;
	is_visible_locator = false;
	is_visible_text = false;

	is_camera_move_left = false;
	is_camera_move_right = false;
	is_camera_move_up = false;
	is_camera_move_down = false;
	is_camera_move_forward = false;
	is_camera_move_backward = false;

	is_camera_tilt_up = false;
	is_camera_tilt_down = false;
	is_camera_pan_left = false;
	is_camera_pan_right = false;
	is_camera_roll_left = false;
	is_camera_roll_right = false;

	is_camera_perspective = true;
	old_is_camera_perspective = is_camera_perspective;

	draw_as_checkerboard = false;

	parameters.add(mouvementsFleches.setup("Fleches", "deplacement Z")->getParameter());
	parameters.add(mouvementsFlechesCtrl.setup("Fleches + ctrl", "Changement angles")->getParameter());
	parameters.add(mouvementsHautBas.setup("Shift - Space", "Haut - Bas")->getParameter());
	parameters.add(agencementCamera.setup("Agencement camera", "1-6")->getParameter());
	parameters.add(btnReset.setup("Reset la camera (r)")->getParameter());

	parameters.add(is_camera_perspective.setup("Mode perspective", is_camera_perspective)->getParameter());
	parameters.add(ui_speed_delta.setup("Vitesse deplacement", speed_delta, 0.0f, 500.0f)->getParameter());
	parameters.add(ui_camera_n.setup("camera near", camera_n, 0.0f, 2000.0f)->getParameter());
	parameters.add(ui_camera_f.setup("camera far", camera_f, 0.0f, 2000.0f)->getParameter());
	parameters.add(ui_camera_fov_h.setup("camera fov", camera_fov, 0.0f, 180.0f)->getParameter());
	parameters.add(ui_camera_zoom_h.setup("camera zoom", camera_zoom, 0.0f, 10.0f)->getParameter());

	btnReset.addListener(this, &CameraRenderer::reset);

	reset();

	setupCamera();
}

void CameraRenderer::keyPressed(int key) {
	switch (key) {
		case OF_KEY_LEFT:
			is_key_press_left = true;
			break;
		case OF_KEY_UP:
			is_key_press_up = true;
			break;
		case OF_KEY_RIGHT:
			is_key_press_right = true;
			break;
		case OF_KEY_DOWN:
			is_key_press_down = true;
			break;
		case OF_KEY_CONTROL:
			is_key_press_ctrl = true;
			break;
		case OF_KEY_SHIFT:
			is_key_press_shift = true;
			break;
		case ' ':
			is_key_press_space = true;
			break;
	}
}

void CameraRenderer::keyReleased(int key) {
	switch (key) {
		case OF_KEY_LEFT:
			is_key_press_left = false;
			break;
		case OF_KEY_UP:
			is_key_press_up = false;
			break;
		case OF_KEY_RIGHT:
			is_key_press_right = false;
			break;
		case OF_KEY_DOWN:
			is_key_press_down = false;
			break;
		case OF_KEY_CONTROL:
			is_key_press_ctrl = false;
			break;
		case OF_KEY_SHIFT:
			is_key_press_shift = false;
			break;
		case ' ':
			is_key_press_space = false;
			break;
		case 'r':
			reset();
			break;
			// Creation des fenetres dans application.cpp
	}
}

void CameraRenderer::reset() {
	ofLog() << "<CameraRenderer::reset>";
	// initialisation des variables
	cube_size = offset_cube / 2.0f;
	offset_scene = cube_count * offset_cube / 2.0f * -1.0f + cube_size;
	offset_color = 255.0f / cube_count;
	offset_camera = offset_scene * 3.5f * -1.0f;

	// position initiale de chaque caméra

	camera_front.setPosition(0, 0, -offset_camera);
	camera_back.setPosition(0, 0, offset_camera);
	camera_left.setPosition(-offset_camera, 0, 0);
	camera_right.setPosition(offset_camera, 0, 0);
	camera_top.setPosition(0, offset_camera, 0);
	camera_down.setPosition(0, -offset_camera, 0);

	camera_target.x = 0.0f;
	camera_target.y = 0.0f;
	camera_target.z = 0.0f;

	// orientation de chaque caméra
	camera_front.lookAt(camera_target);
	camera_back.lookAt(camera_target);
	camera_left.lookAt(camera_target);
	camera_right.lookAt(camera_target);
	camera_top.lookAt(camera_target, ofVec3f(1, 0, 0));
	camera_down.lookAt(camera_target, ofVec3f(1, 0, 0));

	// caméra par défaut
	camera_active = Camera::front;
}

void CameraRenderer::updateRenderer() {
	if (!this->needsRedraw
		&& (is_key_press_left || is_key_press_up || is_key_press_right || is_key_press_down
			|| is_key_press_ctrl || is_key_press_shift || is_key_press_space)) {
		this->needsRedraw = true;
	}
	// fleches seulement
	is_camera_move_left = !is_key_press_ctrl && is_key_press_left;
	is_camera_move_right = !is_key_press_ctrl && is_key_press_right;
	is_camera_move_forward = !is_key_press_ctrl && is_key_press_up;
	is_camera_move_backward = !is_key_press_ctrl && is_key_press_down;

	// Shift
	is_camera_move_down = is_key_press_shift;
	// Space
	is_camera_move_up = is_key_press_space;

	// ctrl + fleches
	is_camera_pan_left = is_key_press_ctrl && is_key_press_left;
	is_camera_pan_right = is_key_press_ctrl && is_key_press_right;
	is_camera_tilt_up = is_key_press_ctrl && is_key_press_up;
	is_camera_tilt_down = is_key_press_ctrl && is_key_press_down;

	// ajustement manuel du plan d'occlusion avant
	if (camera_n != ui_camera_n) {
		camera_n = ui_camera_n;
		old_is_camera_perspective = !is_camera_perspective;
	}

	// ajustement manuel du plan d'occlusion arrière
	if (camera_f != ui_camera_f) {
		camera_f = ui_camera_f;
		old_is_camera_perspective = !is_camera_perspective;
	}

	// ajustement manuel du champ de vision
	if (camera_fov != ui_camera_fov_h) {
		camera_fov = ui_camera_fov_h;

		// recalculer le zoom
		camera_zoom = 1.0f / tanf(glm::radians(camera_fov) / 2.0f);
		ui_camera_zoom_h = camera_zoom;
		old_is_camera_perspective = !is_camera_perspective;
	}

	// ajustement manuel du zoom
	if (camera_zoom != ui_camera_zoom_h) {
		camera_zoom = ui_camera_zoom_h;

		// recalculer le champ de vision
		camera_fov = glm::degrees(2.0f * atanf(1.0f / camera_zoom));
		ui_camera_fov_h = camera_fov;
		old_is_camera_perspective = !is_camera_perspective;
	}

	if (old_is_camera_perspective != is_camera_perspective) {
		old_is_camera_perspective = is_camera_perspective;
		perspectiveChanged();
	}

	speed_delta = ui_speed_delta;

	time_current = ofGetElapsedTimef();
	time_elapsed = time_current - time_last;
	time_last = time_current;

	speed_translation = speed_delta * time_elapsed;
	speed_rotation = speed_translation / 8.0f;

	for (ofCamera* camera : vecCamera) {
		if (is_camera_move_left)
			camera->truck(-speed_translation);
		if (is_camera_move_right)
			camera->truck(speed_translation);

		if (is_camera_move_up)
			camera->boom(speed_translation);
		if (is_camera_move_down)
			camera->boom(-speed_translation);

		if (is_camera_move_forward)
			camera->dolly(-speed_translation);
		if (is_camera_move_backward)
			camera->dolly(speed_translation);

		if (is_camera_tilt_up)
			camera->tiltDeg(-speed_rotation);
		if (is_camera_tilt_down)
			camera->tiltDeg(speed_rotation);

		if (is_camera_pan_left)
			camera->panDeg(speed_rotation);
		if (is_camera_pan_right)
			camera->panDeg(-speed_rotation);

		if (is_camera_roll_left)
			camera->rollDeg(-speed_rotation);
		if (is_camera_roll_right)
			camera->rollDeg(speed_rotation);
	}
}

void CameraRenderer::generateDraw() {
	//time_current = ofGetElapsedTimef();
	//time_elapsed = time_current - time_last;
	//time_last = time_current;

	//if (speed_translation != 0 && time_elapsed > 0.0001) {
	//	speed_translation = speed_delta * time_elapsed;
	//	speed_rotation = speed_translation / 8.0f;
	//}
	
}

void CameraRenderer::render() {
	ofEnableDepthTest();

	// dessiner une grille
	if (is_visible_grid)
		ofDrawGrid(64);

	// dessiner un gizmo pour les axes de la scène
	if (is_visible_axes)
		ofDrawRotationAxes(64);

	// dessiner un gizmo pour toutes les caméras sauf celle qui est active
	if (is_visible_camera) {
		if (camera_active != Camera::front)
			camera_front.draw();
		if (camera_active != Camera::back)
			camera_back.draw();
		if (camera_active != Camera::left)
			camera_left.draw();
		if (camera_active != Camera::right)
			camera_right.draw();
		if (camera_active != Camera::top)
			camera_top.draw();
		if (camera_active != Camera::down)
			camera_down.draw();
	}

	// dessiner une zone de texte avec le nom des caméras
	if (is_visible_text) {
		if (camera_active != Camera::front)
			ofDrawBitmapString(" camera front", camera_front.getPosition());
		if (camera_active != Camera::back)
			ofDrawBitmapString(" camera back", camera_back.getPosition());
		if (camera_active != Camera::left)
			ofDrawBitmapString(" camera left", camera_left.getPosition());
		if (camera_active != Camera::right)
			ofDrawBitmapString(" camera right", camera_right.getPosition());
		if (camera_active != Camera::top)
			ofDrawBitmapString(" camera top", camera_top.getPosition());
		if (camera_active != Camera::down)
			ofDrawBitmapString(" camera down", camera_down.getPosition());
	}

	// dessiner le contenu de la scène
	if (is_visible_box) {
		for (index_x = 0; index_x < cube_count; ++index_x) {
			for (index_y = 0; index_y < cube_count; ++index_y) {
				for (index_z = 0; index_z < cube_count; ++index_z) {
					if (index_x % 2 == index_y % 2 == index_z % 2 || !draw_as_checkerboard) {
						ofSetColor(
							index_x * offset_color,
							index_y * offset_color,
							index_z * offset_color);

						ofDrawBox(
							offset_scene + index_x * offset_cube,
							offset_scene + index_y * offset_cube,
							offset_scene + index_z * offset_cube,
							cube_size);
					}
				}
			}
		}
	}
	ofDisableDepthTest();
}

void CameraRenderer::perspectiveChanged() {
	for (auto& camera : vecCamera) {
		camera_position = camera->getPosition();
		camera_orientation = camera->getOrientationQuat();

		// mode de projection de la caméra
		if (is_camera_perspective) {
			camera->disableOrtho();
			camera->setupPerspective(false, camera_fov, camera_n, camera_f, ofVec2f(0, 0));
			camera_projection = "perspective";
		} else {
			camera->enableOrtho();
			camera_projection = "orthogonale";
		}

		camera->setPosition(camera_position);
		camera->setOrientation(camera_orientation);
	}
}

// fonction de configuration de la caméra active
void CameraRenderer::setupCamera() {
	switch (camera_active) {
		case Camera::front:
			camera = &camera_front;
			camera_name = "avant";
			break;

		case Camera::back:
			camera = &camera_back;
			camera_name = "arriere";
			break;

		case Camera::left:
			camera = &camera_left;
			camera_name = "gauche";
			break;

		case Camera::right:
			camera = &camera_right;
			camera_name = "droite";
			break;

		case Camera::top:
			camera = &camera_top;
			camera_name = "haut";
			break;

		case Camera::down:
			camera = &camera_down;
			camera_name = "bas";
			break;

		default:
			break;
	}
}

std::string CameraRenderer::getCameraName(const Camera& type) {
	std::string returnValue = "";
	switch (type) {
		case Camera::front:
			returnValue = "avant";
			break;
		case Camera::back:
			returnValue = "arriere";
			break;
		case Camera::left:
			returnValue = "gauche";
			break;
		case Camera::right:
			returnValue = "droite";
			break;
		case Camera::top:
			returnValue = "haut";
			break;
		case Camera::down:
			returnValue = "bas";
			break;

		default:
			break;
	}
	return returnValue;
}
