#include "imageRenderer.h"


void ImageRenderer::importImageDialog() {
	ofFileDialogResult dialogResult = ofSystemLoadDialog("Importer une image");
	if (dialogResult.bSuccess) {
		ofLog() << "<app::import: importing file>";
		importImage(dialogResult.getPath());
	} else {
		ofLog() << "<app::import: ABORTED>";
	}
}

void ImageRenderer::setupRenderer(const std::string& pName) {
	parameters.clear();
	parameters.setName(pName);
	const float min = -5000;
	const float max = 5000;
	parameters.add(positionX.set("x1", 0, min, max));
	parameters.add(positionY.set("y1", 0, min, max));
	parameters.add(positionXend.set("x2", 0, min, max));
	parameters.add(positionYend.set("y2", 0, min, max));
	parameters.add(lblDimensions.setup("Dimensions", NO_ITEM_SELECTED)->getParameter());
	parameters.add(name.setup("Nom", NO_ITEM_SELECTED)->getParameter());
	parameters.add(btnResetSelected.setup("Reset taille origine", false)->getParameter());
	parameters.add(btnDeleteSelected.setup("Supprimer image selectionnee", false)->getParameter());

	btnResetSelected.addListener(this, &ImageRenderer::resetSelected);
	btnDeleteSelected.addListener(this, &ImageRenderer::deleteSelected);
}

void ImageRenderer::updateRenderer() {
	if (imageSelected != nullptr) {
		const float differenceX = is_mouse_button_pressed ? this->mouse_current_x - this->mouse_press_x_updated : 0;
		const float differenceY = is_mouse_button_pressed ? this->mouse_current_y - this->mouse_press_y_updated : 0;
		if (is_mouse_button_pressed) {
			this->mouse_press_x_updated = this->mouse_current_x;
			this->mouse_press_y_updated = this->mouse_current_y;
		}

		imageSelected->createObject(
			positionX + differenceX,
			positionY + differenceY,
			positionXend + differenceX,
			positionYend + differenceY,
			imageSelected->getObject(), imageSelected->getName());

		positionX = imageSelected->getCoords().origine.x;
		positionY = imageSelected->getCoords().origine.y;
		positionXend = imageSelected->getCoords().end.x;
		positionYend = imageSelected->getCoords().end.y;

		ofRectangle rect;
		rect.set(positionX, positionY, positionXend - positionX, positionYend - positionY);
		lblDimensions = to_string((int) rect.getWidth()) + "x" + to_string((int) rect.getHeight());

		name = imageSelected->getName();
	} else {
		positionX = 0;
		positionY = 0;
		positionXend = 0;
		positionYend = 0;
		lblDimensions = NO_ITEM_SELECTED;
		name = NO_ITEM_SELECTED;
	}
}

void ImageRenderer::importImage(const std::string& path) {
	this->importImage(path, 0, 0);
}
void ImageRenderer::importImage(const std::string& path, const int& x, const int& y) {
	this->importImage(path, x, y, 0, 0);
}
void ImageRenderer::importImage(const std::string& path, const int& x, const int& y, const int& width, const int& height) {
	ofImageExtended imageTemp;
	if (!imageTemp.load((boost::filesystem::path) path)) {
		ofLogError() << "<import image: unable to load image: '" << path << "'>";
		return;
	}

	int widthToUse = width;
	int heightToUse = height;

	if (width == 0) {
		widthToUse = imageTemp.getWidth();
	}
	if (height == 0) {
		heightToUse = imageTemp.getHeight();
	}
	lstImages.push_back(ObjectBase2D<ofImageExtended>(x, y, x + widthToUse, y + heightToUse, imageTemp, ((boost::filesystem::path) path).filename().string()));
	drawImage();
}

void ImageRenderer::exportImageDialog() const {
	ofFileDialogResult dialogResult = ofSystemSaveDialog("ExportedImage.png", "Exporter image");
	if (dialogResult.bSuccess) {
		ofLog() << "<app::export: saving file>";
		exportImage(dialogResult.getPath(), dialogResult.getName());
	} else {
		ofLog() << "<app::export: ABORTED>";
	}
}

void ImageRenderer::findImage(const int& x, const int& y) {
	bool imageFound = false;
	// Recherche dans la liste à l'envers pour sélectionner selon les éléments visibles
	for (std::list<ObjectBase2D<ofImageExtended>>::reverse_iterator rit = lstImages.rbegin(); rit != lstImages.rend(); ++rit) {
		ObjectBase2D<ofImageExtended>& objetBase = *rit;
		if (objetBase.isPointInObject(x, y)) {
			imageSelected = &objetBase;
			positionX = imageSelected->getCoords().origine.x;
			positionY = imageSelected->getCoords().origine.y;
			positionXend = imageSelected->getCoords().end.x;
			positionYend = imageSelected->getCoords().end.y;

			ofRectangle rect;
			rect.set(positionX, positionY, positionXend - positionX, positionYend - positionY);
			lblDimensions = to_string((int) rect.getWidth()) + "x" + to_string((int) rect.getHeight());

			name = imageSelected->getName();
			imageFound = true;
			break;
		}
	}
	if (!imageFound) {
		imageSelected = nullptr;
	}
	this->drawImage();
}

void ImageRenderer::exportImage(const std::string& path, const std::string& fileName) const {
	ofImageExtended imageToExport;
	imageToExport.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

	string pathToUse = path;
	if (fileName.empty()) { // devrais jamais arriver sur windows (impossible de sauver un fichier sans nom via le dialog)
		const string time_stamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");
		const std::string name = "ExportedImage-" + time_stamp + ".PNG";
		pathToUse = path + name;
	} else {
		size_t posExtension = fileName.find('.');
		if (posExtension == string::npos) {
			pathToUse += ".PNG";
		}
	}
	imageToExport.save(pathToUse);
	ofLog() << "<ImageRenderer::exportImage: Image exported to: " << pathToUse << ">";
}

void ImageRenderer::deleteSelected() {
	if (imageSelected != nullptr) {
		//imageSelected->getObject().clear();
		lstImages.remove(*imageSelected);
		imageSelected = nullptr;
	}
	ofLog() << "<ImageRenderer::deleteSelected: Success>";
}

void ImageRenderer::resetSelected() {
	if (imageSelected != nullptr) {
		ofRectangle rect;
		rect.set(positionX, positionY, imageSelected->getObject().getWidth(), imageSelected->getObject().getHeight());
		positionXend = rect.getBottomRight().x;
		positionYend = rect.getBottomRight().y;
		positionX = rect.getTopLeft().x;
		positionY = rect.getTopLeft().y;
		ofLog() << "<ImageRenderer::resetSelected: Success>";
	}
}

void ImageRenderer::generateDraw() {
}

void ImageRenderer::render() {
	for (const ObjectBase2D<ofImageExtended>& objetBase : lstImages) {
		const ofImageExtended image = objetBase.getObject();
		const Coords2D coords = objetBase.getCoords();
		const float width = coords.end.x - coords.origine.x;
		const float height = coords.end.y - coords.origine.y;
		if (image.isAllocated()) {
			image.draw(coords.origine.x, coords.origine.y, width, height);
		}
	}
}
void ImageRenderer::drawImage() {
	render();
}
