#include "cameraRenderer.h"

void CameraRenderer::setupRenderer(const std::string& name) {
	//ofEnableDepthTest();
	parameters.clear();
	parameters.setName(name);

	// pamamètres de dessin
	circle_radius = 5.0f;
	stroke_width = 3.0f;
	color_frustum_in = ofColor(0, 255, 0);
	color_frustum_out = ofColor(255, 0, 0);

	// attributs de la caméra
	camera_position = { 0.0f, 0.0f, 0.0f };
	camera_target = { 0.0f, 0.0f, 0.0f };
	camera_resolution_w = 1920.0f;
	camera_resolution_h = 1080.0f;
	camera_n = 50.0f;
	camera_f = 1750.0f;

	camera_fov = 60.0f;
	camera_fov_delta = 16.0f;

	speed_delta = 250.0f;

	offset_cube = 64.0f;
	cube_count = 7;

	camera_zoom = 1.0f / tanf(glm::radians(camera_fov) / 2.0f);

	is_visible_axes = false;
	is_visible_grid = false;
	is_visible_camera = false;
	is_visible_box = true;
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

	is_camera_fov_narrow = false;
	is_camera_fov_wide = false;

	is_camera_perspective = true;

	use_color_fill = true;

	draw_as_checkerboard = false;

	parameters.add(ui_camera_n.setup("camera near", camera_n, 0.0f, ofGetHeight())->getParameter());
	parameters.add(ui_camera_f.setup("camera far", camera_f, 0.0f, ofGetHeight())->getParameter());
	parameters.add(ui_camera_fov_h.setup("camera fov", camera_fov, 0.0f, 180.0f)->getParameter());
	parameters.add(ui_camera_zoom_h.setup("camera zoom", camera_zoom, 0.0f, 10.0f)->getParameter());
	parameters.add(color_stroke.set("color stroke", ofColor(255), ofColor(0, 0), ofColor(255, 255)));
	parameters.add(color_fill.set("color fill", (255.0f, 255.0f, 0.0f, 127.0f), ofColor(0, 0), ofColor(255, 255)));

	reset();

	setupCamera();

	//createNewWindow(Camera::front);
	//createNewWindow(Camera::back);
	//createNewWindow(Camera::left);
	//createNewWindow(Camera::right);
	//createNewWindow(Camera::top);
	//createNewWindow(Camera::down);
	//for (auto& window : vecWindow) {
	//	window->setWindowShouldClose();
	//}
}

void CameraRenderer::keyPressed(int key) {
	switch (key) {
		case OF_KEY_LEFT:
			is_camera_move_left = true;
			break;
		case OF_KEY_UP:
			is_camera_move_up = true;
			break;
		case OF_KEY_RIGHT:
			is_camera_move_right = true;
			break;
		case OF_KEY_DOWN:
			is_camera_move_down = true;
			break;
	}
}

