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
	btnDeleteSelected.addListener(this, &Objects3DRenderer::deleteSelected);
	parameters3D.add(importObjButton.setup("Importer un objet 3D"));
	parameters3D.add(bDrawCube.setup("Ajouter un cube"));
	parameters3D.add(bDrawSphere.setup("Ajouter une sphere"));
	parameters3D.add(bDrawCone.setup("Ajouter un cone"));
	parameters3D.add(bDrawCylinder.setup("Ajouter un cylindre"));
	parameters3D.add(boundingBox.set("Afficher boite delimitation", false));
	parameters3D.add(btnDeleteSelected.setup("Supprimer objet selectionne"));

	parameters3D.add(fillColor.set("Fill color", ofColor::red));

	selectedObjParams.setup("Objet selectionne");
	selectedObjParams.add(sliderIdObjs.set("Selectionner une forme 3D", 0, 0, 50));
	selectedObjParams.add(sliderIdModelObjs.set("Selectionner un model 3D", 0, 0, 50));
	selectedObjParams.add(idSelected.setup("ID", NO_ITEM_SELECTED));
	selectedObjParams.add(sliderXpos.set("Position X", 0.0f, -5000, 5000));
	selectedObjParams.add(sliderYpos.set("Position Y", 0.0f, -5000, 5000));
	selectedObjParams.add(sliderZpos.set("Position Z", 0.0f, -5000, 5000));
	selectedObjParams.add(sliderRadiusPos.set("Rayon", 0.0f, 0.0f, 1000.0f));
	selectedObjParams.add(sliderWidthPos.set("Largeur", 0.0f, 0.0f, 1000.0f));
	selectedObjParams.add(sliderHeightPos.set("Hauteur", 0.0f, 0.0f, 1000.0f));
	selectedObjParams.add(sliderLengthPos.set("Profondeur", 0.0f, 0.0f, 1000.0f));
	selectedObjParams.add(fillColorSelected.set("Fill color selected", fillColor.get()));

	parameters3D.add(&selectedObjParams);
}

