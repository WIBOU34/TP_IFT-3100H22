// IFT3100H21_Lighting/renderer.cpp
// Classe responsable du rendu de l'application.

#include "illuminationRenderer.h"
//#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
//#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

void IlluminationRenderer::setupRenderer(const std::string& name) {
	ofSetSphereResolution(32);

	parameters.setName(name);
	parameters.add(indexShader.set("Selection Shader", 0, 0, 4));
	parameters.add(lblShaderActive.setup("Shader", "custom"));
	parameters.add(drawObjetcs.set("Dessiner les objets", false));
	parameters.add(lightPointEnabled.set("Activer lightPoint", true));
	parameters.add(lightSpotEnabled.set("Activer lightSpot", true));
	parameters.add(lightDirEnabled.set("Activer lightDirectional", true));
	parameters.add(lightAmbientEnabled.set("Activer lightAmbient", true));

	parameters.add(ambientColor.set("Couleur ambiante", ofColor(25.5, 25.5, 25.5)));

	gPoint.setName("Point");
	gPoint.add(constantPoint.set("constant: normalement 1", 1, 0, 1));
	gPoint.add(linearPoint.set("linear: reduit intensite", 0, 0, 0.05));
	gPoint.add(quadraticPoint.set("quadratic: generalement tres petit", 0, 0, 0.0001));
	gPoint.add(diffuseColorPoint.set("Couleur diffuse point", ofColor(255, 255, 255)));
	gPoint.add(specularColorPoint.set("Couleur speculaire point", ofColor(191, 191, 191)));
	gPoint.add(pPositionPoint.set("Position lumiere point", ofVec3f(0, 0, 0), ofVec3f(-1000, -1000, -1000), ofVec3f(1000, 1000, 1000)));
	parameters.add(&gPoint);
	gPoint.minimizeAll();

	gSpot.setName("Spot");
	gSpot.add(diffuseColorSpot.set("Couleur diffuse spot", ofColor(255, 255, 255)));
	gSpot.add(specularColorSpot.set("Couleur speculaire spot", ofColor(191, 191, 191)));
	gSpot.add(pPositionSpot.set("Position lumiere spot", ofVec3f(0, 0, 0), ofVec3f(-1000, -1000, -1000), ofVec3f(1000, 1000, 1000)));
	gSpot.add(constantSpot.set("constant: normalement 1", 1, 0, 1));
	gSpot.add(linearSpot.set("linear: reduit intensite", 0, 0, 0.05));
	gSpot.add(quadraticSpot.set("quadratic: generalement tres petit", 0, 0, 0.0001));
	parameters.add(&gSpot);
	gSpot.minimizeAll();

	gDirectional.setName("Directionnelle");
	gDirectional.add(diffuseColorDir.set("Couleur diffuse directionnelle", ofColor(255, 255, 255)));
	gDirectional.add(specularColorDir.set("Couleur speculaire directionnelle", ofColor(191, 191, 191)));
	gDirectional.add(pLookAtDirectional.set("Origine/Angle lumiere directionnelle", ofVec3f(9, -30, 0), ofVec3f(-100, -100, -100), ofVec3f(100, 100, 100)));
	parameters.add(&gDirectional);
	gDirectional.minimizeAll();

	parameters.add(indexObject.set("Selection Objets", 1, 1, 3));
	gMaterial.setName("Materiaux");
	gMaterial.add(setObsidian.setup("Obsidian"));
	gMaterial.add(setBronze.setup("Bronze"));
	gMaterial.add(setGold.setup("Gold"));
	gMaterial.add(setSilver.setup("Silver"));
	parameters.add(&gMaterial);

	parameters.minimizeAll();

	setObsidian.addListener(this, &IlluminationRenderer::setMaterialObsidian);
	setBronze.addListener(this, &IlluminationRenderer::setMaterialBronze);
	setGold.addListener(this, &IlluminationRenderer::setMaterialGold);
	setSilver.addListener(this, &IlluminationRenderer::setMaterialSilver);

	// paramètres
	camera_offset = 0;
	//camera_offset = 350.0f;
	speed_motion = 150.0f;
	oscillation_frequency = 7500.0f;
	oscillation_amplitude = 45.0;
	initial_x = 0.0f;
	initial_z = -100.0f;
	scale_cube = 100.0f;
	scale_sphere = 80.0f;
	scale_teapot = 0.618f;

	// chargement d'un modèle 3D
	//teapot.loadModel("geometry/Humvee.obj");
	teapot.loadModel("geometry/teapot.obj");

	// désactiver le matériau par défaut du modèle
	teapot.disableMaterials();

	// charger, compiler et linker les sources des shaders
	//shader_color_fill.load(
		//"shader/color_fill_330_vs.glsl",
		//"shader/color_fill_330_fs.glsl");

	shader_lambert.load(
		"shader/lambert_330_vs.glsl",
		"shader/lambert_330_fs.glsl");

	shader_gouraud.load(
		"shader/gouraud_330_vs.glsl",
		"shader/gouraud_330_fs.glsl");

	shader_phong.load(
		"shader/phong_330_vs.glsl",
		"shader/phong_330_fs.glsl");

	shader_blinn_phong.load(
		"shader/blinn_phong_330_vs.glsl",
		"shader/blinn_phong_330_fs.glsl");

	// shader actif au lancement de la scène
	shader_active = ShaderType::lambert;

	// initialisation de la scène
	reset();
}

