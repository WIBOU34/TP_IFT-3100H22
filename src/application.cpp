// IFT3100H21_AlmostEmptyProject/application.cpp
// Classe principale de l'application.

#include "application.h"

// fonction d'initialisation de l'application
void Application::setup() {
	ofSetWindowTitle("projet (presque) vide");

	renderer.setup();

	ofLog() << "<app::setup>";
}

// fonction de mise à jour de la logique de l'application
void Application::update() {
	renderer.update();
}

// fonction de mise à jour du rendu de la fenêtre d'affichage de l'application
void Application::draw() {
	renderer.draw();
}

// fonction appelée juste avant de quitter l'application
void Application::exit() {
	ofLog() << "<app::exit>";
}

//--------------------------------------------------------------
void Application::keyPressed(int key) {

}

//--------------------------------------------------------------
void Application::keyReleased(int key) {

}

//--------------------------------------------------------------
void Application::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void Application::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void Application::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void Application::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void Application::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void Application::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void Application::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void Application::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void Application::dragEvent(ofDragInfo dragInfo) {

}
