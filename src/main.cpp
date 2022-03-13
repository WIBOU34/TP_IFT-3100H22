// IFT3100H21_AlmostEmptyProject/main.cpp
// Exemple d'un projet relativement vide, mais avec une bonne structure de base.
// Un bon point de départ pour commencer un nouveau projet avec une scène vide.
// Animation de la couleur d'arrière-plan par interpolation de niveaux de gris en fonction du numéro du frame courant.
// Trace des données temporelles dans la console à chaque frame.

#include "ofMain.h"
#include "application.h"

// point d'entrée de l'application
int main() {
	// création du contexte de rendu (OpenGL 4.5)
	// OpenGL 3+ ne supporte pas lineWidth avec openFramework malgré ce que dit la doc openGL
	ofGLWindowSettings settings;
	//settings.setGLVersion(4, 5);
	settings.setGLVersion(2, 1);
	settings.windowMode = OF_WINDOW;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
	//ofSetupOpenGL(1024, 768, OF_WINDOW);

	shared_ptr<Application> mainApp(new Application());

	ofRunApp(mainWindow, mainApp);
	mainApp->setupGui(mainWindow);
	ofRunMainLoop();
}
