#include "dessinRenderer.h"


void DessinRenderer::setupRenderer(const std::string& name) {
	parameters.clear();
	parameters.setName(name);

	parameters.add(btnDrawPoint.setup(DRAW_POINT_NAME, false)->getParameter());
	parameters.add(btnDrawSquare.setup(DRAW_SQUARE_NAME, false)->getParameter());
	parameters.add(btnDrawRectangle.setup(DRAW_RECTANGLE_NAME, false)->getParameter());
	parameters.add(btnDrawCircle.setup(DRAW_CIRCLE_NAME, false)->getParameter());
	parameters.add(btnDrawEllipse.setup(DRAW_ELLIPSE_NAME, false)->getParameter());
	parameters.add(btnDrawLine.setup(DRAW_LINE_NAME, false)->getParameter());
	parameters.add(btnStopDrawing.setup("Arreter dessin", false)->getParameter());

	parameters.add(strokeWidth.set("Stroke width", 1, 1, 20));
	parameters.add(strokeColor.set("Stroke color", ofColor::black));
	parameters.add(fillColor.set("Fill color", ofColor::red));

	parameters.add(btnDeleteSelected.setup("Supprimer forme selectionnee", false)->getParameter());
	parameters.add(idSelected.set("ID", NO_ITEM_SELECTED));
	parameters.add(strokeColorSelected.set("Stroke color selected", strokeColor.get()));
	parameters.add(fillColorSelected.set("Fill color selected", fillColor.get()));
	parameters.add(strokeWidthSelected.set("Stroke width selected", strokeWidth.get(), 1, 20));
	graphics.enableDraw();

	btnDeleteSelected.addListener(this, &DessinRenderer::deleteSelected);
	btnDrawPoint.addListener(this, &DessinRenderer::deselectButtonsPoint);
	btnDrawSquare.addListener(this, &DessinRenderer::deselectButtonsSquare);
	btnDrawRectangle.addListener(this, &DessinRenderer::deselectButtonsRectangle);
	btnDrawCircle.addListener(this, &DessinRenderer::deselectButtonsCircle);
	btnDrawEllipse.addListener(this, &DessinRenderer::deselectButtonsEllipse);
	btnDrawLine.addListener(this, &DessinRenderer::deselectButtonsLine);
	btnStopDrawing.addListener(this, &DessinRenderer::deselectButtons);
	//TODO: faire le triangle si c'est simple
	btnDrawTriangle.addListener(this, &DessinRenderer::deselectButtonsTriangle);
}

void DessinRenderer::updateRenderer() {
	if (selectedForme != nullptr && !updateShape && saveShape) {
		VectorForme newForme = selectedForme->getObject();
		const float differenceX = is_mouse_button_pressed ? this->mouse_current_x - this->mouse_press_x_updated : 0;
		const float differenceY = is_mouse_button_pressed ? this->mouse_current_y - this->mouse_press_y_updated : 0;
		for (VectorPrimitive* primitive : newForme.primitives) {
			primitive->posStart.x += differenceX;
			primitive->posStart.y += differenceY;
			primitive->posEnd.x += differenceX;
			primitive->posEnd.y += differenceY;
			primitive->fillColor = fillColorSelected.get();
			primitive->strokeColor = strokeColorSelected.get();
			primitive->strokeWidth = strokeWidthSelected.get();
		}
		selectedForme->createObject(
			selectedForme->getCoords().origine.x + differenceX,
			selectedForme->getCoords().origine.y + differenceY,
			selectedForme->getCoords().end.x + differenceX,
			selectedForme->getCoords().end.y + differenceY,
			newForme,
			selectedForme->getName()
		);
		idSelected.set(selectedForme->getName());
		if (is_mouse_button_pressed) {
			this->mouse_press_x_updated = this->mouse_current_x;
			this->mouse_press_y_updated = this->mouse_current_y;
		}
	} else {
		fillColorSelected.set(fillColor.get());
		strokeColorSelected.set(strokeColor.get());
		strokeWidthSelected.set(strokeWidth.get());
		idSelected.set(NO_ITEM_SELECTED);
	}
}