void IlluminationRenderer::reset() {
	// initialisation des variables
	constantPoint = 1.0f;
	linearPoint = 0.001f;
	quadraticPoint = 5e-07f;
	constantSpot = 1.0f;
	linearSpot = 0.001f;
	quadraticSpot = 5e-07f;
	offset_x = initial_x;
	offset_z = initial_z;

	delta_x = speed_motion;
	delta_z = speed_motion;

	use_smooth_lighting = true;

	is_active_ligh_ambient = true;
	is_active_light_directional = true;
	is_active_light_point = true;
	is_active_light_spot = true;

	// centre du framebuffer
	//center_x = ofGetWidth() / 2.0f;
	//center_y = ofGetHeight() / 2.0f;
	center_x = 0;
	center_y = 0;

	// déterminer la position des géométries
	position_cube.set(-ofGetWidth() * (1.0f / 4.0f), 0.0f, 0.0f);
	position_sphere.set(0.0f, 0.0f, 0.0f);
	position_teapot.set(ofGetWidth() * (1.0f / 4.0f), 50.0f, 0.0f);
	ofBoxPrimitive box;
	box.setPosition(0, 0, 0);
	box.set(scale_cube);
	vecObjets.at(0).objet = box;
	ofSpherePrimitive sphere;
	sphere.setPosition(position_sphere);
	sphere.set(scale_sphere, 32);
	vecObjets.at(1).objet = sphere;
	of3dPrimitive model;
	model.setPosition(position_teapot);
	model.setScale(scale_teapot);
	vecObjets.at(2).objet = model;

	// configurer le matériau du cube
	vecObjets.at(0).material = ofMaterial();
	vecObjets.at(0).material.setAmbientColor(ofColor(63, 63, 63));
	vecObjets.at(0).material.setDiffuseColor(ofColor(127, 0, 0));
	vecObjets.at(0).material.setEmissiveColor(ofColor(31, 0, 0));
	vecObjets.at(0).material.setSpecularColor(ofColor(127, 127, 127));
	vecObjets.at(0).material.setShininess(16.0f);

	// configurer le matériau de la sphère
	vecObjets.at(1).material = ofMaterial();
	vecObjets.at(1).material.setAmbientColor(ofColor(63, 63, 63));
	vecObjets.at(1).material.setDiffuseColor(ofColor(191, 63, 0));
	vecObjets.at(1).material.setEmissiveColor(ofColor(0, 31, 0));
	vecObjets.at(1).material.setSpecularColor(ofColor(255, 255, 64));
	vecObjets.at(1).material.setShininess(8.0f);

	// configurer le matériau du teapot
	vecObjets.at(2).material = ofMaterial();
	vecObjets.at(2).material.setAmbientColor(ofColor(63, 63, 63));
	vecObjets.at(2).material.setDiffuseColor(ofColor(63, 0, 63));
	vecObjets.at(2).material.setEmissiveColor(ofColor(0, 0, 31));
	vecObjets.at(2).material.setSpecularColor(ofColor(191, 191, 191));
	vecObjets.at(2).material.setShininess(8.0f);

	// configurer la lumière ambiante
	light_ambient.set(25.5, 25.5, 25.5);

	// configurer la lumière directionnelle
	light_directional.setup();
	light_directional.setDiffuseColor(ofColor(31, 255, 31));
	light_directional.setSpecularColor(ofColor(191, 191, 191));
	light_directional.setOrientation(ofVec3f(0.0f, 0.0f, 0.0f));
	light_directional.setDirectional();

	// configurer la lumière ponctuelle
	light_point.setup();
	light_point.setDiffuseColor(ofColor(255, 255, 255));
	light_point.setSpecularColor(ofColor(191, 191, 191));
	light_point.setPointLight();

	// configurer la lumière projecteur
	light_spot.setup();
	light_spot.setDiffuseColor(ofColor(191, 191, 191));
	light_spot.setSpecularColor(ofColor(191, 191, 191));
	light_spot.setOrientation(ofVec3f(0.0f, 0.0f, 0.0f));
	light_spot.setSpotConcentration(2);
	light_spot.setSpotlightCutOff(30);
	light_spot.setSpotlight();

	pPositionDirectional = ofVec3f(center_x, center_y + 60, camera_offset * 0.75f);
	//pLookAtDirectional = ofVec3f(9, -30, 0);
	pLookAtDirectional = ofVec3f(-4, 4, 0);
	pPositionSpot = ofVec3f(center_x, center_y - scale_sphere * 1.5f, camera_offset * 0.75f);
	//pLookAtDirectional = ofVec3f(4, -4, 0);
	//pLookAtDirectional = orientation_directional.asVec3();
	ofLog() << "<IlluminationRenderer::reset>";
}

