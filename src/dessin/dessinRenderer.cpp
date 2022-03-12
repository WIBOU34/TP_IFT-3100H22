#include "dessinRenderer.h"


void DessinRenderer::setupRenderer(const std::string& name) {
	parameters.clear();
	parameters.setName(name);

	//TODO: force one at a time
	parameters.add(bDrawRectangle.set("Dessiner un rectangle", false));
	parameters.add(bDrawEllipse.set("Dessiner un ellipse", false));
	parameters.add(bDrawLine.set("Dessiner une ligne", false));
	parameters.add(bDrawTriangle.set("Dessiner un triangle", false));

	parameters.add(strokeWidth.set("Stroke width", 5, 1, 50));
	parameters.add(strokeColor.set("Stroke color", ofColor::black));
	parameters.add(fillColor.set("Fill color", ofColor::red));

	parameters.add(idSelected.set("ID", NO_ITEM_SELECTED));
	parameters.add(strokeColorSelected.set("Stroke color selected", strokeColor.get()));
	parameters.add(fillColorSelected.set("Fill color selected", fillColor.get()));
	parameters.add(strokeWidthSelected.set("Stroke width selected", strokeWidth.get(), 1, 50));
	graphics.enableDraw();
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
				case VectorPrimitiveType::pixel:
					break;
				case VectorPrimitiveType::point:
					break;
				case VectorPrimitiveType::line:
					selectedFound = isPointOnLine(primitive->posStart, primitive->posEnd, find);
					break;
				case VectorPrimitiveType::rectangle:
					selectedFound = isPointOnLine(primitive->posStart, topRight, find)	// topLine
						|| isPointOnLine(primitive->posStart, bottomLeft, find)			// left
						|| isPointOnLine(topRight, primitive->posEnd, find)				// right
						|| isPointOnLine(bottomLeft, primitive->posEnd, find);			// bottom
					break;
				case VectorPrimitiveType::ellipse:
				case VectorPrimitiveType::circle:
					selectedFound = isPointOnEllipse(primitive->posStart, ofVec2f(primitive->posStart.x - primitive->posEnd.x, primitive->posStart.y - primitive->posEnd.y), find);
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
	if (bDrawLine || bDrawRectangle || bDrawEllipse || bDrawTriangle) {
		updateShape = false;
		saveShape = false;
		this->drawAtMouse();
		updateShape = true;
	}
}

void DessinRenderer::drawAtMouse() {
	if (bDrawLine.get()) {
		this->line(this->mouse_press_x, this->mouse_press_y, this->mouse_current_x, this->mouse_current_y);
	} else if (bDrawRectangle.get()) {
		this->rect(this->mouse_press_x, this->mouse_press_y, this->mouse_current_x, this->mouse_current_y);
	} else if (bDrawEllipse.get()) {
		this->ellipse(this->mouse_press_x, this->mouse_press_y, this->mouse_current_x, this->mouse_current_y);
	} else if (bDrawTriangle.get()) {
		ofLog() << "DessinRenderer::drawTriangle: not implemented yet>";
	}
}

void DessinRenderer::completeShapeDrawn() {
	updateShape = true;
	saveShape = true;
	//this->drawAtMouse();

	if (bDrawLine.get()) {
		this->line(this->mouse_press_x, this->mouse_press_y, this->mouse_released_x, this->mouse_released_y);
	} else if (bDrawRectangle.get()) {
		this->rect(this->mouse_press_x, this->mouse_press_y, this->mouse_released_x, this->mouse_released_y);
	} else if (bDrawEllipse.get()) {
		this->ellipse(this->mouse_press_x, this->mouse_press_y, this->mouse_released_x, this->mouse_released_y);
	} else if (bDrawTriangle.get()) {
		ofLog() << "DessinRenderer::drawTriangle: not implemented yet>";
	}
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
			case VectorPrimitiveType::line:
				graphics.noFill();
				ofSetColor(primitive->strokeColor);
				graphics.line(primitive->posStart.x, primitive->posStart.y, primitive->posEnd.x, primitive->posEnd.y);
				break;

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
	primitive.strokeWidth = strokeWidth;
	return primitive;
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
	//const float x = (find.x - start.x) / (end.x - start.x);
	//const float y = (find.x - start.x) / (end.x - start.x);
	//ofLog() << "<isPointOnLine: x: " << x << " y: " << y;
	//ofLog() << "<isPointOnLine: distanceOriginale: " << distanceOriginale << " distanceCombinée: " << distanceStartFind + distanceFindEnd << ">";
	//ofLog() << "<isPointOnLine: différence: " << distanceOriginale - (distanceStartFind + distanceFindEnd) << ">";

	const float difference = abs(distanceOriginale - (distanceStartFind + distanceFindEnd));
	return difference < 0.05f;
}

bool DessinRenderer::isPointOnEllipse(const ofVec2f& center, const ofVec2f& radiusXY, const ofVec2f& find) const {
	const float distance = std::powf(find.x - center.x, 2) / std::powf(radiusXY.x, 2) + std::powf(find.y - center.y, 2) / std::powf(radiusXY.y, 2);
	//ofLog() << "<radius: " << radiusXY << ">";
	//ofLog() << "<isPointOnEllipse: distance: " << distance << ">";
	return distance > 0.8f && distance < 1.1f;
}
