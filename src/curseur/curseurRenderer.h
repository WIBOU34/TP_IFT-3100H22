#pragma once

#include "../renderer.h"


class CurseurRenderer : public RendererBase
{
public:

protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();

private:
	void draw_zone(float x1, float y1, float x2, float y2) const;
	void draw_cursor(float x, float y) const;
};