void IlluminationRenderer::updateRenderer() {

	if (indexObject - 1 >= 0) {
		selectedObject = &vecObjets.at(indexObject - 1);
	} else {
		selectedObject = nullptr;
	}

	ofPushMatrix();

	if (lightDirEnabled) {
		// transformer la lumière directionnelle
		//orientation_directional.makeRotate(int(ofGetElapsedTimeMillis() * 0.1f) % 360, 0, 1, 0);

		light_directional.setOrientation(orientation_directional);
		light_directional.lookAt(-glm::vec3(pLookAtDirectional.get().x, pLookAtDirectional.get().y, pLookAtDirectional.get().z));
	}

	if (lightPointEnabled) {
		// transformer la lumière ponctuelle
		light_point.setPosition(pPositionPoint.get().x, pPositionPoint.get().y, pPositionPoint.get().z);
	}

	if (lightSpotEnabled) {
		// transformer la lumière projecteur
		oscillation = oscillate(ofGetElapsedTimeMillis(), oscillation_frequency, oscillation_amplitude);

		orientation_spot.makeRotate(30.0f, ofVec3f(1, 0, 0), oscillation, ofVec3f(0, 1, 0), 0.0f, ofVec3f(0, 0, 1));

		light_spot.setOrientation(orientation_spot);

		light_spot.setPosition(pPositionSpot.get().x, pPositionSpot.get().y, pPositionSpot.get().z);
	}

	ofPopMatrix();

	light_ambient = ambientColor.get();

	light_point.setDiffuseColor(diffuseColorPoint.get());
	light_point.setSpecularColor(specularColorPoint.get());
	light_point.setAttenuation(constantPoint, linearPoint, quadraticPoint);

	light_spot.setDiffuseColor(diffuseColorSpot.get());
	light_spot.setSpecularColor(specularColorSpot.get());
	light_spot.setAttenuation(constantSpot, linearSpot, quadraticSpot);

	light_directional.setDiffuseColor(diffuseColorDir.get());
	light_directional.setSpecularColor(specularColorDir.get());


	// passer les attributs uniformes au shader de sommets
	switch ((ShaderType)indexShader.get()) {
	case ShaderType::defaut:
		lblShaderActive = "Defaut";
		shader_active = (ShaderType)indexShader.get();
		shader = nullptr;
		break;

	case ShaderType::lambert:
		lblShaderActive = "Lambert (diff) f";
		shader = &shader_lambert;
		shader_active = (ShaderType)indexShader.get();
		updateLight();
		break;

	case ShaderType::gouraud:
		lblShaderActive = "Gouraud (diff+spec) f";
		shader = &shader_gouraud;
		shader_active = (ShaderType)indexShader.get();
		updateLight();
		break;

	case ShaderType::phong:
		lblShaderActive = "Phong (diff+spec) v";
		shader = &shader_phong;
		shader_active = (ShaderType)indexShader.get();
		updateLight();
		break;

	case ShaderType::blinn_phong:
		lblShaderActive = "Blinn-Phong (diff+spec) v";
		shader = &shader_blinn_phong;
		shader_active = (ShaderType)indexShader.get();
		updateLight();
		break;

	default:
		break;
	}
}