void Objects3DRenderer::updateRenderer() {
	// Formes 3D
	if (selectedObj != nullptr && sliderIdObjs <= 0) {
		selectedObj = nullptr;
		oldSelectedId = 0;

		idSelected = NO_ITEM_SELECTED;
		sliderXpos = 0;
		sliderYpos = 0;
		sliderZpos = 0;
		sliderRadiusPos = 0;
		sliderWidthPos = 0;
		sliderHeightPos = 0;
		sliderLengthPos = 0;
	}
	if (sliderIdObjs <= lstObjSettings.size() && selectedModelObj == nullptr) {
		if (oldSelectedId != sliderIdObjs) {
			oldSelectedId = sliderIdObjs;
			int index = 0;
			for (std::list<ObjectBase3D<VectorObjSettings>>::iterator it = lstObjSettings.begin(); it != lstObjSettings.end(); ++it) {
				if (++index == sliderIdObjs) {
					selectedObj = &*it;
					idSelected = selectedObj->getName();
					sliderXpos = selectedObj->getObject().object3D->posStart.x;
					sliderYpos = selectedObj->getObject().object3D->posStart.y;
					sliderZpos = selectedObj->getObject().object3D->posStart.z;
					sliderRadiusPos = selectedObj->getObject().object3D->radius;
					sliderWidthPos = selectedObj->getObject().object3D->width;
					sliderHeightPos = selectedObj->getObject().object3D->height;
					sliderLengthPos = selectedObj->getObject().object3D->length;
					fillColorSelected = selectedObj->getObject().object3D->fillColor;
					break;
				}
			}
		}
		if (selectedObj != nullptr) {
			VectorObjSettings newObjSettings;
			newObjSettings.object3D = selectedObj->getObject().object3D;
			newObjSettings.object3D->posStart.x = sliderXpos;
			newObjSettings.object3D->posStart.y = sliderYpos;
			newObjSettings.object3D->posStart.z = sliderZpos;
			newObjSettings.object3D->width = sliderWidthPos;
			newObjSettings.object3D->height = sliderHeightPos;
			newObjSettings.object3D->length = sliderLengthPos;
			newObjSettings.object3D->radius = sliderRadiusPos;
			newObjSettings.object3D->fillColor = fillColorSelected;
			selectedObj->createObject(
				sliderXpos, sliderYpos, sliderZpos,
				sliderWidthPos, sliderHeightPos, sliderLengthPos,
				newObjSettings, selectedObj->getName()
			);
		}
	}

	// Models 3D
	if (selectedModelObj != nullptr && sliderIdModelObjs <= 0) {
		selectedModelObj = nullptr;
		oldSelectedModelId = 0;

		idSelected = NO_ITEM_SELECTED;
		sliderXpos = 0;
		sliderYpos = 0;
		sliderZpos = 0;
		sliderRadiusPos = 0;
		sliderWidthPos = 0;
		sliderHeightPos = 0;
		sliderLengthPos = 0;
	}
	if (sliderIdModelObjs <= listObjImport.size() && selectedObj == nullptr) {
		if (oldSelectedModelId != sliderIdModelObjs) {
			oldSelectedModelId = sliderIdModelObjs;
			int index = 0;
			for (std::list<ObjectBase3D<ofxAssimpModelLoaderExtended*>>::iterator it = listObjImport.begin(); it != listObjImport.end(); ++it) {
				if (++index == sliderIdModelObjs) {
					selectedModelObj = &*it;
					idSelected = selectedModelObj->getName();
					sliderXpos = selectedModelObj->getObject()->getPosition().x;
					sliderYpos = selectedModelObj->getObject()->getPosition().y;
					sliderZpos = selectedModelObj->getObject()->getPosition().z;
					break;
				}
			}
		}
		if (selectedModelObj != nullptr) {
			selectedModelObj->getObject()->setPosition(sliderXpos, sliderYpos, sliderZpos);
			glm::vec3 max;
			glm::vec3 min;
			getBoundingBox(*selectedModelObj->getObject(), min, max);

			glm::vec3 center;
			center.x = (min.x + max.x) / 2.0f;
			center.y = (min.y + max.y) / 2.0f;
			center.z = (min.z + max.z) / 2.0f;

			const float width = max.x - min.x;
			const float height = max.y - min.y;
			const float depth = max.z - min.z;
			selectedModelObj->createObject(
				center.x, center.y, center.z,
				width, height, depth,
				selectedModelObj->getObject(),
				selectedModelObj->getName()
			);
			sliderRadiusPos = 0;
			sliderWidthPos = 0;
			sliderHeightPos = 0;
			sliderLengthPos = 0;
		}
	}
}

void Objects3DRenderer::buttonImportObjPressed() {
	ofFileDialogResult dialogResult = ofSystemLoadDialog("Importer un objet");
	if (dialogResult.bSuccess) {
		ofLog() << "<objects3DRenderer::import: importing file>";
		importObj(dialogResult.getPath(), 0, 0);
	} else {
		ofLog() << "<objects3DRenderer::import: ABORTED>";
	}
}

void Objects3DRenderer::importObj(const std::string& path, const int& x, const int& y) {
	ofxAssimpModelLoaderExtended model;
	if (!model.loadModel(path, false)) {
		ofLogError() << "<objects3DRenderer::import: unable to load object: '" << path << "'>";
		return;
	}
	model.enableColors();
	model.enableTextures();
	model.enableMaterials();
	model.enableNormals();
	model.setPosition(0, 0, 0);

	glm::vec3 max;
	glm::vec3 min;
	getBoundingBox(model, min, max);

	glm::vec3 center;
	center.x = (min.x + max.x) / 2.0f;
	center.y = (min.y + max.y) / 2.0f;
	center.z = (min.z + max.z) / 2.0f;

	const float width = max.x - min.x;
	const float height = max.y - min.y;
	const float depth = max.z - min.z;

	listModelImport.push_back(model);
	listObjImport.push_back(ObjectBase3D<ofxAssimpModelLoaderExtended*>(center.x, center.y, center.z, width, height, depth, &listModelImport.back(), ((boost::filesystem::path) path).filename().string()));
}

