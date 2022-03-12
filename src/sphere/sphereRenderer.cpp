#include "sphereRenderer.h"

void SphereRenderer::setupRenderer(const std::string& name) {
	sphereParameters.clear();
	sphereParameters.setName(name);
	radiusSphereSlider.set("Rayon de la sphere", 10.0f, 1.0f, 100.0f);
	sphereParameters.add(radiusSphereSlider);
}

void SphereRenderer::updateRenderer()
{
	sphere1.setRadius(radiusSphereSlider);
}

void SphereRenderer::createSphere()
{

	sphere1.set(radiusSphereSlider, 25);
	
}

void SphereRenderer::generateDraw()
{
}

void SphereRenderer::render()
{
	ofEnableDepthTest();
	ofSphere(origin, sphere1.getRadius());
	ofDisableDepthTest();
}
