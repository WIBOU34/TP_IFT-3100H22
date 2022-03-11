#include "curseurRenderer.h"




void CurseurRenderer::setupRenderer(const std::string& name) {
}

void CurseurRenderer::updateRenderer() {}

void CurseurRenderer::generateDraw()
{
	//needsRedraw = false;
}

void CurseurRenderer::render()
{
	// dessiner le curseur
	draw_cursor(mouse_current_x, mouse_current_y);
}

// fonction qui dessine un curseur
void CurseurRenderer::draw_cursor(float x, float y) const
{
	float length = 10.0f;
	float offset = 5.0f;

	ofSetLineWidth(2);
	ofSetColor(31);
	ofDrawLine(x + offset, y, x + offset + length, y);
	ofDrawLine(x - offset, y, x - offset - length, y);
	ofDrawLine(x, y + offset, x, y + offset + length);
	ofDrawLine(x, y - offset, x, y - offset - length);
}