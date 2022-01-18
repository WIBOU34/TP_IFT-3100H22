#include "imageRenderer.h"

void ImageRenderer::importImage(const std::string& path) {
	image.load((boost::filesystem::path)path);
	drawImage();
}

void ImageRenderer::exportImage(const std::string& path) {
	ofLog() << "<ImageRenderer::exportImage: Image exported to: " << path << ">";
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
