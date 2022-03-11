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

void Objects3DRenderer::updateCustom()
{
}

void Objects3DRenderer::cube(float x, float y, float z, float width, float height, float length)
{
	lstObjs.push_back(createCube(x, y, z, width, height, length));

	VectorObj* obj = &lstObjs.back();
	updateCube(*obj, x, y, z, width, height, length);

	VectorObjSettings objSet;
	//VectorOutline outline;
	//ObjectBase3D<VectorObjSettings> objSet2;
	//objSet2.getCoords();
	//VectorOutline outlineSet = Outline<VectorOutline>(x, y, z, width, height, length, this);
	objSet.object3D.push_back(obj);
	objSet.renderMode = MeshRenderMode::fill;
	//objSet.outline.push_back(x, y, z, width, height, length);
	//objSet.outline = objSet2.getCoords();
	objTemporaire = ObjectBase3D<VectorObjSettings>(obj->posStart.x, obj->posStart.y, obj->posStart.z, obj->width, obj->height, obj->length, objSet, "3D Object: Cube " + ofToString(lstObjSettings.size()));

	lstObjSettings.push_back(objTemporaire);

}

void Objects3DRenderer::sphere(float x, float y, float z, float r)
{
	lstObjs.push_back(createSphere(x, y, z, r));

	VectorObj* obj = &lstObjs.back();
	updateSphere(*obj, x, y, z, r);

	VectorObjSettings objSet;
	objSet.object3D.push_back(obj);
	objSet.renderMode = MeshRenderMode::fill;
	objTemporaire = ObjectBase3D<VectorObjSettings>(obj->posStart.x, obj->posStart.y, obj->posStart.z, obj->width, obj->height, obj->length, objSet, "3D Object: Sphere " + ofToString(lstObjSettings.size()));
	lstObjSettings.push_back(objTemporaire);
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
	for (const ObjectBase3D<VectorObjSettings>& obj : lstObjSettings) {
		this->drawObjects(obj);
	}
}

// ======================== Méthodes privées ========================

void Objects3DRenderer::drawObjects(const ObjectBase3D<VectorObjSettings>& objSet)
{
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

VectorObj Objects3DRenderer::createNewObj() {
	VectorObj obj;
	obj.fillColor.set(fillColor);
	ofLog() << "<objects3DRenderer::object created>";
	return obj;
}

VectorObj Objects3DRenderer::createCube(float x, float y, float z, float width, float height, float length)
{
	VectorObj obj = createNewObj();
	obj.type = VectorObject3DType::cube;
	this->updateCube(obj, x, y, z, width, height, length);
	ofLog() << "<objects3DRenderer::cube created>";
	return obj;
}

void Objects3DRenderer::updateCube(VectorObj& obj, float x, float y, float z, float width, float height, float length)
{
	obj.posStart.set(x, y, z);
	obj.width = width;
	obj.height = height;
	obj.length = length;
}

VectorObj Objects3DRenderer::createSphere(float x = 0.0f, float y = 0.0f, float z = 0.0f, float r = 10.0f)
{
	VectorObj obj = createNewObj();
	obj.type = VectorObject3DType::sphere;
	this->updateSphere(obj, x, y, z, r);
	return obj;
}

void Objects3DRenderer::updateSphere(VectorObj& obj, float x, float y, float z, float r)
{
	obj.posStart.set(x, y, z);
	obj.width = obj.height = obj.length = r;
	obj.radius = r;
}

VectorObj Objects3DRenderer::createCone(float x, float y, float z, float r, float height)
{
	VectorObj obj = createNewObj();
	obj.type = VectorObject3DType::cone;
	this->updateCone(obj, x, y, z, r, height);
	return obj;
}

void Objects3DRenderer::updateCone(VectorObj& obj, float x, float y, float z, float r, float height)
{
	obj.posStart.set(x, y, z);
	obj.width = obj.length = r;
	obj.radius = r;
	obj.height = height;
}

VectorObj Objects3DRenderer::createCylinder(float x, float y, float z, float r, float height)
{
	VectorObj obj = createNewObj();
	obj.type = VectorObject3DType::cylinder;
	this->updateCone(obj, x, y, z, r, height);
	return obj;
}

void Objects3DRenderer::updateCylinder(VectorObj& obj, float x, float y, float z, float r, float height)
{
	obj.posStart.set(x, y, z);
	obj.width = obj.length = r;
	obj.radius = r;
	obj.height = height;
}

// Crée un cube de base en 0,0
void Objects3DRenderer::buttonCubePressed()
{
	ofLog() << "<objects3DRenderer::buttonCubePressed>";
	cube(centerScreen[0], centerScreen[1], centerScreen[2], 40.0f, 40.0f, 40.0f);
}

// Crée une sphere de base en 0,0
void Objects3DRenderer::buttonSpherePressed()
{
	createSphere(centerScreen[0], centerScreen[1], centerScreen[2], 10);
}

// Crée un cone de base en 0,0
void Objects3DRenderer::buttonConePressed()
{
	createCone(centerScreen[0], centerScreen[1], centerScreen[2], 10, 10);
}

// Crée un cylindre de base en 0,0
void Objects3DRenderer::buttonCylinderPressed()
{
	createCylinder(centerScreen[0], centerScreen[1], centerScreen[2], 10, 10);
}

