// IFT3100H21_AlmostEmptyProject/application.h
// Classe principale de l'application.

#pragma once

#include "ofMain.h"
#include "renderer.h"
#include "image/imageRenderer.h"
#include "ofxGui.h"

class Application : public ofBaseApp {
public:

	ImageRenderer imageRenderer;

	void setup();
	void update();
	void updateGui();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
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
	//bool bSelection;

	ofxButton btnExportImg;
	ofxButton btnImportImg;
	ofParameter<bool> bSelection;
	ofParameter<string> screenSize;
	ofParameterGroup parameters;

	ofxPanel gui;
};
