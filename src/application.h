// IFT3100H21_AlmostEmptyProject/application.h
// Classe principale de l'application.

#pragma once

#include "ofMain.h"
#include "renderer.h"
#include "image/imageRenderer.h"
#include "curseur/curseurRenderer.h"
#include "dessin/dessinRenderer.h"
#include "sphere/sphereRenderer.h"
#include "camera/cameraRenderer.h"
#include "ofxGui.h"

class Application : public ofBaseApp {
public:

	ImageRenderer imageRenderer;
	CurseurRenderer curseurRenderer;
	SphereRenderer sphereRenderer;
	DessinRenderer dessinRenderer;
	CameraRenderer cameraRenderer;

	void setup();
	void update();
	void updateGui();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void customMouseMoved(ofMouseEventArgs& mouse);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void exportImage();
	void importImage();

	bool bHide;

	ofxButton btnExportImg;
	ofxButton* btnExportImgSetup;
	ofxButton btnImportImg;
	ofxButton* btnImportImgSetup;
	ofParameter<bool> bSelection;
	ofParameter<bool> bShowCursor;
	ofParameter<string> screenSize;
	ofParameter<string> mousePosition;
	ofParameterGroup parameters;

	ofxPanel gui;

	// Pour dessiner des lignes
	//ofPolyline polyline;
};
