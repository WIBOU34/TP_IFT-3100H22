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
	void draw();
	void exit();
	//void generateDraw();

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

	ofParameter<float> radius;
	ofParameter<ofColor> color;
	ofParameter<glm::vec2> center;
	ofParameter<int> circleResolution;
	ofParameter<bool> filled;
	ofxButton btnExportImg;
	ofxButton btnImportImg;
	ofParameter<string> screenSize;

	ofxPanel gui;
};