void CameraRenderer::keyReleased(int key) {
	switch (key) {
		case OF_KEY_LEFT:
			is_camera_move_left = false;
			break;
		case OF_KEY_UP:
			is_camera_move_up = false;
			break;
		case OF_KEY_RIGHT:
			is_camera_move_right = false;
			break;
		case OF_KEY_DOWN:
			is_camera_move_down = false;
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
	//// facteur de proportion du frustum par rapport à la fenêtre d'affichage
	//scale_factor = ofGetHeight() * 0.005f;

	//// calculer le ratio d'aspect de la fenêtre d'affichage
	//camera_aspect_ratio = camera_resolution_w / camera_resolution_h;

	//// position de la caméra
	//camera_position.x = ofGetWidth() / 2.0f;
	//camera_position.y = ofGetHeight() / 8.0f;

	//// position vers laquelle la caméra est orientée
	//camera_target.x = camera_position.x;
	//camera_target.y = position_f_c.y;

	//// ajustement manuel du plan d'occlusion avant
	//if (camera_n != ui_camera_n) {
	//	camera_n = ui_camera_n;
	//}

	//// ajustement manuel du plan d'occlusion arrière
	//if (camera_f != ui_camera_f) {
	//	camera_f = ui_camera_f;
	//}

	//// ajustement manuel du champ de vision
	//if (camera_fov != ui_camera_fov_h) {
	//	camera_fov = ui_camera_fov_h;

	//	// recalculer le zoom
	//	camera_zoom = 1.0f / tanf(glm::radians(camera_fov) / 2.0f);
	//	ui_camera_zoom_h = camera_zoom;
	//}

	//// ajustement manuel du zoom
	//if (camera_zoom != ui_camera_zoom_h) {
	//	camera_zoom = ui_camera_zoom_h;

	//	// recalculer le champ de vision
	//	camera_fov = glm::degrees(2.0f * atanf(1.0f / camera_zoom));
	//	ui_camera_fov_h = camera_fov;
	//}

	//// calculer la position de l'intersection entre la ligne de vue et les plans d'occlusion avant et arrière
	//position_n_c.x = camera_position.x;
	//position_f_c.x = camera_position.x;
	//position_n_c.y = camera_position.y + camera_n * scale_factor;
	//position_f_c.y = camera_position.y + camera_f * scale_factor;

	//// calculer la largeur des plans d'occlusion avant et arrière
	//clip_half_width_n = camera_n * tanf(glm::radians(camera_fov / 2.0f));
	//clip_half_width_f = camera_f * tanf(glm::radians(camera_fov / 2.0f));
	//clip_width_n = clip_half_width_n * 2.0f;
	//clip_width_f = clip_half_width_f * 2.0f;

	//// calculer la position de l'intersection entre le champ de vision et les plans d'occlusion avant et arrière
	//position_n_l.x = position_n_c.x - clip_half_width_n * scale_factor;
	//position_n_l.y = position_n_c.y;
	//position_n_r.x = position_n_c.x + clip_half_width_n * scale_factor;
	//position_n_r.y = position_n_c.y;
	//position_f_l.x = position_f_c.x - clip_half_width_f * scale_factor;
	//position_f_l.y = position_f_c.y;
	//position_f_r.x = position_f_c.x + clip_half_width_f * scale_factor;
	//position_f_r.y = position_f_c.y;

	// ligne de contour du frustum
	//camera_frustum_outline.clear();
	//camera_frustum_outline.addVertex(position_n_l.x, position_n_l.y);
	//camera_frustum_outline.addVertex(position_n_r.x, position_n_r.y);
	//camera_frustum_outline.addVertex(position_f_r.x, position_f_r.y);
	//camera_frustum_outline.addVertex(position_f_l.x, position_f_l.y);
	//camera_frustum_outline.close();

	//generate_matrix();
	time_current = ofGetElapsedTimef();
	time_elapsed = time_current - time_last;
	time_last = time_current;

	speed_translation = speed_delta * time_elapsed;
	speed_rotation = speed_translation / 8.0f;

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

	if (is_camera_perspective) {
		if (is_camera_fov_narrow) {
			camera_fov = std::max(camera_fov, 0.0f);
			camera->setFov(camera_fov);
		}

		if (is_camera_fov_wide) {
			camera_fov = std::min(camera_fov, 180.0f);
			camera->setFov(camera_fov);
		}
	}
}

void CameraRenderer::generateDraw() {
}

void CameraRenderer::render() {
	ofEnableDepthTest();
	// activer la caméra
	//camera->begin();

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
	//if (is_visible_box) {
	//	for (index_x = 0; index_x < cube_count; ++index_x) {
	//		for (index_y = 0; index_y < cube_count; ++index_y) {
	//			for (index_z = 0; index_z < cube_count; ++index_z) {
	//				if (index_x % 2 == index_y % 2 == index_z % 2 || !draw_as_checkerboard) {
	//					ofSetColor(
	//						index_x * offset_color,
	//						index_y * offset_color,
	//						index_z * offset_color);

	//					ofDrawBox(
	//						offset_scene + index_x * offset_cube,
	//						offset_scene + index_y * offset_cube,
	//						offset_scene + index_z * offset_cube,
	//						cube_size);
	//				}
	//			}
	//		}
	//	}
	//}

	//camera->end();
	ofDisableDepthTest();
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
	//ofLog() << "<CameraRenderer::setup camera: " << camera_name << " " << camera_projection << ">";

	camera->setPosition(camera_position);
	camera->setOrientation(camera_orientation);

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

//void CameraRenderer::createNewWindow(const Camera& type) {
//	// front = 0, back = 1, left = 2, right = 3, top = 4, down = 5
//
//	auto method = &CameraRenderer::drawFront;
//	int index = -1;
//	switch (type) {
//		case Camera::front:
//			method = &CameraRenderer::drawFront;
//			index = 0;
//			break;
//		case Camera::back:
//			method = &CameraRenderer::drawBack;
//			index = 1;
//			break;
//		case Camera::left:
//			method = &CameraRenderer::drawLeft;
//			index = 2;
//			break;
//		case Camera::right:
//			method = &CameraRenderer::drawRight;
//			index = 3;
//			break;
//		case Camera::top:
//			method = &CameraRenderer::drawTop;
//			index = 4;
//			break;
//		case Camera::down:
//			method = &CameraRenderer::drawDown;
//			index = 5;
//			break;
//		default:
//			index = -1;
//			break;
//	}
//	if (index == -1) {
//		ofLog(ofLogLevel::OF_LOG_ERROR) << "<CameraRenderer::CreateNewWindow: Type invalid>";
//		return;
//	}
//	ofLog() << "<sizeOf window before: " << sizeof(vecWindow.at(index)) << ">";
//	if (vecWindow.at(index) == nullptr || vecWindow.at(index).get()->renderer().get() == nullptr) {
//		ofGLWindowSettings  settings;
//		settings.setGLVersion(2, 1);
//		settings.windowMode = OF_WINDOW;
//		vecWindow.at(index) = ofCreateWindow(settings);
//		vecWindow.at(index)->setWindowTitle(getCameraName(type));
//		ofAddListener(vecWindow.at(index)->events().draw, this, method);
//		ofAddListener(vecWindow.at(index)->events().exit, this, &CameraRenderer::clearWindows, OF_EVENT_ORDER_BEFORE_APP - 100);
//		ofLog() << "<sizeOf window after: " << sizeof(vecWindow.at(index)) << ">";
//	} else {
//		ofLog(ofLogLevel::OF_LOG_WARNING) << "<CameraRenderer::CreateNewWindow: window already exists>";
//	}
//}

//void CameraRenderer::drawFront(ofEventArgs& args) {
//	drawCamera(Camera::front);
//}
//
//void CameraRenderer::drawBack(ofEventArgs& args) {
//	drawCamera(Camera::back);
//}
//
//void CameraRenderer::drawLeft(ofEventArgs& args) {
//	drawCamera(Camera::left);
//}
//
//void CameraRenderer::drawRight(ofEventArgs& args) {
//	drawCamera(Camera::right);
//}
//
//void CameraRenderer::drawTop(ofEventArgs& args) {
//	drawCamera(Camera::top);
//}
//
//void CameraRenderer::drawDown(ofEventArgs& args) {
//	drawCamera(Camera::down);
//}
//
//void CameraRenderer::drawCamera(const Camera& camera) {
//	const Camera old = camera_active;
//	camera_active = camera;
//	setupCamera();
//	update();
//	&Application::drawCamera;
//	//this->parentThis.drawCamera();
//	//this->parent->drawCamera();
//	camera_active = old;
//	setupCamera();
//}

//void CameraRenderer::clearWindows(ofEventArgs& args) {
//	ofGLWindowSettings  settings;
//	settings.setGLVersion(2, 1);
//	settings.windowMode = OF_WINDOW;
//	for (auto& window : vecWindow) {
//		if (window != nullptr && window->renderer() != nullptr) {
//			// Lance une exception d'accès interdit si on ne refait pas un setup
//			// (quand on ferme une fenêtre, les listener d'une autre fenêtre est parfois supprimé)
//			// Je ne sais pas trop pourquoi refaire le setup de chaque fenêtre avant la fermeture règle le problème
//			// Faire cela cause toutefois beacoup "d'erreurs" dans la console disant que ça ne sert à rien
//			window->setup(settings);
//		}
//	}
//}
void CameraRenderer::clearWindows(ofEventArgs& args) {
	ofGLWindowSettings  settings;
	settings.setGLVersion(2, 1);
	settings.windowMode = OF_WINDOW;
	for (auto& window : vecWindow) {
		if (window != nullptr && window->renderer() != nullptr) {
			// Lance une exception d'accès interdit si on ne refait pas un setup
			// (quand on ferme une fenêtre, les listener d'une autre fenêtre est parfois supprimé)
			// Je ne sais pas trop pourquoi refaire le setup de chaque fenêtre avant la fermeture règle le problème
			// Faire cela cause toutefois beacoup "d'erreurs" dans la console disant que ça ne sert à rien
			window->setup(settings);
		}
	}
}

