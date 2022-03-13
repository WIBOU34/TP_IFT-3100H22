// IFT3100H21_AlmostEmptyProject/application.cpp
// Classe principale de l'application.

#include "application.h"

// fonction d'initialisation de l'application
void Application::setup() {
	ofLog() << "<app::setup>";
	ofAddListener(ofEvents().mouseMoved, this, &Application::customMouseMoved, OF_EVENT_ORDER_BEFORE_APP - 100);
	ofSetWindowTitle("Travail Pratique");

	imageRenderer.setup("Images");
	sphereRenderer.setup("Sphere");
	sphereRenderer.createSphere();
	dessinRenderer.setup("Dessin");
	objects3DRenderer.setup("Objets 3D");
	cameraRenderer.setup("Camera - Frustum de vision");
	// initialise les éléments et les enlèves pour avoir des objets vide pour la caméra
	createNewWindow(Camera::front);
	createNewWindow(Camera::back);
	createNewWindow(Camera::left);
	createNewWindow(Camera::right);
	createNewWindow(Camera::top);
	createNewWindow(Camera::down);
	for (auto& window : cameraRenderer.vecWindow) {
		window->setWindowShouldClose();
	}
	ofSetVerticalSync(true);

	// setup crée un nouvel objet en mémoire qui n'est pas supprimé par le gui.clear()

	bHide = true;
	bSelection = false;
	bShowCursor = false;
	//ofLog() << "size Object2D<ofImage>: " << sizeof(ObjectBase2D<ofImage>) <<
	//	"\n size Object2D<int>: " << sizeof(ObjectBase2D<int>) <<
	//	"\n size Object2D<float>: " << sizeof(ObjectBase2D<float>) <<
	//	"\n size Object2D<VectorForme>: " << sizeof(ObjectBase2D<VectorForme>) <<
	//	"\n size vectorForme: " << sizeof(VectorForme) <<
	//	"\n size vectorPrimitive: " << sizeof(VectorPrimitive);
	//"\n size vectorPrimitive2: " << sizeof(VectorPrimitive2);

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
	gui.add(screenSize.setup("screenSize", ofToString(ofGetWindowWidth()) + "x" + ofToString(ofGetWindowHeight())));
	gui.add(mousePosition.setup("mousePos", "X:" + ofToString(ofGetMouseX()) + " Y:" + ofToString(ofGetMouseY())));
	gui.add(cameraRenderer.parameters);
	gui.add(imageRenderer.parameters);
	gui.add(dessinRenderer.parameters);
	gui.add(sphereRenderer.sphereParameters);
	gui.add(&objects3DRenderer.parameters3D);

	gui.minimizeAll();
	bHide = false;
}

void Application::updateGui() {
	// =========== Méthode modifiée ===========
	// ajout de "ownedCollection.clear();" dans ofxGuiGroup.cpp ligne 215
	// pour corriger une fuite de mémoire
	//gui.clear();
}

// fonction de mise à jour de la logique de l'application
void Application::update() {
	imageRenderer.update();
	sphereRenderer.update();
	dessinRenderer.update();
	cameraRenderer.update();
}

// fonction de mise à jour du rendu de la fenêtre d'affichage de l'application
void Application::draw() {
	imageRenderer.draw();
	dessinRenderer.draw();
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
	sphereRenderer.draw();
	objects3DRenderer.draw();
	cameraRenderer.camera->end();
}

// fonction appelée juste avant de quitter l'application
void Application::exit() {
	ofLog() << "<app::exit>";

	for (auto& window : cameraRenderer.vecWindow) {
		if (window != nullptr && window->renderer() != nullptr) {
			window->setWindowShouldClose();
		}
	}
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
	switch (key) {
		case '1':
			//this->camera_active = Camera::front;
			//this->setupCamera();
			this->createNewWindow(Camera::front);
			break;
		case '2':
			//this->camera_active = Camera::back;
			//this->setupCamera();
			this->createNewWindow(Camera::back);
			break;
		case '3':
			//this->camera_active = Camera::left;
			//this->setupCamera();
			this->createNewWindow(Camera::left);
			break;
		case '4':
			//this->camera_active = Camera::right;
			//this->setupCamera();
			this->createNewWindow(Camera::right);
			break;
		case '5':
			//this->camera_active = Camera::top;
			//this->setupCamera();
			this->createNewWindow(Camera::top);
			break;
		case '6':
			//this->camera_active = Camera::down;
			//this->setupCamera();
			this->createNewWindow(Camera::down);
			break;
	}
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


// ===============================================================
// Méthodes de caméra nécessitant un accès aux méthodes de draw
// ===============================================================


void Application::createNewWindow(const Camera& type) {
	// front = 0, back = 1, left = 2, right = 3, top = 4, down = 5

	auto method = &Application::drawFront;
	int index = -1;
	switch (type) {
		case Camera::front:
			method = &Application::drawFront;
			index = 0;
			break;
		case Camera::back:
			method = &Application::drawBack;
			index = 1;
			break;
		case Camera::left:
			method = &Application::drawLeft;
			index = 2;
			break;
		case Camera::right:
			method = &Application::drawRight;
			index = 3;
			break;
		case Camera::top:
			method = &Application::drawTop;
			index = 4;
			break;
		case Camera::down:
			method = &Application::drawDown;
			index = 5;
			break;
		default:
			index = -1;
			break;
	}
	if (index == -1) {
		ofLog(ofLogLevel::OF_LOG_ERROR) << "<CameraRenderer::CreateNewWindow: Type invalid>";
		return;
	}
	if (cameraRenderer.vecWindow.at(index) == nullptr || cameraRenderer.vecWindow.at(index).get()->renderer().get() == nullptr) {
		ofGLFWWindowSettings settings;
		settings.setGLVersion(2, 1);
		settings.windowMode = OF_WINDOW;
		settings.shareContextWith = this->mainWindow;
		cameraRenderer.vecWindow.at(index) = ofCreateWindow(settings);
		cameraRenderer.vecWindow.at(index)->setWindowTitle(cameraRenderer.getCameraName(type));
		cameraRenderer.vecWindow.at(index)->setVerticalSync(true);
		cameraRenderer.vecWindow.at(index)->events().setFrameRate(ofGetFrameRate());

		ofAddListener(cameraRenderer.vecWindow.at(index)->events().draw, this, method);
	} else {
		ofLog(ofLogLevel::OF_LOG_WARNING) << "<CameraRenderer::CreateNewWindow: window already exists>";
	}
}

void Application::drawFront(ofEventArgs& args) {
	drawCamera(Camera::front);
}

void Application::drawBack(ofEventArgs& args) {
	drawCamera(Camera::back);
}

void Application::drawLeft(ofEventArgs& args) {
	drawCamera(Camera::left);
}

void Application::drawRight(ofEventArgs& args) {
	drawCamera(Camera::right);
}

void Application::drawTop(ofEventArgs& args) {
	drawCamera(Camera::top);
}

void Application::drawDown(ofEventArgs& args) {
	drawCamera(Camera::down);
}

void Application::drawCamera(const Camera& camera) {
	const Camera old = cameraRenderer.camera_active;
	cameraRenderer.camera_active = camera;
	cameraRenderer.setupCamera();
	this->drawCamera();
	cameraRenderer.camera_active = old;
	cameraRenderer.setupCamera();
}
