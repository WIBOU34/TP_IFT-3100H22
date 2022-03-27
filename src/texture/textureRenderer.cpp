/**
 * \IFT3100H21
 * \file textureRenderer.cpp
 * \author Stéphane Boulanger
 * \brief Classe responsable des textures de l'application
 * \version 0.1
 * \date 2022-03-27
 */


#include "textureRenderer.h"

void TextureRenderer::setupRenderer(const std::string& name) {
	parameters.clear();
	parameters.setName(name);

    // menu gui pour la texture   
    parameters.add(mesh_sphere_toggle.setup("Sphere mesh", false)->getParameter());
    parameters.add(identite_label.setup("Image originale    ", "' 1 '")->getParameter());
    parameters.add(emboss_label.setup("Filtre emboss      ", "' 2 '")->getParameter());
    parameters.add(sharpen_label.setup("Filtre sharpen     ", "' 3 '")->getParameter());
    parameters.add(edge_detect_label.setup("Filtre edge detect ", "' 4 '")->getParameter());
    
    parameters.add(display.setup("Texture planete", false)->getParameter());
    parameters.add(tone_map_toggle.setup("Tone map", true)->getParameter());
    parameters.add(slider_exposure.set("Exposure",1.0f, 0.0f, 5.0f));
    parameters.add(slider_gamma.set("Gamma", 2.2f, 0.0f, 5.0f)); 
  

     // panel pour choisir la planete
    parameters_planet.setName("Voie lactée"); 

	// boutons pour le planet picker 
    mars_button.addListener(this, &TextureRenderer::buttonMarsPicker);
    parameters_planet.add(mars_button.setup("Mars")->getParameter());

    venus_button.addListener(this, &TextureRenderer::buttonVenusPicker);
    parameters_planet.add(venus_button.setup("Venus")->getParameter());

    terre_button.addListener(this, &TextureRenderer::buttonTerrePicker);
    parameters_planet.add(terre_button.setup("Terre")->getParameter());
    
   
    // loader image de départ                                           
    image.load("earth.jpg"); // https://www.wallpaperflare.com/teal-nebula-galaxy-wallpaper-planets-light-swirl-abstract-wallpaper-daa
    //mars.load("mars.jpg");
    //terre.load("venus.jpg");

    // assigner l'image originale a une variable pour conserver son état d'origine
    image_selection = image; 
    
	// creer un mesh a partir d'une sphere 
	mesh = ofSpherePrimitive(200, 40).getMesh();
	for (int i = 0; i < mesh.getVertices().size(); i++) {
		ofVec2f texCoord = mesh.getTexCoord(i);
		texCoord.x *= image.getWidth();
		texCoord.y = (1.0 - texCoord.y) * image.getHeight();
		mesh.setTexCoord(i, texCoord);
	}

	// load du shader  
    shader_tone_map.load("shader/tone_mapping_330_vs.glsl", "shader/tone_mapping_330_fs.glsl");

	// dimensions de l'image source
	image_width = image.getWidth();
	image_height = image.getHeight();
	// initialiser l'image de destination
	image_destination.allocate(image_width, image_height, OF_IMAGE_COLOR);	
    // sélectionner le filtre de convolution par défaut   
    kernel_type = ConvolutionKernel::identity;
	kernel_name = "identity";   
	// appliquer le filtre de convolution par défaut
	filter();

 
 
}

void TextureRenderer::updateRenderer() {
   

}

void TextureRenderer::generateDraw() {

}

void TextureRenderer::render() {   
    
    //planetPicker();
    
    // faire apparaitre la sphere ou l'image
    if (display) ofDisableArbTex();
    else ofEnableArbTex();
          
    // si on veut afficher la sphere sur laquelle les textures sont appliquées 
    if (mesh_sphere_toggle == true) {
                
        // pour sélectionner le type de tone-mapping
        if (tone_map_toggle) tone_map_toggle.setup("aces filmic", true)->getParameter();       
        else tone_map_toggle.setup("reinhard", false)->getParameter();    

        // shader pour mappage tonal 
        shader_tone_map.begin();
        shader_tone_map.setUniformTexture("image", image.getTexture(), 1);
        shader_tone_map.setUniform1f("tone_mapping_exposure", slider_exposure);
        shader_tone_map.setUniform1f("tone_mapping_gamma", slider_gamma);
        shader_tone_map.setUniform1f("tone_mapping_toggle", tone_map_toggle);   
        
        image.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
        shader_tone_map.end();        
               
        // dessiner la sphere de mesh avec son image en texture 
	    ofEnableDepthTest(); // enable z-buffering 

	    cam.begin();
	    ofPushMatrix();

        ofRotate(ofGetElapsedTimef() * 10, 0, 1, 0);
        image = image_destination;
    
	    image.bind();
	    if (!display) mesh.draw();
        
	    ofPopMatrix();
	    cam.end();
   
    }   


   
}

