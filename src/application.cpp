/**
 * \IFT3100H21
 * \file application.cpp
 * \authors William Boudreault
 *          Stéphane Boulanger
 * \brief Classe principale de l'application
 * \version 0.1
 * \date 2022-03-27
 */

#include "application.h"

// fonction d'initialisation de l'application
void Application::setup() {
	ofLog() << "<app::setup>";
	ofAddListener(ofEvents().mouseMoved, this, &Application::customMouseMoved, OF_EVENT_ORDER_BEFORE_APP - 100);
	ofSetWindowTitle("Travail Pratique");

	ofSetWindowShape(1280, 728);
	ofBackground(31);
	ofSetFrameRate(60);

	imageRenderer.setup("Images");
	dessinRenderer.setup("Dessin");
	objects3DRenderer.setup("Objets 3D");
	//curseurRenderer.setup("Curseur");
	cameraRenderer.setup("Camera - Frustum de vision");
	textureRenderer.setup("Textures");
	courbeBezierRenderer.setup("Courbe parametrique");
	surfaceBezierRenderer.setup("Surface parametrique");
	illuminationRenderer.setup("Illumination");
	illuminationRenderer.camera = cameraRenderer.camera;
	tessellationRenderer.setup("Tessellation");
	ofSetVerticalSync(true);

	// setup crée un nouvel objet en mémoire qui n'est pas supprimé par le gui.clear()

	bHide = true;
	bSelection = false;
	bShowCursor = false;

}

void Application::setupGui(const shared_ptr<ofAppBaseWindow>& window) {
	ofLog() << "<app::GUISetup>";
	mainWindow = window;
	btnExportImgSetup = btnExportImg.setup("Exporter une image");
	btnImportImgSetup = btnImportImg.setup("Importer une image");

	btnExportImg.addListener(this, &Application::exportImage);
	btnImportImg.addListener(this, &Application::importImage);

	gui.setDefaultWidth(300);
	parameters.setName("Menu 'h'");
	gui.setup(parameters);

	gui.add(bSelection.set("Mode Selection 'm'", bSelection));
	gui.add(btnExportImgSetup);
	gui.add(btnImportImgSetup);
	//gui.add(bShowCursor.set("Afficher le curseur 'c'", bShowCursor));
	//gui.add(sliderCurseur.set("Differents curseurs", 1, 0, 4));
	//gui.add(screenSize.setup("screenSize", ofToString(ofGetWindowWidth()) + "x" + ofToString(ofGetWindowHeight())));
	//gui.add(mousePosition.setup("mousePos", "X:" + ofToString(ofGetMouseX()) + " Y:" + ofToString(ofGetMouseY())));
	gui.add(cameraRenderer.parameters);
	gui.add(imageRenderer.parameters);
	gui.add(dessinRenderer.parameters);
	gui.add(&objects3DRenderer.parameters3D);
	gui.add(textureRenderer.parameters);
	gui.add(&illuminationRenderer.parameters);
	gui.add(courbeBezierRenderer.parameters);
	gui.add(surfaceBezierRenderer.parameters);
	gui.add(tessellationRenderer.parameters);

	gui.minimizeAll();
	bHide = false;

	
    // gui pour sélectionner une planète 
	gui_planet.setup("Selection planetes");
	gui_planet.setDefaultWidth(300);
	gui_planet.setPosition(ofGetWindowWidth() - 310, 10);
	gui_planet.add(textureRenderer.parameters_planet);

	// gui pour régler les microfacettes
	gui_specular.setup("Rugosite de la surface");
	gui_specular.setDefaultWidth(300);
	gui_specular.setPosition(ofGetWindowWidth() - 310, 250);
	gui_specular.add(textureRenderer.parameters_specular);
		
	// gui pour sélectionner un point de controle pour la surface de bezier 
	gui_surface_bezier.setup("Selection point controle");
	gui_surface_bezier.setDefaultWidth(300);
	gui_surface_bezier.setPosition(ofGetWindowWidth() - 310, 10);
	gui_surface_bezier.add(surfaceBezierRenderer.parameters_point_controle);
	
	// gui pour la tessellation 
	gui_tessellation.setup("Tessellation");
	gui_tessellation.setDefaultWidth(300);
	gui_tessellation.setPosition(ofGetWindowWidth() - 310, 10);
	gui_tessellation.add(tessellationRenderer.parameters_tessellation);
}

