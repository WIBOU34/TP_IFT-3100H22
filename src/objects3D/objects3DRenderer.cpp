#include "objects3DRenderer.h"
#include "of3dGraphics.h"


void Objects3DRenderer::setup(const std::string& name)
{
	parameters3D.clear();
	parameters3D.setName(name);

	bDrawCube.addListener(this, &Objects3DRenderer::buttonCubePressed);
	bDrawSphere.addListener(this, &Objects3DRenderer::buttonSpherePressed);
	bDrawCone.addListener(this, &Objects3DRenderer::buttonConePressed);
	bDrawCylinder.addListener(this, &Objects3DRenderer::buttonCylinderPressed);
	parameters3D.add(bDrawCube.setup("Ajouter un cube"));
	parameters3D.add(bDrawSphere.setup("Ajouter une sphere"));
	parameters3D.add(bDrawCone.setup("Ajouter un cone"));
	parameters3D.add(bDrawCylinder.setup("Ajouter un cylindre"));

	parameters3D.add(fillColor.set("Fill color", ofColor::red));

	selectedObjParams.add(idSelected.set("ID", NO_ITEM_SELECTED));
	selectedObjParams.add(sliderXpos.set("Position X", 0.0f, ofGetWidth(), 0.0f));
	selectedObjParams.add(sliderYpos.set("Position Y", 0.0f, ofGetHeight(), 0.0f));
	selectedObjParams.add(sliderZpos.set("Position Z", -100.0f, 100.0f, 0.0f));
	selectedObjParams.add(sliderRadiusPos.set("Rayon", 0.0f, 1000.0f, 0.0f));
	selectedObjParams.add(sliderWidthPos.set("Largeur", 0.0f, 1000.0f, 0.0f));
	selectedObjParams.add(sliderHeightPos.set("Hauteur", 0.0f, 1000.0f, 0.0f));
	selectedObjParams.add(sliderLengthPos.set("Profondeur", 0.0f));
	selectedObjParams.add(fillColorSelected.set("Fill color selected", fillColor.get()));

	parameters3D.add(&selectedObjParams);

	graphics.enableDraw();
}

void Objects3DRenderer::updateCustom()
{
}

void Objects3DRenderer::cube(float x, float y, float z, float width, float height, float length)
{
	if (!updateShapeObj3D) {
		lstObjSettings.push_back(createCube(x, y, z, width, height, length));
	}
	VectorObjectSettings* obj = &lstObjSettings.back();
	updateCube(*obj, x, y, z, width, height, length);

	VectorObjs3D forme;
	forme.object3D.push_back(obj);
	forme.renderMode = MeshRenderMode::fill;
	objTemporaire = ObjectBase3D<VectorObjs3D>(obj->posStart.x, obj->posStart.y, obj->posStart.z, obj->width, obj->height, obj->length, forme, "3D Object: Cube " + ofToString(lstObj.size()));
	if (saveShape) {
		lstObj.push_back(objTemporaire);
	}
}

void Objects3DRenderer::sphere(float x, float y, float z, float r)
{
	if (!updateShapeObj3D) {
		lstObjSettings.push_back(createSphere(x, y, z, r));
	}
	VectorObjectSettings* obj = &lstObjSettings.back();
	updateSphere(*obj, x, y, z, r);

	VectorObjs3D forme;
	forme.object3D.push_back(obj);
	forme.renderMode = MeshRenderMode::fill;
	objTemporaire = ObjectBase3D<VectorObjs3D>(obj->posStart.x, obj->posStart.y, obj->posStart.z, obj->width, obj->height, obj->length, forme, "3D Object: Cube " + ofToString(lstObj.size()));
	if (saveShape) {
		lstObj.push_back(objTemporaire);
	}
}

void Objects3DRenderer::selectObject()
{
}

void Objects3DRenderer::generateDraw()
{
}

void Objects3DRenderer::render()
{
	//ofLog() << "<objects3DRenderer::render>";
	for (const ObjectBase3D<VectorObjs3D>& obj : lstObj) {
		this->drawObjects(obj);
	}
}

// ======================== M�thodes priv�es ========================

void Objects3DRenderer::drawObjects(const ObjectBase3D<VectorObjs3D>& forme)
{
	for (const VectorObjectSettings* obj : forme.getObject().object3D) {
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
			ofLog() << "<objects3DRenderer::cube drawn>";
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

VectorObjectSettings Objects3DRenderer::createNewObj() {
	VectorObjectSettings obj;
	obj.fillColor.set(fillColor);
	ofLog() << "<objects3DRenderer::object created>";
	return obj;
}

VectorObjectSettings Objects3DRenderer::createCube(float x, float y, float z, float width, float height, float length)
{
	VectorObjectSettings obj = createNewObj();
	obj.type = VectorObject3DType::cube;
	this->updateCube(obj, x, y, z, width, height, length);
	ofLog() << "<objects3DRenderer::cube created>";
	return obj;
}

void Objects3DRenderer::updateCube(VectorObjectSettings& obj, float x, float y, float z, float width, float height, float length)
{
	obj.posStart.set(x, y, z);
	obj.width = width;
	obj.height = height;
	obj.length = length;
}

VectorObjectSettings Objects3DRenderer::createSphere(float x = 0.0f, float y = 0.0f, float z = 0.0f, float r = 10.0f)
{
	VectorObjectSettings obj = createNewObj();
	obj.type = VectorObject3DType::sphere;
	this->updateSphere(obj, x, y, z, r);
	return obj;
}

void Objects3DRenderer::updateSphere(VectorObjectSettings& obj, float x, float y, float z, float r)
{
	obj.posStart.set(x, y, z);
	obj.width = obj.height = obj.length = r;
	obj.radius = r;
}

VectorObjectSettings Objects3DRenderer::createCone(float x, float y, float z, float r, float height)
{
	VectorObjectSettings obj = createNewObj();
	obj.type = VectorObject3DType::cone;
	this->updateCone(obj, x, y, z, r, height);
	return obj;
}

void Objects3DRenderer::updateCone(VectorObjectSettings& obj, float x, float y, float z, float r, float height)
{
	obj.posStart.set(x, y, z);
	obj.width = obj.length = r;
	obj.radius = r;
	obj.height = height;
}

VectorObjectSettings Objects3DRenderer::createCylinder(float x, float y, float z, float r, float height)
{
	VectorObjectSettings obj = createNewObj();
	obj.type = VectorObject3DType::cylinder;
	this->updateCone(obj, x, y, z, r, height);
	return obj;
}

void Objects3DRenderer::updateCylinder(VectorObjectSettings& obj, float x, float y, float z, float r, float height)
{
	obj.posStart.set(x, y, z);
	obj.width = obj.length = r;
	obj.radius = r;
	obj.height = height;
}

// Cr�e un cube de base en 0,0
void Objects3DRenderer::buttonCubePressed()
{
	ofLog() << "<objects3DRenderer::buttonCubePressed>";
	cube(centerScreen[0], centerScreen[1], centerScreen[2], 40.0f, 40.0f, 40.0f);
}

// Cr�e une sphere de base en 0,0
void Objects3DRenderer::buttonSpherePressed()
{
	createSphere(centerScreen[0], centerScreen[1], centerScreen[2], 10);
}

// Cr�e un cone de base en 0,0
void Objects3DRenderer::buttonConePressed()
{
	createCone(centerScreen[0], centerScreen[1], centerScreen[2], 10, 10);
}

// Cr�e un cylindre de base en 0,0
void Objects3DRenderer::buttonCylinderPressed()
{
	createCylinder(centerScreen[0], centerScreen[1], centerScreen[2], 10, 10);
}
