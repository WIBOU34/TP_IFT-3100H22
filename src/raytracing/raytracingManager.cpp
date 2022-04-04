#include "raytracingManager.h"
#include "of3dGraphics.h"

void RaytracingManager::setupRenderer(const std::string& name) {
	parameters.clear();
	parameters.setName(name);

	warningNbrThreads.setFillColor(ofColor::yellow);
	warningNbrThreads.setTextColor(ofColor::black);
	warningNbrThreads.setBackgroundColor(ofColor::yellow);
	parameters.add(btnSetupTestBox.setup("Setup Test Box")->getParameter());
	parameters.add(btnSetupCornellBox.setup("Setup Cornell Box")->getParameter());
	parameters.add(btnGenerateRaytracedImage.setup("Generer une image avec lancers de rayons")->getParameter());
	parameters.add(warningNbrThreads.setup("WARNING", "Aucune protection surchauffe")->getParameter());
	parameters.add(nbThreads.set("Nombre de threads", std::thread::hardware_concurrency() / 2, 1, std::thread::hardware_concurrency()));
	parameters.add(rayPerPixel.set("Nombre de rayons par pixel", 10, 4, 50000));
	parameters.add(imageHeight.set("Hauteur de l'image en pixel", ofGetHeight(), 256, 4098));
	parameters.add(imageWidth.set("Largeur de l'image en pixel", ofGetWidth(), 256, 4098));

	btnGenerateRaytracedImage.addListener(this, &RaytracingManager::generateRaytracedImage);
	btnSetupCornellBox.addListener(this, &RaytracingManager::setupBoiteCornell);
	btnSetupTestBox.addListener(this, &RaytracingManager::setupTestBoite);
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
	//setupTestBoite();
	//setupBoiteCornell();
	transfer3DObjectsForRaytracing();
	//raytracer.image.resize(Raytracer::image_width, Raytracer::image_height);

	//std::cout << "image resize to " << Raytracer::image.width << "x" << Raytracer::image.height << " (" << Raytracer::image.count << " pixels, " << Raytracer::image.size << " MB)" << std::endl;

	//// calibration de la caméra
	//Raytracer::camera = CameraRaytracing(camera_position, camera_orientation);
	//Raytracer::camera.viewport_width = Raytracer::image.width;
	//Raytracer::camera.viewport_height = Raytracer::image.height;
	//Raytracer::camera.fov = camera_fov;
	//Raytracer::camera.calibrate();
	//glm::vec3 position = camera->getGlobalPosition();
	//glm::vec3 orientation = ofQuaternion(camera->getGlobalOrientation()).asVec3();
	//raytracer.setupCamera(
	//	Vector(position.x, position.y, position.z),
	//	Vector(orientation.x, orientation.y, orientation.z),
	//	camera->getFov() / 100,
	//	Vector(camera->getXAxis().x, camera->getXAxis().y, camera->getXAxis().z),
	//	Vector(camera->getYAxis().x, camera->getYAxis().y, camera->getYAxis().z),
	//	Vector(camera->getZAxis().x, camera->getZAxis().y, camera->getZAxis().z)
	//	);
	//if (raytracer.scene.size() > 0) {
		raytracerGpu.executeGPURaytracer(imageWidth, imageHeight, rayPerPixel);
		//raytracer.raytracerExecute(imageWidth, imageHeight, rayPerPixel, nbThreads);
	//} else {
	//	ofLog() << "<Raytracing::generateImage: scene vide>";
	//}
}

