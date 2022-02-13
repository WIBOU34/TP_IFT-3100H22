// IFT3100H21_AlmostEmptyProject/renderer.cpp
// Classe responsable du rendu de l'application.

#include "renderer.h"

//RendererBase::RendererBase() {
//}

// fonction d'initialisation
void RendererBase::setup() {
	// fréquence de rafraichissement du rendu de la fenêtre d'affichage par seconde
	ofSetFrameRate(60);

	mouse_press_x = mouse_press_y = mouse_current_x = mouse_current_y = 0;
	is_mouse_button_pressed = false;
}

// fonction de mise à jour
void RendererBase::update() {
	// temps courant en secondes
	//time_current = ofGetElapsedTimef();

	//// le temps écoulé depuis le dernier frame en secondes
	//time_elapsed = time_current - time_last;

	//// le temps courant sera le temps précédent du prochain frame
	//time_last = time_current;

	//// trace des données temporelles dans la console
	//ofLog() << std::setprecision(2) << "<frame: " << ofGetFrameNum() << " time current: " << time_current << " time elapsed: " << time_elapsed << " fps: " << 1.0 / time_elapsed << ">";
}

// fonction de rendu
void RendererBase::draw() {
	// couleur d'arrière-plan en niveau de gris en fonction du numéro de frame courant.
	//ofClear(ofGetFrameNum() % 255);
	// afficher l'image sur toute la surface de la fenêtre d'affichage
	//image.draw(0, 0, image.getWidth(), image.getHeight());

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


