// IFT3100H21_AlmostEmptyProject/renderer.cpp
// Classe responsable du rendu de l'application.

#include "renderer.h"

//RendererBase::RendererBase() {
//}

// fonction d'initialisation
void RendererBase::setup(const std::string& name) {
	// fréquence de rafraichissement du rendu de la fenêtre d'affichage par seconde
	ofSetFrameRate(60);
	ofLog() << "<RendererBase::setup>";

	mouse_press_x = mouse_press_y = mouse_current_x = mouse_current_y = 0;
	is_mouse_button_pressed = false;
	this->setupRenderer(name);
}

// fonction de mise à jour
void RendererBase::update() {
	this->updateRenderer();
}

// fonction de rendu
void RendererBase::draw() {
	ofDisableDepthTest();
	ofNoFill();
	ofSetColor(ofColor::limit());
	ofSetLineWidth(1);

	if (needsRedraw) {
		generateDraw();
		needsRedraw = false;
	}
	currentFrame = ofGetFrameNum();
	render();
}

void RendererBase::mousButtonPressed(int x, int y) {
	is_mouse_button_pressed = true;
	mouse_press_x = x;
	mouse_press_y = y;
	mouse_press_x_updated = x;
	mouse_press_y_updated = y;
	setMousePos(x, y);
}

void RendererBase::mouseButtonReleased(int x, int y) {
	is_mouse_button_pressed = false;
	mouse_released_x = x;
	mouse_released_y = y;
	setMousePos(x, y);
}

void RendererBase::setMousePos(int x, int y) {
	mouse_current_x = x;
	mouse_current_y = y;
}