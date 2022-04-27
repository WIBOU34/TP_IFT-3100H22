#include "raytracingManager.h"
#include "of3dGraphics.h"

void RaytracingManager::setupRenderer(const std::string& name) {
	parameters.clear();
	parameters.setName(name);

	sceneNames.push_back("test.scn");
	sceneNames.push_back("test_boxes.scn");
	sceneNames.push_back("test_cylinders.scn");
	sceneNames.push_back("test_spheres.scn");

	sceneNames.push_back("all_primitives.scn");
	sceneNames.push_back("cornell.scn");
	sceneNames.push_back("cornell_boxes.scn");
	sceneNames.push_back("cornell_cylinders.scn");
	sceneNames.push_back("cornell_large.scn");
	sceneNames.push_back("simple.scn");
	sceneNames.push_back("caustic.scn");
	sceneNames.push_back("caustic_box.scn");
	sceneNames.push_back("caustic_cylinder.scn");
	sceneNames.push_back("caustic3.scn");
	sceneNames.push_back("caustic3_boxes.scn");
	sceneNames.push_back("caustic3_cylinders.scn");
	sceneNames.push_back("complex.scn");
	sceneNames.push_back("wada.scn");
	sceneNames.push_back("wada2.scn");
	sceneNames.push_back("overlap.scn");
	sceneNames.push_back("forest.scn");


	parameters.add(sceneIndex.set("Scene selectionnee", 0, 0, sceneNames.size() - 1));
	parameters.add(lblSceneName.setup("Nom", "")->getParameter());
	parameters.add(btnGenerateRaytracedImage.setup("Generer la scene")->getParameter());
	parameters.add(rayPerPixel.set("Nombre de passes", 10, 4, 100000));
	parameters.add(depthMax.set("Nombre de bonds des rayons", 6, 6, 1000));
	parameters.add(imageHeight.set("Hauteur de l'image en pixel", ofGetHeight(), 256, 4098));
	parameters.add(imageWidth.set("Largeur de l'image en pixel", ofGetWidth(), 256, 4098));

	btnGenerateRaytracedImage.addListener(this, &RaytracingManager::generateRaytracedImage);
}

void RaytracingManager::updateRenderer() {
	lblSceneName = sceneNames.at(sceneIndex);
}

// ==========================================================

void RaytracingManager::generateDraw() {
}

void RaytracingManager::render() {
}

void RaytracingManager::generateRaytracedImage() {
	raytracerGpu.executeGPURaytracer(imageWidth, imageHeight, rayPerPixel, sceneNames.at(sceneIndex), depthMax);
}

