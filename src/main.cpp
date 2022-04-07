/**
 * \IFT3100H21
 * \file main.cpp
 * \authors William Boudreault
 *          Stéphane Boulanger
 * \brief classe main de l'application
 * \version 0.1
 * \date 2022-03-27
 */

#include "ofMain.h"
#include "application.h"

// point d'entrée de l'application
int main() {
	// création du contexte de rendu (OpenGL 4.5)
	// OpenGL 3+ ne supporte pas lineWidth avec openFramework malgré ce que dit la doc openGL
	ofGLFWWindowSettings settings;
	settings.setGLVersion(3, 3);
	settings.windowMode = OF_WINDOW;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
	//ofSetupOpenGL(1024, 768, OF_WINDOW);

	shared_ptr<Application> mainApp(new Application());

	ofRunApp(mainWindow, mainApp);
	mainApp->setupGui(mainWindow);
	ofRunMainLoop();
}
