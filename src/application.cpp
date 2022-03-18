// IFT3100H21_AlmostEmptyProject/application.cpp
// Classe principale de l'application.

#include "application.h"

// fonction d'initialisation de l'application
void Application::setup() {
	ofLog() << "<app::setup>";
	ofAddListener(ofEvents().mouseMoved, this, &Application::customMouseMoved, OF_EVENT_ORDER_BEFORE_APP - 100);
	ofSetWindowTitle("Travail Pratique");

	imageRenderer.setup("Images");
	dessinRenderer.setup("Dessin");
	objects3DRenderer.setup("Objets 3D");
	curseurRenderer.setup("Curseur");
	cameraRenderer.setup("Camera - Frustum de vision");
	textureRenderer.setup();
	ofSetVerticalSync(true);

	// setup crée un nouvel objet en mémoire qui n'est pas supprimé par le gui.clear()

	bHide = true;
	bSelection = false;
	bShowCursor = false;

}

void Application::setupGui(const shared_ptr<ofAppBaseWindow>& window) {
	ofLog() << "<app::GUISetup>";
	mainWindow = window;
	btnExportImgSetup = btnExportImg.setup("Exporter en image 'e'");
	btnImportImgSetup = btnImportImg.setup("Importer une image");

	btnExportImg.addListener(this, &Application::exportImage);
	btnImportImg.addListener(this, &Application::importImage);

	gui.setDefaultWidth(300);
	parameters.setName("Menu 'h'");
	gui.setup(parameters);

	gui.add(bSelection.set("Mode Selection 's'", bSelection));
	gui.add(btnExportImgSetup);
	gui.add(btnImportImgSetup);
	gui.add(bShowCursor.set("Afficher le curseur 'c'", bShowCursor));
	gui.add(sliderCurseur.set("Differents curseurs", 1, 0, 4));
	gui.add(screenSize.setup("screenSize", ofToString(ofGetWindowWidth()) + "x" + ofToString(ofGetWindowHeight())));
	gui.add(mousePosition.setup("mousePos", "X:" + ofToString(ofGetMouseX()) + " Y:" + ofToString(ofGetMouseY())));
	gui.add(cameraRenderer.parameters);
	gui.add(imageRenderer.parameters);
	gui.add(dessinRenderer.parameters);
	gui.add(&objects3DRenderer.parameters3D);

	gui.minimizeAll();
	bHide = false;
}

// fonction de mise à jour de la logique de l'application
void Application::update() {
	imageRenderer.update();
	dessinRenderer.update();
	cameraRenderer.update();
	objects3DRenderer.update();
	textureRenderer.update();

	curseurRenderer.valCurseurSlider = sliderCurseur;
}

// fonction de mise à jour du rendu de la fenêtre d'affichage de l'application
void Application::draw() {
	imageRenderer.draw();
	dessinRenderer.draw();
	textureRenderer.draw();
	drawCamera();

	if (!bHide) {
		ofDisableDepthTest();
		gui.draw();
	}

	curseurRenderer.draw();
}

// fonction pour dessiner les éléments qui seront affectés par la caméra
void Application::drawCamera() {
	cameraRenderer.camera->begin();
	cameraRenderer.draw();
	objects3DRenderer.draw();
	cameraRenderer.camera->end();
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
	cameraRenderer.keyPressed(key);
}

//--------------------------------------------------------------
void Application::keyReleased(int key) {
	cameraRenderer.keyReleased(key);
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
	mousePosition = "X:" + ofToString(x) + " Y:" + ofToString(y);
}

//--------------------------------------------------------------
void Application::mousePressed(int x, int y, int button) {
	curseurRenderer.mousButtonPressed(x, y);
	dessinRenderer.mousButtonPressed(x, y);
	imageRenderer.mousButtonPressed(x, y);

	if (bSelection) {
		imageRenderer.findImage(x, y);
		dessinRenderer.selectPrimitive();
	} else {
		dessinRenderer.beginShapeDraw();
	}
}

//--------------------------------------------------------------
void Application::mouseReleased(int x, int y, int button) {
	curseurRenderer.mouseButtonReleased(x, y);
	dessinRenderer.mouseButtonReleased(x, y);
	imageRenderer.mouseButtonReleased(x, y);

	if (!bSelection) {
		dessinRenderer.completeShapeDrawn();
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
}

void Application::exportImage() {
	imageRenderer.exportImageDialog();
}

//--------------------------------------------------------------
void Application::dragEvent(ofDragInfo dragInfo) {
	ofLog() << "<app::ofDragInfo file[0]: " << dragInfo.files.at(0)
		<< " at : " << dragInfo.position << ">";
	imageRenderer.importImage(dragInfo.files.at(0), dragInfo.position.x, dragInfo.position.y);
}