void RaytracingManager::setupBoiteCornell() {
	constexpr double anchor = 1e5;
	constexpr double wall_radius = anchor;

	constexpr double box_size_x = 100.0;
	constexpr double box_size_y = 81.6;
	constexpr double box_size_z = 81.6;

	constexpr double box_x_min = 0.0;
	constexpr double box_x_max = box_size_x;
	constexpr double box_y_min = 0.0;
	constexpr double box_y_max = box_size_y;
	constexpr double box_z_min = 0.0;
	constexpr double box_z_max = box_size_z;

	constexpr double box_center_x = (box_x_max - box_x_min) / 2.0;
	constexpr double box_center_y = (box_y_max - box_y_min) / 2.0;
	constexpr double box_center_z = (box_z_max - box_z_min) / 2.0;

	// vider la scène de son contenu
	for (size_t i = 0; i < raytracer.scene.size(); i++) {
		delete raytracer.scene.at(i);
	}
	raytracer.scene.clear();
	//sphereVector.clear();
	//cubeVector.clear();

	// génération du contenu de la scène
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(box_center_x, anchor, box_size_z), Vector(), Vector(0.75, 0.75, 0.75), SurfaceType::diffuse));    // plancher
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(box_center_x, -anchor + box_size_y, box_size_z), Vector(), Vector(0.75, 0.75, 0.75), SurfaceType::diffuse));    // plafond
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(anchor + 1, box_center_y, box_size_z), Vector(), Vector(0.75, 0.25, 0.25), SurfaceType::diffuse));    // mur gauche
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(box_center_x, box_center_y, anchor), Vector(), Vector(0.25, 0.75, 0.25), SurfaceType::diffuse));    // mur arrière
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(-anchor + 99, box_center_y, box_size_z), Vector(), Vector(0.25, 0.25, 0.75), SurfaceType::diffuse));    // mur droit
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(box_center_x, box_center_y, -anchor + 170), Vector(), Vector(0.0, 0.0, 0.0), SurfaceType::diffuse));    // mur avant

	// ensemble des sphères situées à l'intérieur de la boîte de Cornell
	raytracer.scene.push_back(new SphereCpu(22.5, Vector(30, 30, 40), Vector(), Vector(1.0, 1.0, 1.0), SurfaceType::specular));   // sphère mirroir
	raytracer.scene.push_back(new SphereCpu(17.5, Vector(75, 40, 75), Vector(), Vector(1.0, 1.0, 1.0), SurfaceType::refraction)); // sphère de verre

	raytracer.scene.push_back(new SphereCpu(600, Vector(box_center_x, 600.0 + box_size_z - 0.27, box_size_z), Vector(15, 15, 15), Vector(0.0, 0.0, 0.0), SurfaceType::diffuse)); // sphère lumineuse

	//for (size_t i = 0; i < sphereVector.size(); i++) {
	//	raytracer.scene.push_back(&sphereVector.at(i));
	//}
}

void RaytracingManager::setupTestBoite() {
	constexpr double anchor = 1e5;
	constexpr double wall_radius = anchor;

	constexpr double box_size_x = 100.0;
	constexpr double box_size_y = 81.6;
	constexpr double box_size_z = 81.6;

	constexpr double box_x_min = 0.0;
	constexpr double box_x_max = box_size_x;
	constexpr double box_y_min = 0.0;
	constexpr double box_y_max = box_size_y;
	constexpr double box_z_min = 0.0;
	constexpr double box_z_max = box_size_z;

	constexpr double box_center_x = (box_x_max - box_x_min) / 2.0;
	constexpr double box_center_y = (box_y_max - box_y_min) / 2.0;
	constexpr double box_center_z = (box_z_max - box_z_min) / 2.0;

	// vider la scène de son contenu
	for (size_t i = 0; i < raytracer.scene.size(); i++) {
		delete raytracer.scene.at(i);
	}
	raytracer.scene.clear();
	// génération du contenu de la scène
	//obj3Drenderer->sphere(box_center_x, anchor, box_size_z, wall_radius); // plancher
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->fillColor.set(ofFloatColor(0.75, 0.75, 0.75));
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->surfaceType = SurfaceType::diffuse;
	//obj3Drenderer->sphere(box_center_x, -anchor + box_size_y, box_size_z, wall_radius); // plafond
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->fillColor.set(ofFloatColor(0.75, 0.75, 0.75));
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->surfaceType = SurfaceType::diffuse;
	//obj3Drenderer->sphere(anchor + 1, box_center_y, box_size_z, wall_radius); // mur gauche
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->fillColor.set(ofFloatColor(0.75, 0.25, 0.25));
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->surfaceType = SurfaceType::diffuse;
	//obj3Drenderer->sphere(box_center_x, box_center_y, anchor, wall_radius); // mur arriere
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->fillColor.set(ofFloatColor(0.25, 0.75, 0.25));
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->surfaceType = SurfaceType::diffuse;
	//obj3Drenderer->sphere(-anchor + 99, box_center_y, box_size_z, wall_radius); // mur droit
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->fillColor.set(ofFloatColor(0.25, 0.25, 0.75));
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->surfaceType = SurfaceType::diffuse;
	//obj3Drenderer->sphere(box_center_x, box_center_y, -anchor + 170, wall_radius); // mur avant
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->fillColor.set(ofFloatColor(0.0, 0.0, 0.0));
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->surfaceType = SurfaceType::diffuse;

	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(box_center_x, anchor, box_size_z), Vector(), Vector(0.75, 0.75, 0.75), SurfaceType::diffuse));    // plancher
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(box_center_x, -anchor + box_size_y, box_size_z), Vector(), Vector(0.75, 0.75, 0.75), SurfaceType::diffuse));    // plafond
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(anchor + 1, box_center_y, box_size_z), Vector(), Vector(0.75, 0.25, 0.25), SurfaceType::diffuse));    // mur gauche
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(box_center_x, box_center_y, anchor), Vector(), Vector(0.25, 0.75, 0.25), SurfaceType::diffuse));    // mur arrière
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(-anchor + 99, box_center_y, box_size_z), Vector(), Vector(0.25, 0.25, 0.75), SurfaceType::diffuse));    // mur droit
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(box_center_x, box_center_y, -anchor + 170), Vector(), Vector(0.0, 0.0, 0.0), SurfaceType::diffuse));    // mur avant

	// ensemble des sphères situées à l'intérieur de la boîte de Cornell

	//obj3Drenderer->sphere(72, 17, 102, 12.5); // sphere mirroir
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->fillColor.set(ofFloatColor(1.0, 1.0, 1.0));
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->surfaceType = SurfaceType::specular;

	//obj3Drenderer->cube(15, 35, 90, 20, 20, 20); // cube de verre
	////obj3Drenderer->cube(15, 1, 90, 20, 20, 20); // cube de verre
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->fillColor.set(ofFloatColor(1.0, 1.0, 1.0));
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->surfaceType = SurfaceType::refraction;

	//raytracer.scene.push_back(new Sphere(12.5, Vector(72, 17, 102), Vector(), Vector(1.0, 1.0, 1.0), SurfaceType::specular));   // sphère mirroir
	raytracer.scene.push_back(new SphereCpu(17.5, Vector(32, 18, 50), Vector(), Vector(1.0, 1.0, 1.0), SurfaceType::refraction));   // sphère de verre
	//raytracer.scene.push_back(new Cube(Vector(60, 5, 90), Vector(25, 25, 25), Vector(), Vector(1.0, 1.0, 1.0), SurfaceType::specular)); // Cube mirroir
	raytracer.scene.push_back(new CubeCpu(Vector(60, 5, 90), Vector(25, 25, 25), Vector(), Vector(0.36, 0.25, 0.75), SurfaceType::diffuse)); // Cube solide
	raytracer.scene.push_back(new CubeCpu(Vector(15, 1, 90), Vector(20, 20, 20), Vector(), Vector(1.0, 1.0, 1.0), SurfaceType::refraction)); // Cube de verre


	//obj3Drenderer->sphere(box_center_x, 600.0 + box_size_z - 0.27, box_size_z, 600); // sphère lumineuse
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->fillColor.set(ofFloatColor(0.0, 0.0, 0.0));
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->surfaceType = SurfaceType::diffuse;
	//obj3Drenderer->lstObjSettings.back().getObject().object3D->material.setEmissiveColor(ofColor(15, 15, 15));

	//camera->setPosition(glm::vec3())

	raytracer.scene.push_back(new SphereCpu(600, Vector(box_center_x, 600.0 + box_size_z - 0.27, box_size_z), Vector(15, 15, 15), Vector(0.0, 0.0, 0.0), SurfaceType::diffuse)); // sphère lumineuse
}