void DessinRenderer::selectPrimitive() {
	bool selectedFound = false;
	const ofVec2f find = ofVec2f(this->mouse_press_x, this->mouse_press_y);
	for (std::list<ObjectBase2D<VectorForme>>::reverse_iterator rit = lstFormes.rbegin(); rit != lstFormes.rend(); ++rit) {
		ObjectBase2D<VectorForme>& forme = *rit;

		for (const VectorPrimitive* primitive : forme.getObject().primitives) {
			const ofVec2f topRight = ofVec2f(primitive->posEnd.x, primitive->posStart.y);
			const ofVec2f bottomLeft = ofVec2f(primitive->posStart.x, primitive->posEnd.y);
			switch (primitive->type) {
				case VectorPrimitiveType::square:
				case VectorPrimitiveType::rectangle:
					selectedFound = isPointOnLine(primitive->posStart, topRight, find)	// topLine
						|| isPointOnLine(primitive->posStart, bottomLeft, find)			// left
						|| isPointOnLine(topRight, primitive->posEnd, find)				// right
						|| isPointOnLine(bottomLeft, primitive->posEnd, find);			// bottom
					break;
				case VectorPrimitiveType::circle:
				case VectorPrimitiveType::ellipse:
					selectedFound = isPointOnEllipse(primitive->posStart, ofVec2f(primitive->posStart.x - primitive->posEnd.x, primitive->posStart.y - primitive->posEnd.y), find);
					break;
				case VectorPrimitiveType::pixel:
				case VectorPrimitiveType::point:
				case VectorPrimitiveType::line:
					selectedFound = isPointOnLine(primitive->posStart, primitive->posEnd, find);
					break;
				default:
					ofLog(ofLogLevel::OF_LOG_WARNING) << "<DessinRenderer::SelectPrimitive Primitive not found>";
					break;
			}
			if (selectedFound) {
				strokeColorSelected.set(primitive->strokeColor);
				fillColorSelected.set(primitive->fillColor);
				strokeWidthSelected.set(primitive->strokeWidth);
				break;
			}
		}
		if (selectedFound) {
			idSelected = forme.getName();
			selectedForme = &forme;
			break;
		}
	}
	if (!selectedFound) {
		selectedForme = nullptr;
		idSelected = NO_ITEM_SELECTED;
	}
}

void DessinRenderer::beginShapeDraw() {
	if (bDrawPoint || bDrawLine || bDrawSquare || bDrawRectangle || bDrawCircle || bDrawEllipse || bDrawTriangle) {
		updateShape = false;
		saveShape = false;
		this->drawAtMouse();
		updateShape = true;
	}
}

void DessinRenderer::drawAtMouse() {
	if (bDrawPoint) {
		this->point(this->mouse_press_x, this->mouse_press_y);
	} else if (bDrawSquare) {
		this->square(this->mouse_press_x, this->mouse_press_y, this->mouse_current_x, this->mouse_current_y);
	} else if (bDrawRectangle) {
		this->rect(this->mouse_press_x, this->mouse_press_y, this->mouse_current_x, this->mouse_current_y);
	} else if (bDrawCircle) {
		this->circle(this->mouse_press_x, this->mouse_press_y, this->mouse_current_x, this->mouse_current_y);
	} else if (bDrawEllipse) {
		this->ellipse(this->mouse_press_x, this->mouse_press_y, this->mouse_current_x, this->mouse_current_y);
	} else if (bDrawLine) {
		this->line(this->mouse_press_x, this->mouse_press_y, this->mouse_current_x, this->mouse_current_y);
	} else if (bDrawTriangle) {
		ofLog() << "DessinRenderer::drawTriangle: not implemented yet>";
	}
}

void DessinRenderer::completeShapeDrawn() {
	updateShape = true;
	saveShape = true;
	this->drawAtMouse();
	updateShape = false;
}

void DessinRenderer::generateDraw() {
}

void DessinRenderer::render() {
	for (const ObjectBase2D<VectorForme>& forme : lstFormes) {
		this->drawPrimitives(forme);
	}
	if (!saveShape && updateShape && this->is_mouse_button_pressed) {
		this->drawAtMouse();
		this->drawPrimitives(this->formeTemporaire);
	}
}