// ==========================================================
// Initialisation/création des objets
void Objects3DRenderer::cube(float x, float y, float z, float width, float height, float length) {
	lstObjs.push_back(createCube(x, y, z, width, height, length));

	VectorObj* obj = &lstObjs.back();
	updateCube(*obj, x, y, z, width, height, length);

	VectorObjSettings objSet;
	objSet.object3D = obj;
	objSet.renderMode = MeshRenderMode::wireframe;

	lstObjSettings.push_back(ObjectBase3D<VectorObjSettings>(obj->posStart.x, obj->posStart.y, obj->posStart.z, obj->width, obj->height, obj->length, objSet, "3D Object: Cube " + ofToString(lstObjSettings.size())));

}

void Objects3DRenderer::sphere(float x, float y, float z, float r) {
	lstObjs.push_back(createSphere(x, y, z, r));

	VectorObj* obj = &lstObjs.back();
	updateSphere(*obj, x, y, z, r);

	VectorObjSettings objSet;
	objSet.object3D = obj;
	objSet.renderMode = MeshRenderMode::wireframe;
	lstObjSettings.push_back(ObjectBase3D<VectorObjSettings>(obj->posStart.x, obj->posStart.y, obj->posStart.z, obj->width, obj->height, obj->length, objSet, "3D Object: Sphere " + ofToString(lstObjSettings.size())));
}

void Objects3DRenderer::cone(float x, float y, float z, float r, float height) {
	lstObjs.push_back(createCone(x, y, z, r, height));

	VectorObj* obj = &lstObjs.back();
	updateCone(*obj, x, y, z, r, height);

	VectorObjSettings objSet;
	objSet.object3D = obj;
	objSet.renderMode = MeshRenderMode::wireframe;
	lstObjSettings.push_back(ObjectBase3D<VectorObjSettings>(obj->posStart.x, obj->posStart.y, obj->posStart.z, obj->width, obj->height, obj->length, objSet, "3D Object: Cone " + ofToString(lstObjSettings.size())));
}

void Objects3DRenderer::cylinder(float x, float y, float z, float r, float height) {
	lstObjs.push_back(createCylinder(x, y, z, r, height));

	VectorObj* obj = &lstObjs.back();
	updateCylinder(*obj, x, y, z, r, height);

	VectorObjSettings objSet;
	objSet.object3D = obj;
	objSet.renderMode = MeshRenderMode::wireframe;
	lstObjSettings.push_back(ObjectBase3D<VectorObjSettings>(obj->posStart.x, obj->posStart.y, obj->posStart.z, obj->width, obj->height, obj->length, objSet, "3D Object: Cylinder " + ofToString(lstObjSettings.size())));
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

	for (const ObjectBase3D<ofxAssimpModelLoaderExtended*>& model : listObjImport) {
		ofSetColor(ofColor::limit());
		model.getObject()->draw(ofPolyRenderMode::OF_MESH_WIREFRAME);

		if (boundingBox) {
			ofNoFill();
			ofSetLineWidth(2);
			ofSetColor(ofColor::lightSkyBlue);
			ofVec3f center;
			center.x = model.getCoords().start.x;
			center.y = model.getCoords().start.y;
			center.z = model.getCoords().start.z;
			ofDrawBox(center, model.getCoords().width, model.getCoords().height, model.getCoords().length);
		}
	}
	ofDisableDepthTest();
}

// ======================== Méthodes privées ========================

