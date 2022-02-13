#include "dessinRenderer.h"


void DessinRenderer::setup(const std::string& name) {
	parameters.clear();
	parameters.setName(name);

	//TODO: force one at a time
	parameters.add(bDrawRectangle.set("Dessiner un rectangle", false));
	parameters.add(bDrawLine.set("Dessiner une ligne", false));
	parameters.add(bDrawTriangle.set("Dessiner un triangle", false));

	parameters.add(idSelected.set("ID", NO_ITEM_SELECTED));
	parameters.add(strokeWidth.set("Stroke width", 5));
	parameters.add(strokeColor.set("Stroke color", ofColor::black));
	parameters.add(fillColor.set("Fill color", ofColor::red));
	graphics.enableDraw();
}

void DessinRenderer::update() {
}

void DessinRenderer::selectPrimitive() {
	bool selectedFound = false;
	for (std::vector<ObjectBase2D<VectorForme>>::reverse_iterator rit = lstFormes.rbegin(); rit != lstFormes.rend(); ++rit) {
		ObjectBase2D<VectorForme>& forme = *rit;
		//if (forme.isPointInObject(this->mouse_released_x, this->mouse_released_y)) {

		for (const VectorPrimitive* primitive : forme.getObject().primitives) {
			switch (primitive->type) {
				case VectorPrimitiveType::line:
					selectedFound = isPointOnLine(primitive->posStart, primitive->posEnd, ofVec2f(this->mouse_released_x, this->mouse_released_y));
					break;
				case VectorPrimitiveType::rectangle:
					break;
				default:
					ofLog() << "<DessinRenderer::SelectPrimitive Primitive not found>";
					break;
			}
			if (selectedFound) {
				break;
			}
		}
		if (selectedFound) {
			idSelected = forme.getName();
			break;
		}
		//}
	}
}

void DessinRenderer::drawAtMouse() {
	if (bDrawLine.get()) {
		this->line(this->mouse_press_x, this->mouse_press_y, this->mouse_released_x, this->mouse_released_y);
	} else if (bDrawRectangle.get()) {
		this->rect(this->mouse_press_x, this->mouse_press_y, this->mouse_released_x, this->mouse_released_y);
	} else if (bDrawTriangle.get()) {
		ofLog() << "DessinRenderer::drawTriangle: not implemented yet>";
	}
	this->render();
}

void DessinRenderer::generateDraw() {
}

void DessinRenderer::render() {
	for (const ObjectBase2D<VectorForme>& forme : lstFormes) {
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
						std::abs(primitive->posEnd.x - primitive->posStart.x),
						std::abs(primitive->posEnd.y - primitive->posStart.y));

					graphics.noFill();
					graphics.setLineWidth(primitive->strokeWidth);
					//graphics.changeColor(primitive->strokeColor.r, primitive->strokeColor.g, primitive->strokeColor.b);
					ofSetColor(primitive->strokeColor);
					graphics.rect(
						primitive->posStart.x,
						primitive->posStart.y,
						std::abs(primitive->posEnd.x - primitive->posStart.x),
						std::abs(primitive->posEnd.y - primitive->posStart.y));
					break;

				case VectorPrimitiveType::ellipse:
					graphics.fill();
					graphics.setLineWidth(0);
					ofSetColor(primitive->fillColor);
					//graphics.changeColor(primitive->fillColor.r, primitive->fillColor.g, primitive->fillColor.b);
					graphics.ellipse(
						primitive->posStart.x,
						primitive->posStart.y,
						std::abs(primitive->posEnd.x - primitive->posStart.x),
						std::abs(primitive->posEnd.y - primitive->posStart.y));

					graphics.noFill();
					graphics.setLineWidth(primitive->strokeWidth);
					//graphics.changeColor(primitive->strokeColor.r, primitive->strokeColor.g, primitive->strokeColor.b);
					ofSetColor(primitive->strokeColor);
					graphics.ellipse(
						primitive->posStart.x,
						primitive->posStart.y,
						std::abs(primitive->posEnd.x - primitive->posStart.x),
						std::abs(primitive->posEnd.y - primitive->posStart.y));
					break;
				default:
					break;
			}
		}
	}
	//for (const VectorPrimitive& primitive : lstPrimitives) {

	//}
}

void DessinRenderer::rect(float x1, float y1, float x2, float y2) {
	lstPrimitives.push_back(createRectangle(x1, y1, x2, y2));
	VectorPrimitive* primitive = &lstPrimitives.back();

	VectorForme forme;
	forme.primitives.push_back(primitive);
	forme.type = VectorFormeType::primitive;
	lstFormes.push_back(ObjectBase2D<VectorForme>(primitive->posStart.x, primitive->posStart.y, primitive->posEnd.x, primitive->posEnd.y, forme, "FormePrimitive Rectangle " + ofToString(lstFormes.size())));
	graphics.rect(x1, y1, x1, y2);
}

void DessinRenderer::line(float x1, float y1, float x2, float y2) {
	lstPrimitives.push_back(createLine(x1, y1, x2, y2));
	VectorPrimitive* primitive = &lstPrimitives.back();

	VectorForme forme;
	forme.primitives.push_back(primitive);
	//forme.primitives = { &primitive };
	forme.type = VectorFormeType::primitive;
	lstFormes.push_back(ObjectBase2D<VectorForme>(primitive->posStart.x, primitive->posStart.y, primitive->posEnd.x, primitive->posEnd.y, forme, "FormePrimitive Ligne " + ofToString(lstFormes.size())));
	graphics.line(x1, y1, x2, y2);
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
	primitive.posStart.set(x1, y1);
	primitive.posEnd.set(x2, y2);

	return primitive;
}

VectorPrimitive DessinRenderer::createLine(float x1, float y1, float x2, float y2) {
	VectorPrimitive primitive = createPrimitive();
	primitive.type = VectorPrimitiveType::line;
	primitive.posStart.set(x1, y1);
	primitive.posEnd.set(x2, y2);

	return primitive;
}

VectorPrimitive DessinRenderer::createEllipse(float x1, float y1, float x2, float y2) {
	VectorPrimitive primitive = createPrimitive();
	primitive.type = VectorPrimitiveType::ellipse;
	primitive.posStart.set(x1, y1);
	primitive.posEnd.set(x2, y2);

	return primitive;
}

bool DessinRenderer::isPointOnLine(const ofVec2f& start, const ofVec2f& end, const ofVec2f& find) const {
	const float distanceOriginale = ofDist(start.x, start.y, end.x, end.y);
	const float distanceStartFind = ofDist(start.x, start.y, find.x, find.y);
	const float distanceFindEnd = ofDist(find.x, find.y, end.x, end.y);
	//const float x = (find.x - start.x) / (end.x - start.x);
	//const float y = (find.x - start.x) / (end.x - start.x);
	//ofLog() << "<isPointOnLine: x: " << x << " y: " << y;
	ofLog() << "<isPointOnLine: distanceOriginale: " << distanceOriginale << " distanceCombinée: " << distanceStartFind + distanceFindEnd << ">";
	ofLog() << "<isPointOnLine: différence: " << distanceOriginale - (distanceStartFind + distanceFindEnd) << ">";

	const float difference = abs(distanceOriginale - (distanceStartFind + distanceFindEnd));
	return difference < 0.05f;
}
