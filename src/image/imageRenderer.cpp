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

void ImageRenderer::setup(const std::string& name) {
	parameters.setName(name);
}

void ImageRenderer::importImage(const std::string& path) {
	this->importImage(path, 0, 0);
}
void ImageRenderer::importImage(const std::string& path, const int& x, const int& y) {
	this->importImage(path, x, y, 0, 0);
}
void ImageRenderer::importImage(const std::string& path, const int& x, const int& y, const int& width, const int& height) {
	ofImage imageTemp;
	if (!imageTemp.load((boost::filesystem::path)path)) {
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
	lstImages.push_back(ObjectBase2D<ofImage>(x, y, widthToUse, heightToUse, imageTemp));
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

void ImageRenderer::exportImage(const std::string& path, const std::string& fileName) const {
	ofImage imageToExport;
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

void ImageRenderer::generateDraw() {
}

void ImageRenderer::render() {
	for (const ObjectBase2D<ofImage>& objetBase : lstImages) {
		const ofImage image = objetBase.getObject();
		const Coords2D coords = objetBase.getCoords();
		if (image.isAllocated()) {
			image.draw(coords.origine.x, coords.origine.y, coords.width, coords.height);
		}
	}
}
void ImageRenderer::drawImage() {
	render();
}
