#include "objects3DRenderer.h"
#include "of3dGraphics.h"


void Objects3DRenderer::setupRenderer(const std::string& name) {
	parameters3D.clear();
	parameters3D.setName(name);

	importObjButton.addListener(this, &Objects3DRenderer::buttonImportObjPressed);
	bDrawCube.addListener(this, &Objects3DRenderer::buttonCubePressed);
	bDrawSphere.addListener(this, &Objects3DRenderer::buttonSpherePressed);
	bDrawCone.addListener(this, &Objects3DRenderer::buttonConePressed);
	bDrawCylinder.addListener(this, &Objects3DRenderer::buttonCylinderPressed);
	parameters3D.add(importObjButton.setup("Importer un objet 3D"));
	parameters3D.add(bDrawCube.setup("Ajouter un cube"));
	parameters3D.add(bDrawSphere.setup("Ajouter une sphere"));
	parameters3D.add(bDrawCone.setup("Ajouter un cone"));
	parameters3D.add(bDrawCylinder.setup("Ajouter un cylindre"));

	parameters3D.add(fillColor.set("Fill color", ofColor::red));

	selectedObjParams.add(sliderIdObjs.set("Slider pour selectionner un objet", 0, 0, 50));
	selectedObjParams.add(idSelected.set("ID", NO_ITEM_SELECTED));
	selectedObjParams.add(sliderXpos.set("Position X", 0.0f, 0.0f, ofGetWidth()));
	selectedObjParams.add(sliderYpos.set("Position Y", 0.0f, 0.0f, ofGetHeight()));
	selectedObjParams.add(sliderZpos.set("Position Z", 0.0f, -100.0f, 100.0f));
	selectedObjParams.add(sliderRadiusPos.set("Rayon", 0.0f, 0.0f, 1000.0f));
	selectedObjParams.add(sliderWidthPos.set("Largeur", 0.0f, 0.0f, 1000.0f));
	selectedObjParams.add(sliderHeightPos.set("Hauteur", 0.0f, 0.0f, 1000.0f));
	selectedObjParams.add(sliderLengthPos.set("Profondeur", 0.0f));
	selectedObjParams.add(fillColorSelected.set("Fill color selected", fillColor.get()));

	parameters3D.add(&selectedObjParams);

	graphics.enableDraw();
}

void Objects3DRenderer::updateRenderer() {

}

void Objects3DRenderer::buttonImportObjPressed()
{
	ofFileDialogResult dialogResult = ofSystemLoadDialog("Importer un objet");
	if (dialogResult.bSuccess) {
		ofLog() << "<objects3DRenderer::import: importing file>";
		importObj(dialogResult.getPath(), 0, 0);
	}
	else {
		ofLog() << "<objects3DRenderer::import: ABORTED>";
	}
}

void Objects3DRenderer::importObj(const std::string& path, const int& x, const int& y) {
	ofxAssimpModelLoader tempModel;

	if (!tempModel.loadModel(path, false)) {
		ofLogError() << "<objects3DRenderer::import: unable to load object: '" << path << "'>";
		return;
	}
	tempModel.setPosition(0, 0, 0);
	listobjImport.push_back(tempModel);
	//lstImages.push_back(ObjectBase2D<ofImage>(x, y, x + widthToUse, y + heightToUse, tempObj, ((boost::filesystem::path)path).filename().string()));
	//drawImage();
}

// ==========================================================
// Initialisation/création des objets
void Objects3DRenderer::cube(float x, float y, float z, float width, float height, float length) {
	lstObjs.push_back(createCube(x, y, z, width, height, length));

	VectorObj* obj = &lstObjs.back();
	updateCube(*obj, x, y, z, width, height, length);

	VectorObjSettings objSet;
	VectorOutline outlineSet = createOutline(x, y, z, width, height, length);
	objSet.object3D.push_back(obj);
	objSet.renderMode = MeshRenderMode::fill;
	objSet.outline.push_back(outlineSet);
	objTemporaire = ObjectBase3D<VectorObjSettings>(obj->posStart.x, obj->posStart.y, obj->posStart.z, obj->width, obj->height, obj->length, objSet, "3D Object: Cube " + ofToString(lstObjSettings.size()));

	lstObjSettings.push_back(objTemporaire);

}