void Objects3DRenderer::drawObjects(const ObjectBase3D<VectorObjSettings>& objSet) {
	const VectorObj* obj = objSet.getObject().object3D;
	VectorOutline& outline = objSet.getObject().outline;
	if (objSet.getObject().renderMode == MeshRenderMode::fill) {
		ofFill();
		ofSetLineWidth(0);
	} else {
		ofNoFill();
		ofSetLineWidth(1);
	}
	switch (obj->type) {
		case VectorObject3DType::sphere:
			outline.posStart = obj->posStart;
			outline.width = obj->radius * 2;
			outline.height = obj->radius * 2;
			outline.length = obj->radius * 2;
			ofSetColor(obj->fillColor);
			ofDrawSphere(obj->posStart.x, obj->posStart.y, obj->posStart.z, obj->radius);
			break;

		case VectorObject3DType::cube:
			outline.posStart = obj->posStart;
			outline.width = obj->width;
			outline.height = obj->height;
			outline.length = obj->length;
			ofSetColor(obj->fillColor);
			ofDrawBox(obj->posStart, obj->width, obj->height, obj->length);
			break;

		case VectorObject3DType::cone:
			outline.posStart = obj->posStart;
			outline.width = obj->radius * 2;
			outline.height = obj->height;
			outline.length = obj->radius * 2;
			ofSetColor(obj->fillColor);
			ofDrawCone(obj->posStart, obj->radius, obj->height);
			break;

		case VectorObject3DType::cylinder:
			outline.posStart = obj->posStart;
			outline.width = obj->radius * 2;
			outline.height = obj->height;
			outline.length = obj->radius * 2;
			ofSetColor(obj->fillColor);
			ofDrawCylinder(obj->posStart, obj->radius, obj->height);
			break;

		default:
			break;
	}
	if (boundingBox) {
		ofNoFill();
		ofSetLineWidth(2);
		ofSetColor(outline.fillColor);
		ofDrawBox(outline.posStart, outline.width, outline.height, outline.length);
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

void Objects3DRenderer::deleteSelected() {
	if (selectedObj != nullptr) {
		lstObjs.remove(*selectedObj->getObject().object3D);
		lstObjSettings.remove(*selectedObj);
		selectedObj = nullptr;
		sliderIdObjs = std::max(0, sliderIdObjs - 1);
	}
	if (selectedModelObj != nullptr) {
		listModelImport.remove(*selectedModelObj->getObject());
		listObjImport.remove(*selectedModelObj);
		sliderIdModelObjs = std::max(0, sliderIdModelObjs - 1);
	}
	ofLog() << "<Objects3DRenderer::deleteSelected: Success>";
}


void Objects3DRenderer::getBoundingBox(ofxAssimpModelLoaderExtended& model, glm::vec3& cornerMin, glm::vec3& cornerMax) {
	glm::vec4 minTemp;
	glm::vec4 maxTemp;
	getBoundingBox(model, minTemp, maxTemp);
	cornerMin.x = minTemp.x;
	cornerMin.y = minTemp.y;
	cornerMin.z = minTemp.z;

	cornerMax.x = maxTemp.x;
	cornerMax.y = maxTemp.y;
	cornerMax.z = maxTemp.z;
}

void Objects3DRenderer::getBoundingBox(ofxAssimpModelLoaderExtended& model, glm::vec4& cornerMin, glm::vec4& cornerMax) {
	glm::mat4 modelMatrix = ofMatrix4x4::getTransposedOf(model.getModelMatrix());
	glm::vec3 min = model.getSceneMin();
	glm::vec3 max = model.getSceneMax();

	cornerMax.x = max.x;
	cornerMax.y = max.y;
	cornerMax.z = max.z;
	cornerMax.w = 1;
	cornerMax = cornerMax * modelMatrix;

	cornerMin.x = min.x;
	cornerMin.y = min.y;
	cornerMin.z = min.z;
	cornerMin.w = 1;
	cornerMin = cornerMin * modelMatrix;
}

