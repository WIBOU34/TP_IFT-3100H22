// IFT3100H21 
// Classe responsable des testures de l'application.

#include "textureRenderer.h"

void TextureRenderer::setupRenderer(const std::string& name) {
	parameters.clear();
	parameters.setName(name);

    // menu gui pour la texture   
    parameters.add(mesh_sphere_toggle.setup("Sphere mesh", true)->getParameter());
    parameters.add(identite_label.setup("Image originale    ", "' 1 '")->getParameter());
    parameters.add(emboss_label.setup("Filtre emboss      ", "' 2 '")->getParameter());
    parameters.add(sharpen_label.setup("Filtre sharpen     ", "' 3 '")->getParameter());
    parameters.add(edge_detect_label.setup("Filtre edge detect ", "' 4 '")->getParameter());
    
    
	// loader image 
	image.load("fire.jpg");
    // assigner l'image originale a une variable pour conserver son �tat d'origine
    image_selection = image; 
    
	// creer un mesh a partir d'une sphere 
	mesh = ofSpherePrimitive(200, 40).getMesh();
	for (int i = 0; i < mesh.getVertices().size(); i++) {
		ofVec2f texCoord = mesh.getTexCoord(i);
		texCoord.x *= image.getWidth();
		texCoord.y = (1.0 - texCoord.y) * image.getHeight();
		mesh.setTexCoord(i, texCoord);
	}

	// load des shaders 
	shader.load("shader/frag_150.glsl", "shader/geom_150.glsl", "shader/vert_150.glsl");

	// dimensions de l'image source
	image_width = image.getWidth();
	image_height = image.getHeight();
	// initialiser l'image de destination
	image_destination.allocate(image_width, image_height, OF_IMAGE_COLOR);	
    // s�lectionner le filtre de convolution par d�faut   
    kernel_type = ConvolutionKernel::identity;
	kernel_name = "identity";   
	// appliquer le filtre de convolution par d�faut
	filter();
}

void TextureRenderer::updateRenderer() {
 
}

void TextureRenderer::generateDraw() {

}

void TextureRenderer::render() {

	
    if (mesh_sphere_toggle == true) {

    
    // enable z-buffering 
	ofEnableDepthTest();

    image = image_destination;

	cam.begin();
	//light.enable();
	ofPushMatrix();
    ofRotate(ofGetElapsedTimef() * 10, 0, 1, 0);
	

	image.bind();
	//mesh.drawWireframe();
	mesh.draw();

	ofPopMatrix();
	//light.disable();
	cam.end();

    
    }    
}

void TextureRenderer::filter()
{
    // r�solution du kernel de convolution
    const int kernel_size = 3;

    // d�calage � partir du centre du kernel
    const int kernel_offset = kernel_size / 2;

    // nombre de composantes de couleur (RGB)
    const int color_component_count = 3;

    // indices de l'image
    int x, y;

    // indices du kernel
    int i, j;

    // index des composantes de couleur
    int c;

    // index du pixel de l'image source utilis� pour le filtrage
    int pixel_index_img_src;

    // index du pixel de l'image de destination en cours de filtrage
    int pixel_index_img_dst;

    // index du pixel de l'image de destination en cours de filtrage
    int kernel_index;

    // valeur � un des indices du kernel de convolution
    float kernel_value;

    // extraire les pixels de l'image source
    ofPixels pixel_array_src = image.getPixels();

    // extraire les pixels de l'image de destination
    ofPixels pixel_array_dst = image_destination.getPixels();

    // couleur du pixel lu dans l'image source
    ofColor pixel_color_src;

    // couleur du pixel � �crire dans l'image de destination
    ofColor pixel_color_dst;

    // somme du kernel appliqu�e � chaque composante de couleur d'un pixel
    float sum[color_component_count];

    // it�ration sur les rang�es des pixels de l'image source
    for (y = 0; y < image_height; ++y)
    {
        // it�ration sur les colonnes des pixels de l'image source
        for (x = 0; x < image_width; ++x)
        {
            // initialiser le tableau o� les valeurs de filtrage sont accumul�es
            for (c = 0; c < color_component_count; ++c)
                sum[c] = 0;

            // d�terminer l'index du pixel de l'image de destination
            pixel_index_img_dst = (image_width * y + x) * color_component_count;

            // it�ration sur les colonnes du kernel de convolution
            for (j = -kernel_offset; j <= kernel_offset; ++j)
            {
                // it�ration sur les rang�es du kernel de convolution
                for (i = -kernel_offset; i <= kernel_offset; ++i)
                {
                    // d�terminer l'index du pixel de l'image source � lire
                    pixel_index_img_src = (image_width * (y - j) + (x - i)) * color_component_count;

                    // lire la couleur du pixel de l'image source
                    pixel_color_src = pixel_array_src.getColor(pixel_index_img_src);

                    // d�terminer l'indice du facteur � lire dans le kernel de convolution
                    kernel_index = kernel_size * (j + kernel_offset) + (i + kernel_offset);

                    // extraction de la valeur � cet index du kernel
                    switch (kernel_type)
                    {
                    
                    case ConvolutionKernel::emboss:
                        kernel_value = convolution_kernel_emboss.at(kernel_index);
                        break;

                    case ConvolutionKernel::sharpen:
                        kernel_value = convolution_kernel_sharpen.at(kernel_index);
                        break;

                    case ConvolutionKernel::edge_detect:
                        kernel_value = convolution_kernel_edge_detect.at(kernel_index);
                        break;
                
                    default:
                        kernel_value = convolution_kernel_identity.at(kernel_index);
                        break;
                    }

                    // it�ration sur les composantes de couleur
                    for (c = 0; c < color_component_count; ++c)
                    {
                        // accumuler les valeurs de filtrage en fonction du kernel de convolution
                        sum[c] = sum[c] + kernel_value * pixel_color_src[c];
                    }
                }
            }

            // d�terminer la couleur du pixel � partir des valeurs de filtrage accumul�es pour chaque composante
            for (c = 0; c < color_component_count; ++c)
            {
                // conversion vers entier et validation des bornes de l'espace de couleur
                pixel_color_dst[c] = (int)ofClamp(sum[c], 0, 255);
            }

            // �crire la couleur � l'index du pixel en cours de filtrage
            pixel_array_dst.setColor(pixel_index_img_dst, pixel_color_dst);
        }
    }

    // �crire les pixels dans l'image de destination
    image_destination.setFromPixels(pixel_array_dst);

    //ofLog() << "<convolution filter done>";
}

void TextureRenderer::keyReleased(int key) {
    
    image = image_selection; // permet de revenir a l'image d'origine et ne pas emplier les filtres 

    switch (key) {
        
    case 49: // touche 1 
        kernel_type = ConvolutionKernel::identity;
        kernel_name = "identity";
        break;

    case 50: // touche 2
        kernel_type = ConvolutionKernel::emboss;
        kernel_name = "emboss";
        break;

    case 51: // touche 3
        kernel_type = ConvolutionKernel::sharpen;
        kernel_name = "sharpen";
        break;

    case 52: // touche 4
        kernel_type = ConvolutionKernel::edge_detect;
        kernel_name = "edge_detect";        
        break;

    default:
        break;

    }
   
    
   
    // appliquer le filtre de convolution 
    filter();
}

void TextureRenderer::buttonSpherePressed()
{
}


