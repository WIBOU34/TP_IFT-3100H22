// IFT3100H21_AlmostEmptyProject/application.cpp
// Classe principale de l'application.

#include "application.h"

// fonction d'initialisation de l'application
void Application::setup() {
	ofLog() << "<app::setup>";
	ofSetWindowTitle("projet (presque) vide");

	imageRenderer.setup();
	ofLog() << "<app::GUISetup>";
	ofSetVerticalSync(true);

	// we add this listener before setting up so the initial circle resolution is correct
	btnExportImg.addListener(this, &Application::exportImage);
	btnImportImg.addListener(this, &Application::importImage);

	gui.setup("Menu"); // most of the time you don't need a name but don't forget to call setup
	gui.add(filled.set("bFill", true));
	gui.add(btnExportImg.setup("Exporter en image"));
	gui.add(btnImportImg.setup("Importer une image"));
	gui.add(screenSize.set("screenSize", ""));

	bHide = false;
}

// fonction de mise à jour de la logique de l'application
void Application::update() {
	imageRenderer.update();
}

// fonction de mise à jour du rendu de la fenêtre d'affichage de l'application
void Application::draw() {
	imageRenderer.draw();

	if (!bHide) {
		gui.draw();
	}
}

// fonction appelée juste avant de quitter l'application
void Application::exit() {
	ofLog() << "<app::exit>";
}

//--------------------------------------------------------------
void Application::keyPressed(int key) {
	if (key == 'h') {
		bHide = !bHide;
	}
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
	screenSize = ofToString(w) + "x" + ofToString(h);
}

//--------------------------------------------------------------
void Application::gotMessage(ofMessage msg) {

}

void Application::importImage() {
	imageRenderer.importImageDialog();
}

void Application::exportImage() {
	bool original = bHide;
	bHide = true;
	draw();
	imageRenderer.exportImageDialog();
	bHide = original;
}

//--------------------------------------------------------------
void Application::dragEvent(ofDragInfo dragInfo) {
	ofLog() << "<app::ofDragInfo file[0]: " << dragInfo.files.at(0)
		<< " at : " << dragInfo.position << ">";
	imageRenderer.importImage(dragInfo.files.at(0));
}
