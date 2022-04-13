#include "raytracingManager.h"
#include "of3dGraphics.h"

void RaytracingManager::setupRenderer(const std::string& name) {
	parameters.clear();
	parameters.setName(name);

	//sceneNames.at(0) = "cornell.scn";
	//sceneNames.at(1) = "cornell_large.scn";
	//sceneNames.at(2) = "cornell_boxes.scn";
	//sceneNames.at(3) = "simple.scn";
	//sceneNames.at(4) = "caustic.scn";
	//sceneNames.at(5) = "caustic_boxes.scn";
	//sceneNames.at(6) = "caustic3.scn";
	//sceneNames.at(7) = "caustic3_boxes.scn";
	//sceneNames.at(8) = "complex.scn";

	//sceneNames.at(9) = "test.scn";
	sceneNames.push_back("test.scn");
	sceneNames.push_back("test_boxes.scn");
	sceneNames.push_back("test_cylinders.scn");
	sceneNames.push_back("test_spheres.scn");

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


	//warningNbrThreads.setFillColor(ofColor::yellow);
	//warningNbrThreads.setTextColor(ofColor::black);
	//warningNbrThreads.setBackgroundColor(ofColor::yellow);
	parameters.add(sceneIndex.set("Scene selectionnee", 0, 0, sceneNames.size() - 1));
	parameters.add(lblSceneName.setup("Nom", "")->getParameter());
	//parameters.add(btnSetupTestBox.setup("Setup Test Box")->getParameter());
	//parameters.add(btnSetupCornellBox.setup("Setup Cornell Box")->getParameter());
	parameters.add(btnGenerateRaytracedImage.setup("Generer la scene")->getParameter());
	//parameters.add(warningNbrThreads.setup("WARNING", "Aucune protection surchauffe")->getParameter());
	//parameters.add(nbThreads.set("Nombre de threads", std::thread::hardware_concurrency() / 2, 1, std::thread::hardware_concurrency()));
	parameters.add(rayPerPixel.set("Nombre de passes", 10, 4, 100000));
	//parameters.add(rayPerPixel.set("Nombre de rayons par pixel", 10, 4, 50000));
	parameters.add(imageHeight.set("Hauteur de l'image en pixel", ofGetHeight(), 256, 4098));
	parameters.add(imageWidth.set("Largeur de l'image en pixel", ofGetWidth(), 256, 4098));

	btnGenerateRaytracedImage.addListener(this, &RaytracingManager::generateRaytracedImage);
	btnSetupCornellBox.addListener(this, &RaytracingManager::setupBoiteCornell);
	btnSetupTestBox.addListener(this, &RaytracingManager::setupTestBoite);

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
	raytracerGpu.executeGPURaytracer(imageWidth, imageHeight, rayPerPixel, sceneNames.at(sceneIndex));
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

	// vider la sc�ne de son contenu
	for (size_t i = 0; i < raytracer.scene.size(); i++) {
		delete raytracer.scene.at(i);
	}
	raytracer.scene.clear();

	// g�n�ration du contenu de la sc�ne
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(box_center_x, anchor, box_size_z), Vector(), Vector(0.75, 0.75, 0.75), SurfaceType::diffuse));    // plancher
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(box_center_x, -anchor + box_size_y, box_size_z), Vector(), Vector(0.75, 0.75, 0.75), SurfaceType::diffuse));    // plafond
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(anchor + 1, box_center_y, box_size_z), Vector(), Vector(0.75, 0.25, 0.25), SurfaceType::diffuse));    // mur gauche
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(box_center_x, box_center_y, anchor), Vector(), Vector(0.25, 0.75, 0.25), SurfaceType::diffuse));    // mur arri�re
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(-anchor + 99, box_center_y, box_size_z), Vector(), Vector(0.25, 0.25, 0.75), SurfaceType::diffuse));    // mur droit
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(box_center_x, box_center_y, -anchor + 170), Vector(), Vector(0.0, 0.0, 0.0), SurfaceType::diffuse));    // mur avant

	// ensemble des sph�res situ�es � l'int�rieur de la bo�te de Cornell
	raytracer.scene.push_back(new SphereCpu(22.5, Vector(30, 30, 40), Vector(), Vector(1.0, 1.0, 1.0), SurfaceType::specular));   // sph�re mirroir
	raytracer.scene.push_back(new SphereCpu(17.5, Vector(75, 40, 75), Vector(), Vector(1.0, 1.0, 1.0), SurfaceType::refraction)); // sph�re de verre

	raytracer.scene.push_back(new SphereCpu(600, Vector(box_center_x, 600.0 + box_size_z - 0.27, box_size_z), Vector(15, 15, 15), Vector(0.0, 0.0, 0.0), SurfaceType::diffuse)); // sph�re lumineuse
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

	// vider la sc�ne de son contenu
	for (size_t i = 0; i < raytracer.scene.size(); i++) {
		delete raytracer.scene.at(i);
	}
	raytracer.scene.clear();
	// g�n�ration du contenu de la sc�ne

	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(box_center_x, anchor, box_size_z), Vector(), Vector(0.75, 0.75, 0.75), SurfaceType::diffuse));    // plancher
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(box_center_x, -anchor + box_size_y, box_size_z), Vector(), Vector(0.75, 0.75, 0.75), SurfaceType::diffuse));    // plafond
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(anchor + 1, box_center_y, box_size_z), Vector(), Vector(0.75, 0.25, 0.25), SurfaceType::diffuse));    // mur gauche
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(box_center_x, box_center_y, anchor), Vector(), Vector(0.25, 0.75, 0.25), SurfaceType::diffuse));    // mur arri�re
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(-anchor + 99, box_center_y, box_size_z), Vector(), Vector(0.25, 0.25, 0.75), SurfaceType::diffuse));    // mur droit
	raytracer.scene.push_back(new SphereCpu(wall_radius, Vector(box_center_x, box_center_y, -anchor + 170), Vector(), Vector(0.0, 0.0, 0.0), SurfaceType::diffuse));    // mur avant

	// ensemble des sph�res situ�es � l'int�rieur de la bo�te de Cornell

	//raytracer.scene.push_back(new Sphere(12.5, Vector(72, 17, 102), Vector(), Vector(1.0, 1.0, 1.0), SurfaceType::specular));   // sph�re mirroir
	raytracer.scene.push_back(new SphereCpu(17.5, Vector(32, 18, 50), Vector(), Vector(1.0, 1.0, 1.0), SurfaceType::refraction));   // sph�re de verre
	//raytracer.scene.push_back(new Cube(Vector(60, 5, 90), Vector(25, 25, 25), Vector(), Vector(1.0, 1.0, 1.0), SurfaceType::specular)); // Cube mirroir
	raytracer.scene.push_back(new CubeCpu(Vector(60, 5, 90), Vector(25, 25, 25), Vector(), Vector(0.36, 0.25, 0.75), SurfaceType::diffuse)); // Cube solide
	raytracer.scene.push_back(new CubeCpu(Vector(15, 1, 90), Vector(20, 20, 20), Vector(), Vector(1.0, 1.0, 1.0), SurfaceType::refraction)); // Cube de verre

	//camera->setPosition(glm::vec3())

	raytracer.scene.push_back(new SphereCpu(600, Vector(box_center_x, 600.0 + box_size_z - 0.27, box_size_z), Vector(15, 15, 15), Vector(0.0, 0.0, 0.0), SurfaceType::diffuse)); // sph�re lumineuse
}