void DessinRenderer::drawPrimitives(const ObjectBase2D<VectorForme>& forme) {
	for (const VectorPrimitive* primitive : forme.getObject().primitives) {
		switch (primitive->type) {
			case VectorPrimitiveType::pixel:
			case VectorPrimitiveType::point:
			case VectorPrimitiveType::line:
				graphics.noFill();
				graphics.setLineWidth(primitive->strokeWidth);
				ofSetColor(primitive->strokeColor);
				graphics.line(primitive->posStart.x, primitive->posStart.y, primitive->posEnd.x, primitive->posEnd.y);
				break;

			case VectorPrimitiveType::square:
			case VectorPrimitiveType::rectangle:
				graphics.fill();
				graphics.setLineWidth(0);
				ofSetColor(primitive->fillColor);
				//graphics.changeColor(primitive->fillColor.r, primitive->fillColor.g, primitive->fillColor.b);
				graphics.rect(
					primitive->posStart.x,
					primitive->posStart.y,
					primitive->posEnd.x - primitive->posStart.x,
					primitive->posEnd.y - primitive->posStart.y);

				graphics.noFill();
				graphics.setLineWidth(primitive->strokeWidth);
				//graphics.changeColor(primitive->strokeColor.r, primitive->strokeColor.g, primitive->strokeColor.b);
				ofSetColor(primitive->strokeColor);
				graphics.rect(
					primitive->posStart.x,
					primitive->posStart.y,
					primitive->posEnd.x - primitive->posStart.x,
					primitive->posEnd.y - primitive->posStart.y);
				break;

			case VectorPrimitiveType::circle:
			case VectorPrimitiveType::ellipse:
				graphics.fill();
				graphics.setLineWidth(0);
				ofSetColor(primitive->fillColor);
				//graphics.changeColor(primitive->fillColor.r, primitive->fillColor.g, primitive->fillColor.b);
				graphics.ellipse(
					primitive->posStart.x,
					primitive->posStart.y,
					(primitive->posEnd.x - primitive->posStart.x) * 2,
					(primitive->posEnd.y - primitive->posStart.y) * 2);

				graphics.noFill();
				graphics.setLineWidth(primitive->strokeWidth);
				//graphics.changeColor(primitive->strokeColor.r, primitive->strokeColor.g, primitive->strokeColor.b);
				ofSetColor(primitive->strokeColor);
				graphics.ellipse(
					primitive->posStart.x,
					primitive->posStart.y,
					(primitive->posEnd.x - primitive->posStart.x) * 2,
					(primitive->posEnd.y - primitive->posStart.y) * 2);
				break;
			default:
				break;
		}
	}
}

void DessinRenderer::point(float x, float y) {
	if (!updateShape) {
		lstPrimitives.push_back(createPoint(x, y));
	}
	VectorPrimitive* primitive = &lstPrimitives.back();
	updatePoint(*primitive, x, y);

	VectorForme forme;
	forme.primitives.push_back(primitive);
	forme.type = VectorFormeType::primitive;
	formeTemporaire = ObjectBase2D<VectorForme>(primitive->posStart.x, primitive->posStart.y, primitive->posEnd.x, primitive->posEnd.y, forme, "FormePrimitive Point " + ofToString(lstFormes.size()));
	if (saveShape) {
		lstFormes.push_back(formeTemporaire);
	}
}

void DessinRenderer::square(float x1, float y1, float x2, float y2) {
	if (!updateShape) {
		lstPrimitives.push_back(createSquare(x1, y1, x2, y2));
	}
	VectorPrimitive* primitive = &lstPrimitives.back();
	updateSquare(*primitive, x1, y1, x2, y2);

	VectorForme forme;
	forme.primitives.push_back(primitive);
	forme.type = VectorFormeType::primitive;
	formeTemporaire = ObjectBase2D<VectorForme>(primitive->posStart.x, primitive->posStart.y, primitive->posEnd.x, primitive->posEnd.y, forme, "FormePrimitive Carre " + ofToString(lstFormes.size()));
	if (saveShape) {
		lstFormes.push_back(formeTemporaire);
	}
}

void DessinRenderer::rect(float x1, float y1, float x2, float y2) {
	if (!updateShape) {
		lstPrimitives.push_back(createRectangle(x1, y1, x2, y2));
	}
	VectorPrimitive* primitive = &lstPrimitives.back();
	updateRectangle(*primitive, x1, y1, x2, y2);

	VectorForme forme;
	forme.primitives.push_back(primitive);
	forme.type = VectorFormeType::primitive;
	formeTemporaire = ObjectBase2D<VectorForme>(primitive->posStart.x, primitive->posStart.y, primitive->posEnd.x, primitive->posEnd.y, forme, "FormePrimitive Rectangle " + ofToString(lstFormes.size()));
	if (saveShape) {
		lstFormes.push_back(formeTemporaire);
	}
}