void Objects3DRenderer::sphere(float x, float y, float z, float r) {
	lstObjs.push_back(createSphere(x, y, z, r));

	VectorObj* obj = &lstObjs.back();
	updateSphere(*obj, x, y, z, r);

	VectorObjSettings objSet;
	VectorOutline outlineSet = createOutline(x, y, z, r);
	objSet.object3D.push_back(obj);
	objSet.renderMode = MeshRenderMode::fill;
	objSet.outline.push_back(outlineSet);
	objTemporaire = ObjectBase3D<VectorObjSettings>(obj->posStart.x, obj->posStart.y, obj->posStart.z, obj->width, obj->height, obj->length, objSet, "3D Object: Sphere " + ofToString(lstObjSettings.size()));
	lstObjSettings.push_back(objTemporaire);
}

void Objects3DRenderer::cone(float x, float y, float z, float r, float heightPositive) {
	const float height = - heightPositive;
	lstObjs.push_back(createCone(x, y, z, r, height));

	VectorObj* obj = &lstObjs.back();
	updateCone(*obj, x, y, z, r, height);

	VectorObjSettings objSet;
	VectorOutline outlineSet = createOutline(x, y, z, r, height);
	objSet.object3D.push_back(obj);
	objSet.renderMode = MeshRenderMode::fill;
	objSet.outline.push_back(outlineSet);
	objTemporaire = ObjectBase3D<VectorObjSettings>(obj->posStart.x, obj->posStart.y, obj->posStart.z, obj->width, obj->height, obj->length, objSet, "3D Object: Cone " + ofToString(lstObjSettings.size()));
	lstObjSettings.push_back(objTemporaire);
}

void Objects3DRenderer::cylinder(float x, float y, float z, float r, float height) {
	lstObjs.push_back(createCylinder(x, y, z, r, height));

	VectorObj* obj = &lstObjs.back();
	updateCylinder(*obj, x, y, z, r, height);

	VectorObjSettings objSet;
	VectorOutline outlineSet = createOutline(x, y, z, r, height);
	objSet.object3D.push_back(obj);
	objSet.renderMode = MeshRenderMode::fill;
	objSet.outline.push_back(outlineSet);
	objTemporaire = ObjectBase3D<VectorObjSettings>(obj->posStart.x, obj->posStart.y, obj->posStart.z, obj->width, obj->height, obj->length, objSet, "3D Object: Cylinder " + ofToString(lstObjSettings.size()));
	lstObjSettings.push_back(objTemporaire);
}
// =======================================================

void Objects3DRenderer::selectObject() {

}

void Objects3DRenderer::generateDraw() {
}

void Objects3DRenderer::render() {
	ofEnableDepthTest();
	for (const ObjectBase3D<VectorObjSettings>& obj : lstObjSettings) {
		this->drawObjects(obj);
	}

	ofDisableDepthTest();
}

// ======================== Méthodes privées ========================

void Objects3DRenderer::drawObjects(const ObjectBase3D<VectorObjSettings>& objSet) {
	for (const VectorObj* obj : objSet.getObject().object3D) {
		switch (obj->type) {
			case VectorObject3DType::sphere:
				//graphics.noFill();
				ofSetColor(obj->fillColor);
				ofSphere(obj->posStart.x, obj->posStart.y, obj->posStart.z, obj->radius);
				break;

			case VectorObject3DType::cube:
				//graphics.fill();
				//graphics.setLineWidth(0);
				ofSetColor(obj->fillColor);
				//graphics.changeColor(primitive->fillColor.r, primitive->fillColor.g, primitive->fillColor.b);
				ofDrawBox(obj->posStart, obj->width, obj->height, obj->length);
				//ofLog() << "<objects3DRenderer::cube drawn>";
				break;

			case VectorObject3DType::cone:
				graphics.fill();
				graphics.setLineWidth(0);
				ofSetColor(obj->fillColor);
				//graphics.changeColor(primitive->fillColor.r, primitive->fillColor.g, primitive->fillColor.b);
				ofDrawCone(obj->posStart, obj->radius, obj->height);
				break;

			case VectorObject3DType::cylinder:
				graphics.fill();
				graphics.setLineWidth(0);
				ofSetColor(obj->fillColor);
				//graphics.changeColor(primitive->fillColor.r, primitive->fillColor.g, primitive->fillColor.b);
				ofDrawCylinder(obj->posStart, obj->radius, obj->height);
				break;

			default:
				break;
		}
	}
}

