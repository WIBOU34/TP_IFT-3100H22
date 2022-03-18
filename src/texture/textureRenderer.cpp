// IFT3100H21_AlmostEmptyProject/renderer.cpp
// Classe responsable du rendu de l'application.

#include "textureRenderer.h"

void TextureRenderer::setupRenderer(const std::string& name) {
	parameters.clear();
	parameters.setName(name);

	// load an image 
	image.load("miror.jpg");

	// create mesh from sphere 
	mesh = ofSpherePrimitive(200, 40).getMesh();
	for (int i = 0; i < mesh.getVertices().size(); i++) {
		ofVec2f texCoord = mesh.getTexCoord(i);
		texCoord.x *= image.getWidth();
		texCoord.y = (1.0 - texCoord.y) * image.getHeight();
		mesh.setTexCoord(i, texCoord);
	}

	// load the shaders 
	shader.load("shader/frag_150.glsl", "shader/geom_150.glsl", "shader/vert_150.glsl");
}

void TextureRenderer::updateRenderer() {
}

void TextureRenderer::generateDraw() {
}

void TextureRenderer::render() {
	// enable z-buffering 
	ofEnableDepthTest();

	cam.begin();
	//light.enable();
	ofPushMatrix();

	ofRotate(ofGetElapsedTimef() * 10, 0, 1, 0);

	image.bind();
	//mesh.drawWireframe();
	mesh.draw();

	ofPopMatrix();
	//light.disable();
	cam.end();
}


