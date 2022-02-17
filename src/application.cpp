// IFT3100H21_AlmostEmptyProject/application.cpp
// Classe principale de l'application.

#include "application.h"
#include <sstream>

// fonction d'initialisation de l'application
void Application::setup() {
	ofLog() << "<app::setup>";
	ofAddListener(ofEvents().mouseMoved, this, &Application::customMouseMoved, OF_EVENT_ORDER_BEFORE_APP - 100);
	ofSetWindowTitle("projet (presque) vide");

	imageRenderer.setup("Images");
	sphereRenderer.setup("Sphere");
	sphereRenderer.createSphere();
	dessinRenderer.setup("Dessin");
	ofLog() << "<app::GUISetup>";
	ofSetVerticalSync(true);

	updateGui();
	bHide = false;
	bSelection = false;
	bShowCursor = false;
	ofLog() << "size Object2D<ofImage>: " << sizeof(ObjectBase2D<ofImage>) <<
		"\n size Object2D<int>: " << sizeof(ObjectBase2D<int>) <<
		"\n size Object2D<float>: " << sizeof(ObjectBase2D<float>) <<
		"\n size Object2D<VectorForme>: " << sizeof(ObjectBase2D<VectorForme>) <<
		"\n size vectorForme: " << sizeof(VectorForme) <<
		"\n size vectorPrimitive: " << sizeof(VectorPrimitive);
	//"\n size vectorPrimitive2: " << sizeof(VectorPrimitive2);
}

void Application::updateGui() {
	glm::vec3 position = gui.getPosition();
	gui.clear();
	parameters.clear();
	btnExportImg.removeListener(this, &Application::exportImage);
	btnImportImg.removeListener(this, &Application::importImage);

	gui.setDefaultWidth(300);

	parameters.setName("Menu 'h'");
	gui.setup(parameters);
	gui.setPosition(position);
	gui.add(bSelection.set("Mode Selection 's'", bSelection));
	gui.add(btnExportImg.setup("Exporter en image 'e'"));
	gui.add(btnImportImg.setup("Importer une image"));
	gui.add(bShowCursor.set("Afficher le curseur 'c'", bShowCursor));
	gui.add(screenSize.set("screenSize", ofToString(ofGetWindowWidth()) + "x" + ofToString(ofGetWindowHeight())));
	gui.add(mousePosition.set("mousePos", "X:" + ofToString(ofGetMouseX()) + " Y:" + ofToString(ofGetMouseY())));
	gui.add(imageRenderer.parameters);
	gui.add(dessinRenderer.parameters);
	gui.add(sphereRenderer.sphereParameters);

	btnExportImg.addListener(this, &Application::exportImage);
	btnImportImg.addListener(this, &Application::importImage);

}

// fonction de mise à jour de la logique de l'application
void Application::update() {
	imageRenderer.update();
	sphereRenderer.updateCustom();
}

// fonction de mise à jour du rendu de la fenêtre d'affichage de l'application
void Application::draw() {
	imageRenderer.draw();
	dessinRenderer.draw();

	if (!bHide) {
		gui.draw();
	}

	curseurRenderer.draw();
	sphereRenderer.draw();
}

// fonction appelée juste avant de quitter l'application
void Application::exit() {
	ofLog() << "<app::exit>";
}

//--------------------------------------------------------------
void Application::keyPressed(int key) {
	if (key == 'h') {
		bHide = !bHide;
	} else if (key == 'e') {
		this->exportImage();
	} else if (key == 's') {
		bSelection = !bSelection;
	} else if (key == 'c') {
		bShowCursor = !bShowCursor;
		if (bShowCursor) {
			ofShowCursor();
		} else
			ofHideCursor();
	}
}

//--------------------------------------------------------------
void Application::keyReleased(int key) {

}

//--------------------------------------------------------------
void Application::mouseMoved(int x, int y) {
	// NOTE: utiliser la méthode Application::customMouseMoved
}

void Application::customMouseMoved(ofMouseEventArgs& mouse) {
	curseurRenderer.setMousePos(mouse.x, mouse.y);
	dessinRenderer.setMousePos(mouse.x, mouse.y);
	imageRenderer.setMousePos(mouse.x, mouse.y);
	mousePosition = "X:" + ofToString(mouse.x) + " Y:" + ofToString(mouse.y);
}

//--------------------------------------------------------------
void Application::mouseDragged(int x, int y, int button) {
	curseurRenderer.setMousePos(x, y);
	dessinRenderer.setMousePos(x, y);
	imageRenderer.setMousePos(x, y);

	polyline.addVertex(ofPoint(x, y));
}

//--------------------------------------------------------------
void Application::mousePressed(int x, int y, int button) {
	curseurRenderer.mousButtonPressed(x, y);
	dessinRenderer.mousButtonPressed(x, y);
	imageRenderer.mousButtonPressed(x, y);

	polyline.addVertex(ofPoint(x, y));
}

//--------------------------------------------------------------
void Application::mouseReleased(int x, int y, int button) {
	curseurRenderer.mouseButtonReleased(x, y);
	dessinRenderer.mouseButtonReleased(x, y);
	imageRenderer.mouseButtonReleased(x, y);

	if (bSelection) {
		imageRenderer.findImage(x, y);
		dessinRenderer.selectPrimitive();
		this->updateGui();
	} else {
		dessinRenderer.drawAtMouse();
	}
}

//--------------------------------------------------------------
void Application::mouseEntered(int x, int y) {
	if (!bShowCursor) {
		ofHideCursor();
	}
}

//--------------------------------------------------------------
void Application::mouseExited(int x, int y) {

	ofShowCursor();

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
	//gui.add(imageRenderer.parameters);
	this->updateGui();
}

void Application::exportImage() {
	imageRenderer.exportImageDialog();
}

//--------------------------------------------------------------
void Application::dragEvent(ofDragInfo dragInfo) {
	ofLog() << "<app::ofDragInfo file[0]: " << dragInfo.files.at(0)
		<< " at : " << dragInfo.position << ">";
	imageRenderer.importImage(dragInfo.files.at(0), dragInfo.position.x, dragInfo.position.y);
	this->updateGui();
	//gui.add(imageRenderer.parameters);
}