void RaytracingManager::transfer3DObjectsForRaytracing() {
	//// vider la scène de son contenu
	//for (size_t i = 0; i < raytracer.scene.size(); i++) {
	//	delete raytracer.scene.at(i);
	//}
	//raytracer.scene.clear();
	//for (const auto& objSettings : obj3Drenderer->lstObjSettings) {
	//	const VectorObj* obj3D = objSettings.getObject().object3D;

	//	const ofColor tempEmissiveColor = ofColor(obj3D->material.getEmissiveColor());
	//	const Vector emissiveColor = Vector(tempEmissiveColor.r, tempEmissiveColor.g, tempEmissiveColor.b);
	//	const ofFloatColor tempFillColor = ofFloatColor(obj3D->fillColor);
	//	const Vector fillColor = Vector(tempFillColor.r, tempFillColor.g, tempFillColor.b);
	//	if (obj3D->type == VectorObject3DType::sphere) {
	//		raytracer.scene.push_back(
	//			new Sphere(obj3D->radius, Vector(obj3D->posStart.x, obj3D->posStart.y, obj3D->posStart.z),
	//				emissiveColor,
	//				fillColor,
	//				obj3D->surfaceType));
	//	} else if (obj3D->type == VectorObject3DType::cube) {
	//		//const Vector position = Vector(
	//		//	obj3D->posStart.x - obj3D->width / 2,
	//		//	obj3D->posStart.y - obj3D->height / 2,
	//		//	obj3D->posStart.z - obj3D->length / 2);
	//		const Vector dimensions = Vector(obj3D->width, obj3D->height, obj3D->length);
	//		raytracer.scene.push_back(
	//			new Cube(Vector(obj3D->posStart.x, obj3D->posStart.y, obj3D->posStart.z), Vector(obj3D->width, obj3D->height, obj3D->length),
	//				emissiveColor,
	//				fillColor,
	//				obj3D->surfaceType));
	//	}
	//}
}

