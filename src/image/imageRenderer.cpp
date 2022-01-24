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

void ImageRenderer::importImage(const std::string& path) {
	image.load((boost::filesystem::path)path);
	drawImage();
}

void ImageRenderer::exportImageDialog() {
	ofFileDialogResult dialogResult = ofSystemSaveDialog("ExportedImage.png", "Exporter image");
	if (dialogResult.bSuccess) {
		ofLog() << "<app::export: saving file>";
		exportImage(dialogResult.getPath(), dialogResult.getName());
	} else {
		ofLog() << "<app::export: ABORTED>";
	}
}

void ImageRenderer::exportImage(const std::string& path, const std::string& fileName) {
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
	if (image.isAllocated()) {
		image.draw(0, 0, image.getWidth(), image.getHeight());
	}
}
void ImageRenderer::drawImage() {
	render();
}
