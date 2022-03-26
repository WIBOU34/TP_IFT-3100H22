#include "raytracingManager.h"
#include "of3dGraphics.h"
#include <thread>

void RaytracingManager::setupRenderer(const std::string& name) {
	parameters.clear();
	parameters.setName(name);

	warningNbrThreads.setFillColor(ofColor::yellow);
	warningNbrThreads.setTextColor(ofColor::black);
	warningNbrThreads.setBackgroundColor(ofColor::yellow);
	parameters.add(btnGenerateRaytracedImage.setup("Generer une image avec lancers de rayons")->getParameter());
	parameters.add(warningNbrThreads.setup("WARNING", "Aucune protection surchauffe")->getParameter());
	parameters.add(nbThreads.set("Nombre de threads", std::thread::hardware_concurrency() / 2, 1, std::thread::hardware_concurrency()));
	parameters.add(rayPerPixel.set("Nombre de rayons par pixel", 10, 4, 500));
	parameters.add(imageHeight.set("Hauteur de l'image en pixel", ofGetHeight(), 256, 4098));
	parameters.add(imageWidth.set("Largeur de l'image en pixel", ofGetWidth(), 256, 4098));

	btnGenerateRaytracedImage.addListener(this, &RaytracingManager::generateRaytracedImage);
}

void RaytracingManager::updateRenderer() {
}

// ==========================================================
// Conversion des objets
//void RaytracingManager::cube(float x, float y, float z, float width, float height, float length) {
//	lstObjs.push_back(createCube(x, y, z, width, height, length));
//
//	VectorObj* obj = &lstObjs.back();
//	updateCube(*obj, x, y, z, width, height, length);
//
//	VectorObjSettings objSet;
//	objSet.object3D = obj;
//	objSet.renderMode = MeshRenderMode::wireframe;
//
//	lstObjSettings.push_back(ObjectBase3D<VectorObjSettings>(obj->posStart.x, obj->posStart.y, obj->posStart.z, obj->width, obj->height, obj->length, objSet, "3D Object: Cube " + ofToString(lstObjSettings.size())));
//
//}
//
//void RaytracingManager::sphere(float x, float y, float z, float r) {
//	lstObjs.push_back(createSphere(x, y, z, r));
//
//	VectorObj* obj = &lstObjs.back();
//	updateSphere(*obj, x, y, z, r);
//
//	VectorObjSettings objSet;
//	objSet.object3D = obj;
//	objSet.renderMode = MeshRenderMode::wireframe;
//	lstObjSettings.push_back(ObjectBase3D<VectorObjSettings>(obj->posStart.x, obj->posStart.y, obj->posStart.z, obj->width, obj->height, obj->length, objSet, "3D Object: Sphere " + ofToString(lstObjSettings.size())));
//}
//
//void RaytracingManager::cone(float x, float y, float z, float r, float height) {
//	lstObjs.push_back(createCone(x, y, z, r, height));
//
//	VectorObj* obj = &lstObjs.back();
//	updateCone(*obj, x, y, z, r, height);
//
//	VectorObjSettings objSet;
//	objSet.object3D = obj;
//	objSet.renderMode = MeshRenderMode::wireframe;
//	lstObjSettings.push_back(ObjectBase3D<VectorObjSettings>(obj->posStart.x, obj->posStart.y, obj->posStart.z, obj->width, obj->height, obj->length, objSet, "3D Object: Cone " + ofToString(lstObjSettings.size())));
//}
//
//void RaytracingManager::cylinder(float x, float y, float z, float r, float height) {
//	lstObjs.push_back(createCylinder(x, y, z, r, height));
//
//	VectorObj* obj = &lstObjs.back();
//	updateCylinder(*obj, x, y, z, r, height);
//
//	VectorObjSettings objSet;
//	objSet.object3D = obj;
//	objSet.renderMode = MeshRenderMode::wireframe;
//	lstObjSettings.push_back(ObjectBase3D<VectorObjSettings>(obj->posStart.x, obj->posStart.y, obj->posStart.z, obj->width, obj->height, obj->length, objSet, "3D Object: Cylinder " + ofToString(lstObjSettings.size())));
//}

void RaytracingManager::generateDraw() {
}

void RaytracingManager::render() {
}

void RaytracingManager::generateRaytracedImage() {
	//const int imageWidth = 256;
	//const int imageHeight = 256;
	//const int rayPerPixel = 10;
	//const int nbThreads = 10;
	raytracer.raytracerExecute(imageWidth, imageHeight, rayPerPixel, nbThreads);
	//raytracer.raytracerExecute(imageWidth, imageHeight, rayPerPixel);
}