void DessinRenderer::circle(float xc, float yc, float x2, float y2) {
	if (!updateShape) {
		lstPrimitives.push_back(createCircle(xc, yc, x2, y2));
	}
	VectorPrimitive* primitive = &lstPrimitives.back();
	updateCircle(*primitive, xc, yc, x2, y2);

	VectorForme forme;
	forme.primitives.push_back(primitive);
	forme.type = VectorFormeType::primitive;
	formeTemporaire = ObjectBase2D<VectorForme>(primitive->posStart.x, primitive->posStart.y, primitive->posEnd.x, primitive->posEnd.y, forme, "FormePrimitive Cercle " + ofToString(lstFormes.size()));
	if (saveShape) {
		lstFormes.push_back(formeTemporaire);
	}
}

// x1 et y1 sont les coordonnées du centre
void DessinRenderer::ellipse(float x1, float y1, float x2, float y2) {
	const float width = (x2 - x1) * 2;
	const float height = (y2 - y1) * 2;
	if (!updateShape) {
		lstPrimitives.push_back(createEllipse(x1, y1, x2, y2));
	}
	VectorPrimitive* primitive = &lstPrimitives.back();
	updateEllipse(*primitive, x1, y1, x2, y2);

	VectorForme forme;
	forme.primitives.push_back(primitive);
	forme.type = VectorFormeType::primitive;
	formeTemporaire = ObjectBase2D<VectorForme>(primitive->posStart.x, primitive->posStart.y, primitive->posEnd.x, primitive->posEnd.y, forme, "FormePrimitive Ellipse " + ofToString(lstFormes.size()));
	if (saveShape) {
		lstFormes.push_back(formeTemporaire);
	}
}

void DessinRenderer::line(float x1, float y1, float x2, float y2) {
	if (!updateShape) {
		lstPrimitives.push_back(createLine(x1, y1, x2, y2));
	}
	VectorPrimitive* primitive = &lstPrimitives.back();
	updateLine(*primitive, x1, y1, x2, y2);

	VectorForme forme;
	forme.primitives.push_back(primitive);
	forme.type = VectorFormeType::primitive;
	formeTemporaire = ObjectBase2D<VectorForme>(primitive->posStart.x, primitive->posStart.y, primitive->posEnd.x, primitive->posEnd.y, forme, "FormePrimitive Ligne " + ofToString(lstFormes.size()));
	if (saveShape) {
		lstFormes.push_back(formeTemporaire);
	}
}

// ======================== Méthodes privées ========================

VectorPrimitive DessinRenderer::createPrimitive() {
	VectorPrimitive primitive;
	primitive.fillColor.set(fillColor);
	primitive.strokeColor.set(strokeColor);
	primitive.strokeWidth = strokeWidth.get();
	return primitive;
}

VectorPrimitive DessinRenderer::createPoint(float x, float y) {
	VectorPrimitive primitive = createPrimitive();
	primitive.type = VectorPrimitiveType::point;
	this->updatePoint(primitive, x, y);

	return primitive;
}

void DessinRenderer::updatePoint(VectorPrimitive& primitive, float x, float y) {
	primitive.posStart.set(x, y);
	primitive.posEnd.set(x, y);
}

VectorPrimitive DessinRenderer::createSquare(float x1, float y1, float x2, float y2) {
	VectorPrimitive primitive = createPrimitive();
	primitive.type = VectorPrimitiveType::square;
	this->updateSquare(primitive, x1, y1, x2, y2);

	return primitive;
}

void DessinRenderer::updateSquare(VectorPrimitive& primitive, float x1, float y1, float x2, float y2) {
	primitive.posStart.set(x1, y1);
	const float width = (x2 - x1);
	const float height = (y2 - y1);
	if (std::abs(width) > std::abs(height)) {
		primitive.posEnd.set(x1 + width, y1 + width);
	} else {
		primitive.posEnd.set(x1 + height, y1 + height);
	}
}

VectorPrimitive DessinRenderer::createRectangle(float x1, float y1, float x2, float y2) {
	VectorPrimitive primitive = createPrimitive();
	primitive.type = VectorPrimitiveType::rectangle;
	this->updateRectangle(primitive, x1, y1, x2, y2);

	return primitive;
}