VectorObj Objects3DRenderer::createNewObj() {
	VectorObj obj;
	obj.fillColor.set(fillColor);
	ofLog() << "<objects3DRenderer::object created>";
	return obj;
}

// ============= Cube =================
VectorObj Objects3DRenderer::createCube(float x, float y, float z, float width, float height, float length) {
	VectorObj obj = createNewObj();
	obj.type = VectorObject3DType::cube;
	this->updateCube(obj, x, y, z, width, height, length);
	ofLog() << "<objects3DRenderer::cube created>";
	return obj;
}

void Objects3DRenderer::updateCube(VectorObj& obj, float x, float y, float z, float width, float height, float length) {
	obj.posStart.set(x, y, z);
	obj.width = width;
	obj.height = height;
	obj.length = length;
}

// ============= Sphere =================
VectorObj Objects3DRenderer::createSphere(float x = 0.0f, float y = 0.0f, float z = 0.0f, float r = 10.0f) {
	VectorObj obj = createNewObj();
	obj.type = VectorObject3DType::sphere;
	this->updateSphere(obj, x, y, z, r);
	return obj;
}

void Objects3DRenderer::updateSphere(VectorObj& obj, float x, float y, float z, float r) {
	obj.posStart.set(x, y, z);
	obj.width = obj.height = obj.length = r;
	obj.radius = r;
}

// ============= Cone =================
VectorObj Objects3DRenderer::createCone(float x, float y, float z, float r, float height) {
	VectorObj obj = createNewObj();
	obj.type = VectorObject3DType::cone;
	this->updateCone(obj, x, y, z, r, height);
	return obj;
}

void Objects3DRenderer::updateCone(VectorObj& obj, float x, float y, float z, float r, float height) {
	obj.posStart.set(x, y, z);
	obj.width = obj.length = r;
	obj.radius = r;
	obj.height = height;
}

// ============= Cube =================
VectorObj Objects3DRenderer::createCylinder(float x, float y, float z, float r, float height) {
	VectorObj obj = createNewObj();
	obj.type = VectorObject3DType::cylinder;
	this->updateCone(obj, x, y, z, r, height);
	return obj;
}

void Objects3DRenderer::updateCylinder(VectorObj& obj, float x, float y, float z, float r, float height) {
	obj.posStart.set(x, y, z);
	obj.width = obj.length = r;
	obj.radius = r;
	obj.height = height;
}


// ================== Outline ==========================

VectorOutline Objects3DRenderer::createOutline(float x, float y, float z, float width, float height, float length) {
	VectorOutline outline;
	this->updateOutline(outline, x, y, z, width, height, length);
	return outline;
}

VectorOutline Objects3DRenderer::createOutline(float x, float y, float z, float r, float length) {
	VectorOutline outline;
	float width = r;
	float height = r;
	this->updateOutline(outline, x, y, z, width, height, length);
	return outline;
}

VectorOutline Objects3DRenderer::createOutline(float x, float y, float z, float r) {
	VectorOutline outline;
	float width = r;
	float height = r;
	float length = r;
	this->updateOutline(outline, x, y, z, width, height, length);
	return outline;
}

void Objects3DRenderer::updateOutline(VectorOutline& outline, float x, float y, float z, float width, float height, float length) {
	outline.posStart.set(x, y, z);
	outline.width = width;
	outline.height = height;
	outline.length = length;
}


// ============== Lorsqu'un bouton est pesé ==================================

// Crée un cube de base en 0,0
void Objects3DRenderer::buttonCubePressed() {
	ofLog() << "<objects3DRenderer::buttonCubePressed>";
	cube(origin.x, origin.y, origin.z, 40.0f, 40.0f, 40.0f);
}

// Crée une sphere de base en 0,0
void Objects3DRenderer::buttonSpherePressed() {
	sphere(origin.x, origin.y, origin.z, 10);
}

// Crée un cone de base en 0,0
void Objects3DRenderer::buttonConePressed() {
	cone(origin.x, origin.y, origin.z, 10, 10);
}

// Crée un cylindre de base en 0,0
void Objects3DRenderer::buttonCylinderPressed() {
	cylinder(origin.x, origin.y, origin.z, 10, 10);
}

