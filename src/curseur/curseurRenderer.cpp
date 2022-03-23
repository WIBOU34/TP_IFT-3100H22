#include "curseurRenderer.h"




void CurseurRenderer::setupRenderer(const std::string& name) {
	//group.clear();
	//group.setName(name);
	//sliderCurseurs.set("différents curseurs", 1, 1, 4);
}

void CurseurRenderer::updateRenderer() {}

void CurseurRenderer::generateDraw()
{
	//needsRedraw = false;
}

void CurseurRenderer::render()
{
	// dessiner le curseur
	//draw_cursor(mouse_current_x, mouse_current_y);
}

// fonction qui dessine un curseur
void CurseurRenderer::draw_cursor(float x, float y) const
{
	float length = 10.0f;
	float offset = 5.0f;

	if (valCurseurSlider == 1)
	{
		// curseur 1 - triangle
		ofSetLineWidth(2);
		ofSetColor(255);
		ofFill();
		ofDrawTriangle(x, y, x, y + (length * 1.4), x + length, y + length);
	}

	else if (valCurseurSlider == 2)
	{
		// curseur 2 - angle
		ofSetLineWidth(2);
		ofSetColor(255);
		ofDrawLine(x, y, x, y + (length * 1.5));
		(x, y, x + length, y + length);
	}

	else if (valCurseurSlider == 3)
	{
		// curseur 3 - white dot
		ofSetColor(255);
		ofNoFill();
		ofSetLineWidth(1);
		ofSetCurveResolution(30);
		ofDrawCircle(x, y, 2);
	}

	else if (valCurseurSlider == 4)
	{
		// curseur 4 - 4 bars
		ofSetLineWidth(2);
		ofSetColor(31);
		ofDrawLine(x + offset, y, x + offset + length, y);
		ofDrawLine(x - offset, y, x - offset - length, y);
		ofDrawLine(x, y + offset, x, y + offset + length);
		ofDrawLine(x, y - offset, x, y - offset - length);
	}
	
}