void DessinRenderer::updateRectangle(VectorPrimitive& primitive, float x1, float y1, float x2, float y2) {
	primitive.posStart.set(x1, y1);
	primitive.posEnd.set(x2, y2);
}

VectorPrimitive DessinRenderer::createLine(float x1, float y1, float x2, float y2) {
	VectorPrimitive primitive = createPrimitive();
	primitive.type = VectorPrimitiveType::line;
	this->updateLine(primitive, x1, y1, x2, y2);

	return primitive;
}

void DessinRenderer::updateLine(VectorPrimitive& primitive, float x1, float y1, float x2, float y2) {
	primitive.posStart.set(x1, y1);
	primitive.posEnd.set(x2, y2);
}

VectorPrimitive DessinRenderer::createCircle(float xc, float yc, float x2, float y2) {
	VectorPrimitive primitive = createPrimitive();
	primitive.type = VectorPrimitiveType::circle;
	this->updateCircle(primitive, xc, yc, x2, y2);
	return primitive;
}

void DessinRenderer::updateCircle(VectorPrimitive& primitive, float xc, float yc, float x2, float y2) {
	primitive.posStart.set(xc, yc);
	const float radiusX = (x2 - xc);
	const float radiusY = (y2 - yc);
	if (std::abs(radiusX) > std::abs(radiusY)) {
		primitive.posEnd.set(xc + radiusX, yc + radiusX);
	} else {
		primitive.posEnd.set(xc + radiusY, yc + radiusY);
	}
}

VectorPrimitive DessinRenderer::createEllipse(float xc, float yc, float x2, float y2) {
	VectorPrimitive primitive = createPrimitive();
	primitive.type = VectorPrimitiveType::ellipse;
	this->updateEllipse(primitive, xc, yc, x2, y2);

	return primitive;
}

void DessinRenderer::updateEllipse(VectorPrimitive& primitive, float xc, float yc, float x2, float y2) {
	primitive.posStart.set(xc, yc);
	primitive.posEnd.set(x2, y2);
}

bool DessinRenderer::isPointOnLine(const ofVec2f& start, const ofVec2f& end, const ofVec2f& find) const {
	const float distanceOriginale = ofDist(start.x, start.y, end.x, end.y);
	const float distanceStartFind = ofDist(start.x, start.y, find.x, find.y);
	const float distanceFindEnd = ofDist(find.x, find.y, end.x, end.y);

	const float difference = abs(distanceOriginale - (distanceStartFind + distanceFindEnd));
	return difference < 0.05f;
}

bool DessinRenderer::isPointOnEllipse(const ofVec2f& center, const ofVec2f& radiusXY, const ofVec2f& find) const {
	const float distance = std::powf(find.x - center.x, 2) / std::powf(radiusXY.x, 2) + std::powf(find.y - center.y, 2) / std::powf(radiusXY.y, 2);
	return distance > 0.8f && distance < 1.1f;
}

void DessinRenderer::deleteSelected() {
	if (selectedForme != nullptr) {
		for (VectorPrimitive* primitive : selectedForme->getObject().primitives) {
			lstPrimitives.remove(*primitive);
			primitive = nullptr;
		}
		lstFormes.remove(*selectedForme);
		selectedForme = nullptr;
	}
	ofLog() << "<DessinRenderer::deleteSelected: Success>";
}


// ==================================== LISTENERS ====================================

void DessinRenderer::deselectButtonsPoint() {
	deselectButtons();
	bDrawPoint = true;
}

void DessinRenderer::deselectButtonsSquare() {
	deselectButtons();
	bDrawSquare = true;
}

void DessinRenderer::deselectButtonsRectangle() {
	deselectButtons();
	bDrawRectangle = true;
}

void DessinRenderer::deselectButtonsCircle() {
	deselectButtons();
	bDrawCircle = true;
}

void DessinRenderer::deselectButtonsEllipse() {
	deselectButtons();
	bDrawEllipse = true;
}

void DessinRenderer::deselectButtonsLine() {
	deselectButtons();
	bDrawLine = true;
}

void DessinRenderer::deselectButtonsTriangle() {
	deselectButtons();
	bDrawTriangle = true;
}

void DessinRenderer::deselectButtons() {
	bDrawPoint = false;
	bDrawSquare = false;
	bDrawRectangle = false;
	bDrawCircle = false;
	bDrawEllipse = false;
	bDrawLine = false;
	bDrawTriangle = false;
}