void TextureRenderer::filter()
{
    // résolution du kernel de convolution
    const int kernel_size = 3;

    // décalage à partir du centre du kernel
    const int kernel_offset = kernel_size / 2;

    // nombre de composantes de couleur (RGB)
    const int color_component_count = 3;

    // indices de l'image
    int x, y;

    // indices du kernel
    int i, j;

    // index des composantes de couleur
    int c;

    // index du pixel de l'image source utilisé pour le filtrage
    int pixel_index_img_src;

    // index du pixel de l'image de destination en cours de filtrage
    int pixel_index_img_dst;

    // index du pixel de l'image de destination en cours de filtrage
    int kernel_index;

    // valeur à un des indices du kernel de convolution
    float kernel_value;

    // extraire les pixels de l'image source
    ofPixels pixel_array_src = image.getPixels();

    // extraire les pixels de l'image de destination
    ofPixels pixel_array_dst = image_destination.getPixels();

    // couleur du pixel lu dans l'image source
    ofColor pixel_color_src;

    // couleur du pixel à écrire dans l'image de destination
    ofColor pixel_color_dst;

    // somme du kernel appliquée à chaque composante de couleur d'un pixel
    float sum[color_component_count];

    // itération sur les rangées des pixels de l'image source
    for (y = 0; y < image_height; ++y)
    {
        // itération sur les colonnes des pixels de l'image source
        for (x = 0; x < image_width; ++x)
        {
            // initialiser le tableau où les valeurs de filtrage sont accumulées
            for (c = 0; c < color_component_count; ++c)
                sum[c] = 0;

            // déterminer l'index du pixel de l'image de destination
            pixel_index_img_dst = (image_width * y + x) * color_component_count;

            // itération sur les colonnes du kernel de convolution
            for (j = -kernel_offset; j <= kernel_offset; ++j)
            {
                // itération sur les rangées du kernel de convolution
                for (i = -kernel_offset; i <= kernel_offset; ++i)
                {
                    // déterminer l'index du pixel de l'image source à lire
                    pixel_index_img_src = (image_width * (y - j) + (x - i)) * color_component_count;

                    // lire la couleur du pixel de l'image source
                    pixel_color_src = pixel_array_src.getColor(pixel_index_img_src);

                    // déterminer l'indice du facteur à lire dans le kernel de convolution
                    kernel_index = kernel_size * (j + kernel_offset) + (i + kernel_offset);

                    // extraction de la valeur à cet index du kernel
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

                    // itération sur les composantes de couleur
                    for (c = 0; c < color_component_count; ++c)
                    {
                        // accumuler les valeurs de filtrage en fonction du kernel de convolution
                        sum[c] = sum[c] + kernel_value * pixel_color_src[c];
                    }
                }
            }

            // déterminer la couleur du pixel à partir des valeurs de filtrage accumulées pour chaque composante
            for (c = 0; c < color_component_count; ++c)
            {
                // conversion vers entier et validation des bornes de l'espace de couleur
                pixel_color_dst[c] = (int)ofClamp(sum[c], 0, 255);
            }

            // écrire la couleur à l'index du pixel en cours de filtrage
            pixel_array_dst.setColor(pixel_index_img_dst, pixel_color_dst);
        }
    }

    // écrire les pixels dans l'image de destination
    image_destination.setFromPixels(pixel_array_dst);

    //ofLog() << "<convolution filter done>";
}

void TextureRenderer::buttonMarsPicker() {
    

    image.load("mars.jpg");
   
    image.update();
    
    image_selection = image; 
    image_selection.update();
    image_width = image.getWidth();
    image_height = image.getHeight();
    image_destination.allocate(image_width, image_height, OF_IMAGE_COLOR);
    image_destination.update();
       
    filter();
    std::cout << "--> BUTTON MARS CLICKED <--";


}

void TextureRenderer::buttonVenusPicker() {

    image.load("venus.jpg");
    
    image.update();

    image_selection = image;
    image_selection.update();
    image_width = image.getWidth();
    image_height = image.getHeight();
    image_destination.allocate(image_width, image_height, OF_IMAGE_COLOR);
    image_destination.update();

    filter();

    std::cout << "--> BUTTON VENUS CLICKED <--";

}

void TextureRenderer::buttonTerrePicker() {
    image.load("earth.jpg");
    image.update();

    image_selection = image;
    image_selection.update();
    image_width = image.getWidth();
    image_height = image.getHeight();
    image_destination.allocate(image_width, image_height, OF_IMAGE_COLOR);
    image_destination.update();

    filter();

    std::cout << "--> BUTTON EARTH CLICKED <--";
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