// fonction de mise à jour de la logique de l'application
void Application::update() {
	imageRenderer.update();
	dessinRenderer.update();
	cameraRenderer.update();
	objects3DRenderer.update();
	textureRenderer.update();
	illuminationRenderer.update();
	courbeBezierRenderer.update();
	surfaceBezierRenderer.update();
	tessellationRenderer.update();


	//curseurRenderer.valCurseurSlider = sliderCurseur;
}

// fonction de mise à jour du rendu de la fenêtre d'affichage de l'application
void Application::draw() {
	imageRenderer.draw();
	dessinRenderer.draw();
	textureRenderer.draw();
	courbeBezierRenderer.draw();
	surfaceBezierRenderer.draw();
	tessellationRenderer.draw();
	//illuminationRenderer.draw();
	drawCamera();

	if (!bHide) {
		ofDisableDepthTest();
		gui.draw();
	}

	//curseurRenderer.draw();
	if (textureRenderer.mesh_sphere_toggle || textureRenderer.mesh_square_toggle) {
		gui_planet.draw();
	}

	if (textureRenderer.mesh_square_toggle) gui_specular.draw();
	
	if (tessellationRenderer.tessellation_toggle) gui_tessellation.draw();
	
	if (surfaceBezierRenderer.surface_bezier_toggle) gui_surface_bezier.draw();


}

// fonction pour dessiner les éléments qui seront affectés par la caméra
void Application::drawCamera() {
	cameraRenderer.camera->begin();
	cameraRenderer.draw();
	objects3DRenderer.draw();
	illuminationRenderer.draw();
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
	} else if (key == 'm') {
		bSelection = !bSelection;
	} else if (key == 'r' && illuminationRenderer.drawObjetcs) {
		illuminationRenderer.reset();
	}
	cameraRenderer.keyPressed(key);
	courbeBezierRenderer.keyPressed(key);
	surfaceBezierRenderer.keyPressed(key);
}

//--------------------------------------------------------------
void Application::keyReleased(int key) {
	cameraRenderer.keyReleased(key);
	textureRenderer.keyReleased(key);
	courbeBezierRenderer.keyReleased(key);
	surfaceBezierRenderer.keyReleased(key);
}

//--------------------------------------------------------------
void Application::mouseMoved(int x, int y) {
	// NOTE: utiliser la méthode Application::customMouseMoved
}

void Application::customMouseMoved(ofMouseEventArgs& mouse) {
	//curseurRenderer.setMousePos(mouse.x, mouse.y);
	dessinRenderer.setMousePos(mouse.x, mouse.y);
	imageRenderer.setMousePos(mouse.x, mouse.y);
	mousePosition = "X:" + ofToString(mouse.x) + " Y:" + ofToString(mouse.y);
}

//--------------------------------------------------------------
void Application::mouseDragged(int x, int y, int button) {
	//curseurRenderer.setMousePos(x, y);
	dessinRenderer.setMousePos(x, y);
	imageRenderer.setMousePos(x, y);
	mousePosition = "X:" + ofToString(x) + " Y:" + ofToString(y);
}

//--------------------------------------------------------------
void Application::mousePressed(int x, int y, int button) {
	//curseurRenderer.mousButtonPressed(x, y);
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
	//curseurRenderer.mouseButtonReleased(x, y);
	dessinRenderer.mouseButtonReleased(x, y);
	imageRenderer.mouseButtonReleased(x, y);

	if (!bSelection) {
		dessinRenderer.completeShapeDrawn();
	}
}

//--------------------------------------------------------------
void Application::mouseEntered(int x, int y) {
	//if (!bShowCursor) {
	//	ofHideCursor();
	//}
}

//--------------------------------------------------------------
void Application::mouseExited(int x, int y) {
	//ofShowCursor();
}

//--------------------------------------------------------------
void Application::windowResized(int w, int h) {
	//screenSize = ofToString(w) + "x" + ofToString(h);
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