void IlluminationRenderer::generateDraw() {
}

void IlluminationRenderer::render() {
	if (!drawObjetcs) {
		return;
	}
	ofFill();
	ofSetLineWidth(0);
	ofEnableDepthTest();
	//ofPushMatrix();
	ofSetSmoothLighting(use_smooth_lighting);

	// afficher un repère visuel pour les lumières
	if (lightPointEnabled)
		light_point.draw();
	//if (lightDirEnabled)
	//	light_directional.draw();
	if (lightSpotEnabled && shader_active == ShaderType::defaut)
		light_spot.draw();
	//ofPopMatrix();

	// activer l'éclairage dynamique
	ofEnableLighting();

	// activer les lumières
	lighting_on();
	// activer le shader
	if (shader != nullptr) {
		shader->begin();
	}

	ofPushMatrix();

	// transformer l'origine de la scène au milieu de la fenêtre d'affichage
	//ofTranslate(center_x + offset_x, center_y, offset_z);

	// légère rotation de la scène
	ofRotateDeg(ofGetFrameNum() * 0.25f, 0, 1, 0);

	// ========= Cube =========
	ofPushMatrix();

	// position
	ofTranslate(
		position_cube.x,
		position_cube.y,
		position_cube.z);

	// rotation locale
	ofRotateDeg(ofGetFrameNum() * 1.0f, 0, 1, 0);
	//vecObjets.at(0).objet.rotateDeg(ofGetFrameNum() * 1.0f, 0, 1, 0);

	setMaterialObject(vecObjets.at(0).material);
	// activer le matériau
	if (shader == nullptr) {
		vecObjets.at(0).material.begin();
	}
	// dessiner un cube
	vecObjets.at(0).objet.draw();

	// désactiver le matériau
	if (shader == nullptr) {
		vecObjets.at(0).material.end();
	}

	ofPopMatrix();

	// ========= Sphere =========
	ofPushMatrix();

	// position
	ofTranslate(
		position_sphere.x,
		position_sphere.y,
		position_sphere.z);

	// rotation locale
	ofRotateDeg(ofGetFrameNum() * 1.0f, 0, 1, 0);
	//vecObjets.at(1).objet.rotateDeg(ofGetFrameNum() * 1.0f, 0, 1, 0);

	setMaterialObject(vecObjets.at(1).material);
	// activer le matériau
	if (shader == nullptr) {
		vecObjets.at(1).material.begin();
	}
	// dessiner une sphère
	vecObjets.at(1).objet.draw();

	// désactiver le matériau
	if (shader == nullptr) {
		vecObjets.at(1).material.end();
	}

	ofPopMatrix();


	// ========= Teapot =========
	ofPushMatrix();

	// position
	teapot.setPosition(
		vecObjets.at(2).objet.getPosition().x,
		vecObjets.at(2).objet.getPosition().y + 15,
		vecObjets.at(2).objet.getPosition().z);

	// rotation locale
	teapot.setRotation(0, ofGetFrameNum() * -1.0f, 0, 1, 0);

	// dimension
	teapot.setScale(
		vecObjets.at(2).objet.getScale().x,
		vecObjets.at(2).objet.getScale().y,
		vecObjets.at(2).objet.getScale().z);

	// désactiver le matériau par défaut du modèle
	teapot.disableMaterials();


	setMaterialObject(vecObjets.at(2).material);
	// activer le matériau
	if (shader == nullptr) {
		vecObjets.at(2).material.begin();
	}
	// dessiner un teapot
	teapot.draw(OF_MESH_FILL);

	// désactiver le matériau
	if (shader == nullptr) {
		vecObjets.at(2).material.end();
	}

	ofPopMatrix();

	ofPopMatrix();

	// désactiver le shader

	if (shader != nullptr) {
		shader->end();
	}
	// désactiver les lumières
	lighting_off();

	// désactiver l'éclairage dynamique
	ofDisableLighting();
	ofDisableDepthTest();
}

