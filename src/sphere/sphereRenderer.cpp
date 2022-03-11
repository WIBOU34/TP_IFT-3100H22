#include "sphereRenderer.h"

void SphereRenderer::setup(const std::string& name) {
	sphereParameters.clear();
	sphereParameters.setName(name);
	radiusSphereSlider.set("Rayon de la sphere", 10.0f, 1.0f, 300.0f);
	xSphereSlider.set("Position X de la sphere", 500.0f, 0.0f, 1100.0f);
	ySphereSlider.set("Position Y de la sphere", 500.0f, 0.0f, 1100.0f);
	zSphereSlider.set("Position Z de la sphere", 10.0f, 0.0f, 100.0f);
	sphereParameters.add(radiusSphereSlider, xSphereSlider, ySphereSlider, zSphereSlider);
}

void SphereRenderer::updateCustom()
{
	sphere1.setRadius(radiusSphereSlider);
	sphere2.setRadius(radiusSphereSlider);
}

void SphereRenderer::createSphere()
{

	sphere1.set(radiusSphereSlider, 25);
	sphere2.set(radiusSphereSlider, 25);
}

void SphereRenderer::generateDraw()
{
}

void SphereRenderer::render()
{
	ofSphere(ofVec3f(xSphereSlider, ySphereSlider, zSphereSlider), sphere1.getRadius());
	ofSphere(centerScreen, sphere2.getRadius());
}