// désactivation des lumières dynamiques
void IlluminationRenderer::lighting_on() {
	if (lightAmbientEnabled)
		ofSetGlobalAmbientColor(light_ambient);
	else
		ofSetGlobalAmbientColor(ofColor(0, 0, 0));

	if (lightDirEnabled)
		light_directional.enable();

	if (lightPointEnabled)
		light_point.enable();

	if (lightSpotEnabled)
		light_spot.enable();
}

// activation des lumières dynamiques
void IlluminationRenderer::lighting_off() {
	ofSetGlobalAmbientColor(ofColor(0, 0, 0));
	light_directional.disable();
	light_point.disable();
	light_spot.disable();
}

// fonction d'oscillation
float IlluminationRenderer::oscillate(float time, float frequency, float amplitude) {
	return sinf(time * 2.0f * PI / frequency) * amplitude;
}

void IlluminationRenderer::updateLight() {
	shader->begin();
	// couleur ambiante
	if (lightAmbientEnabled) {
		shader->setUniform3f("color_ambient", light_ambient.r, light_ambient.g, light_ambient.b);
	} else {
		shader->setUniform3f("color_ambient", 0, 0, 0);
	}

	// light_point
	if (lightPointEnabled) {
		shader->setUniform3f("light_point.ambiant", light_point.getAmbientColor().r, light_point.getAmbientColor().g, light_point.getAmbientColor().b);
		shader->setUniform3f("light_point.diffuse", light_point.getDiffuseColor().r, light_point.getDiffuseColor().g, light_point.getDiffuseColor().b);
		shader->setUniform3f("light_point.specular", light_point.getSpecularColor().r, light_point.getSpecularColor().g, light_point.getSpecularColor().b);
	} else {
		shader->setUniform3f("light_point.ambiant", 0, 0, 0);
		shader->setUniform3f("light_point.diffuse", 0, 0, 0);
		shader->setUniform3f("light_point.specular", 0, 0, 0);
	}
	const ofVec3f globalPosition = -(camera->getGlobalPosition() - light_point.getGlobalPosition());
	shader->setUniform3f("light_point.position", (glm::vec4(globalPosition.x, globalPosition.y, globalPosition.z, 0.0f) * camera->getGlobalTransformMatrix()) * ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
	shader->setUniform1f("light_point.constant", constantPoint.get());
	shader->setUniform1f("light_point.linear", linearPoint.get());
	shader->setUniform1f("light_point.quadratic", quadraticPoint.get());

	// light_directional
	const ofVec3f lightDirDir = ofVec3f(pLookAtDirectional.get().x, pLookAtDirectional.get().y, pLookAtDirectional.get().z);
	shader->setUniform3f("light_directional.direction", (glm::vec4(lightDirDir.x, lightDirDir.y, lightDirDir.z, 0.0f) * camera->getGlobalTransformMatrix()) * ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
	if (lightDirEnabled) {
		//light_directional.enable();
		shader->setUniform3f("light_directional.ambiant", light_directional.getAmbientColor().r, light_directional.getAmbientColor().g, light_directional.getAmbientColor().b);
		shader->setUniform3f("light_directional.diffuse", light_directional.getDiffuseColor().r, light_directional.getDiffuseColor().g, light_directional.getDiffuseColor().b);
		shader->setUniform3f("light_directional.specular", light_directional.getSpecularColor().r, light_directional.getSpecularColor().g, light_directional.getSpecularColor().b);
	} else {
		//light_directional.disable();
		shader->setUniform3f("light_directional.ambiant", 0, 0, 0);
		shader->setUniform3f("light_directional.diffuse", 0, 0, 0);
		shader->setUniform3f("light_directional.specular", 0, 0, 0);
	}

	// light_spot
	//ofVec4f lightSpotDir = ofQuaternion(light_spot.getGlobalOrientation()).asVec4();
	//shader->setUniform3f("light_spot.direction", glm::vec4(lightSpotDir[0], lightSpotDir[1], lightSpotDir[2], lightSpotDir[3]));// *ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
	////shader->setUniform3f("light_spot.direction", glm::vec4(light_spot.getLookAtDir(), 0.0f) * ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
	//shader->setUniform3f("light_spot.position", glm::vec4(light_spot.getGlobalPosition(), 0.0f));// *ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
	//shader->setUniform1f("light_spot.cutOff", light_spot.getSpotlightCutOff());
	//shader->setUniform1f("light_spot.outerCutOff", light_spot.getSpotlightCutOff() + 50);
	//if (lightSpotEnabled) {
	//	//light_spot.enable();
	//	shader->setUniform3f("light_spot.ambiant", light_spot.getAmbientColor().r, light_spot.getAmbientColor().g, light_spot.getAmbientColor().b);
	//	shader->setUniform3f("light_spot.diffuse", light_spot.getDiffuseColor().r, light_spot.getDiffuseColor().g, light_spot.getDiffuseColor().b);
	//	shader->setUniform3f("light_spot.specular", light_spot.getSpecularColor().r, light_spot.getSpecularColor().g, light_spot.getSpecularColor().b);
	//} else {
	//	//light_spot.disable();
	//	shader->setUniform3f("light_spot.ambiant", 0, 0, 0);
	//	shader->setUniform3f("light_spot.diffuse", 0, 0, 0);
	//	shader->setUniform3f("light_spot.specular", 0, 0, 0);
	//}
	//shader->setUniform1f("light_spot.constant", constant);
	//shader->setUniform1f("light_spot.linear", linear);
	//shader->setUniform1f("light_spot.quadratic", quadratic);
	shader->end();
}

// Source: http://devernay.free.fr/cours/opengl/materials.html
// Shininess n'est pas en décimal avec notre version donc x100 a ete effectue
void IlluminationRenderer::setMaterialObsidian() {
	if (selectedObject != nullptr) {
		selectedObject->material.setAmbientColor(ofFloatColor(0.05375, 0.05, 0.06625));
		selectedObject->material.setDiffuseColor(ofFloatColor(0.18275, 0.17, 0.22525));
		selectedObject->material.setSpecularColor(ofFloatColor(0.332741, 0.328634, 0.346435));
		selectedObject->material.setEmissiveColor(ofFloatColor(0.01, 0.01, 0));
		selectedObject->material.setShininess(30.0f);
	}
}

void IlluminationRenderer::setMaterialBronze() {
	if (selectedObject != nullptr) {
		selectedObject->material.setAmbientColor(ofFloatColor(0.2125, 0.1275, 0.054));
		selectedObject->material.setDiffuseColor(ofFloatColor(0.714, 0.4284, 0.18144));
		selectedObject->material.setSpecularColor(ofFloatColor(0.393548, 0.271906, 0.166721));
		selectedObject->material.setEmissiveColor(ofFloatColor(0.1, 0, 0));
		selectedObject->material.setShininess(20.0f);
	}
}

void IlluminationRenderer::setMaterialGold() {
	if (selectedObject != nullptr) {
		selectedObject->material.setAmbientColor(ofFloatColor(0.24725, 0.1995, 0.0745));
		selectedObject->material.setDiffuseColor(ofFloatColor(0.75164, 0.60648, 0.22648));
		selectedObject->material.setSpecularColor(ofFloatColor(0.628281, 0.555802, 0.366065));
		selectedObject->material.setEmissiveColor(ofFloatColor(0.1, 0.05, 0));
		selectedObject->material.setShininess(40.0f);
	}
}

void IlluminationRenderer::setMaterialSilver() {
	if (selectedObject != nullptr) {
		selectedObject->material.setAmbientColor(ofFloatColor(0.19225, 0.19225, 0.19225));
		selectedObject->material.setDiffuseColor(ofFloatColor(0.50754, 0.50754, 0.50754));
		selectedObject->material.setSpecularColor(ofFloatColor(0.508273, 0.508273, 0.508273));
		selectedObject->material.setEmissiveColor(ofFloatColor(0.1, 0.1, 0.1));
		selectedObject->material.setShininess(40.0f);
	}
}

void IlluminationRenderer::setMaterialObject(const ofMaterial& material) {
	if (shader != nullptr) {
		shader->end();
		shader->begin();
		shader->setUniform3f("material.ambient", material.getAmbientColor().r, material.getAmbientColor().g, material.getAmbientColor().b);
		shader->setUniform3f("material.diffuse", material.getDiffuseColor().r, material.getDiffuseColor().g, material.getDiffuseColor().b);
		shader->setUniform3f("material.specular", material.getSpecularColor().r, material.getSpecularColor().g, material.getSpecularColor().b);
		shader->setUniform1f("material.shininess", material.getShininess());
		shader->end();
		shader->begin();
	}
}
