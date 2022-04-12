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
    parameters.add(mesh_sphere_toggle.setup("Afficher la planete", false)->getParameter()); 
    parameters.add(mesh_square_toggle.setup("Relief de la planete", false)->getParameter());    
    parameters.add(display.setup("Texture planete", false)->getParameter());
    parameters.add(tone_map_toggle.setup("Tone map", true)->getParameter());
    parameters.add(slider_exposure.set("Exposure",1.0f, 0.0f, 5.0f));
    parameters.add(slider_gamma.set("Gamma", 2.2f, 0.0f, 5.0f)); 
  

    // panel pour choisir la planete
    parameters_planet.setName("Systeme solaire"); 

	// boutons pour le planet picker 
    mercure_button.addListener(this, &TextureRenderer::buttonMercurePicker);
    parameters_planet.add(mercure_button.setup("Mercure")->getParameter());
    venus_button.addListener(this, &TextureRenderer::buttonVenusPicker);
    parameters_planet.add(venus_button.setup("Venus")->getParameter());
    terre_button.addListener(this, &TextureRenderer::buttonTerrePicker);
    parameters_planet.add(terre_button.setup("Terre")->getParameter()); 
    mars_button.addListener(this, &TextureRenderer::buttonMarsPicker);
    parameters_planet.add(mars_button.setup("Mars")->getParameter());    
    jupiter_button.addListener(this, &TextureRenderer::buttonJupiterPicker);
    parameters_planet.add(jupiter_button.setup("Jupiter")->getParameter()); 
    saturn_button.addListener(this, &TextureRenderer::buttonSaturnPicker);
    parameters_planet.add(saturn_button.setup("Saturne")->getParameter());
    uranus_button.addListener(this, &TextureRenderer::buttonUranusPicker);
    parameters_planet.add(uranus_button.setup("Uranus")->getParameter());
    neptune_button.addListener(this, &TextureRenderer::buttonNeptunePicker);
    parameters_planet.add(neptune_button.setup("Neptune")->getParameter());
    pluton_button.addListener(this, &TextureRenderer::buttonPlutonPicker);
    parameters_planet.add(pluton_button.setup("Pluton")->getParameter());    

    // slider pour le square mesh 
    map_mesh.setName("Effet de relief");   
    map_mesh.add(slider_3.set("Vagues", 5.0f, 0.0f, 100.0f));
    map_mesh.add(slider_4.set("Rotation", 0.0f, 0.0f, 420.0f));  
   
    // loader image de départ                                           
    image.load("images/earth.jpg"); 

    // assigner l'image originale a une variable pour conserver son état d'origine
    image_selection = image; 

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
    
	// creer un mesh a partir d'une sphere 
	sphere_mesh = ofSpherePrimitive(200, 40).getMesh();
	for (int i = 0; i < sphere_mesh.getVertices().size(); i++) {
		ofVec2f texCoord = sphere_mesh.getTexCoord(i);
		texCoord.x *= image.getWidth();
		texCoord.y = (1.0 - texCoord.y) * image.getHeight();
		sphere_mesh.setTexCoord(i, texCoord);
	}	

    // creer et initialiser un mesh square a partir de vertex 
    grid_width = 100;
    grid_height = 100; 
    mesh_size = 6; 
 
    // initialiser et assigner les vertices
    for (int y = 0; y < grid_height; y++) {
        for (int x = 0; x < grid_width; x++) {
            square_mesh.addVertex(ofPoint((x - grid_width / 2) * mesh_size, (y - grid_width / 2) * mesh_size, 0)); 
            square_mesh.addTexCoord(glm::vec2(x * (image_width / grid_width), y * (image_height / grid_height)));
            square_mesh.addColor(ofColor(255, 255, 255));
        }
    }

    // setup des triangles 
    for (int y = 0; y < grid_height - 1; y++) {
        for (int x = 0; x < grid_width - 1; x++) {
            int i1 = x + grid_width * y;
            int i2 = x + 1 + grid_width * y;
            int i3 = x + grid_width * (y + 1);
            int i4 = x + 1 + grid_width * (y + 1);

            square_mesh.addTriangle(i1, i2, i3);
            square_mesh.addTriangle(i2, i4, i3);
        }
    }

    // load du shader pour le tone mapping
    shader_tone_map.load("shader/tone_mapping_330_vs.glsl", "shader/tone_mapping_330_fs.glsl");

   
}

void TextureRenderer::updateRenderer() {

    // change vertices pour le square mesh
    for (int y = 0; y < grid_width; y++) {
        for (int x = 0; x < grid_height; x++) {

            // vertex index
            int i = x + grid_width * y;
            ofPoint p = square_mesh.getVertex(i);

            // change z-coordinate pour vertex
            p.z = ofNoise(x * 0.05, y * 0.05, ofGetElapsedTimef() * 0.5) * slider_3;
            square_mesh.setVertex(i, p);

            // change color pour vertex
            square_mesh.setColor(i, ofColor(255, 255, 255));
        }
    }   
}

void TextureRenderer::generateDraw() { }

void TextureRenderer::render() { 

    // faire apparaitre la sphere ou l'image
    if (display) ofDisableArbTex();
    else ofEnableArbTex();
          
    // si on veut afficher la sphere sur laquelle les textures sont appliquées 
    if (mesh_sphere_toggle) {
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
	    ofEnableDepthTest(); 
	    cam_tex.begin();
	    ofPushMatrix();
        ofRotate(ofGetElapsedTimef() * 10, 0, 1, 0);
        image = image_destination;    
	    image.bind();
        if (!display) sphere_mesh.draw(); 
	    ofPopMatrix();
	    cam_tex.end();
    } 

    // si on veut afficher le square mesh sur lequel l'image sera affiché
    if (mesh_square_toggle) {        
        cam_tex.begin();
        ofPushMatrix();
        ofRotate(slider_4);
        image.bind();
        square_mesh.draw();
        ofPopMatrix();
        cam_tex.end();
    }
    
    // afficher les instructions de controle pour le filtre de convolution 
    if (mesh_sphere_toggle || mesh_square_toggle) {        
        ofSetColor(200);
        string msg = "i: image originale\ne: filtre emboss\ns: filtre sharpen\nd: filtre edge detect";
        ofDrawBitmapString(msg, 400, 20);
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

    image.load("images/mars.jpg");   
    image.update();    
    image_selection = image; 
    image_selection.update();
    image_width = image.getWidth();
    image_height = image.getHeight();
    image_destination.allocate(image_width, image_height, OF_IMAGE_COLOR);
    image_destination.update();       
    filter();
}

void TextureRenderer::buttonVenusPicker() {

    image.load("images/venus.jpg");  
    image.resize(500, 300);
    image.update();
    image_selection = image;
    image_selection.update();
    image_width = image.getWidth();
    image_height = image.getHeight();
    image_destination.allocate(image_width, image_height, OF_IMAGE_COLOR);
    image_destination.update();
    filter();
}

void TextureRenderer::buttonTerrePicker() {

    image.load("images/earth.jpg"); 
    image.update();
    image_selection = image;
    image_selection.update();
    image_width = image.getWidth();
    image_height = image.getHeight();
    image_destination.allocate(image_width, image_height, OF_IMAGE_COLOR);
    image_destination.update();
    filter();
}

void TextureRenderer::buttonJupiterPicker() {

    image.load("images/jupiter.jpg");
    image.resize(500, 300);
    image.update();
    image_selection = image;
    image_selection.update();
    image_width = image.getWidth();
    image_height = image.getHeight();
    image_destination.allocate(image_width, image_height, OF_IMAGE_COLOR);
    image_destination.update();
    filter();
}

void TextureRenderer::buttonSaturnPicker() {

    image.load("images/saturn.jpg");
    image.resize(500, 300);
    image.update();
    image_selection = image;
    image_selection.update();
    image_width = image.getWidth();
    image_height = image.getHeight();
    image_destination.allocate(image_width, image_height, OF_IMAGE_COLOR);
    image_destination.update();
    filter();
}

void TextureRenderer::buttonMercurePicker() {

    image.load("images/mercure.jpg");
    image.resize(500, 250);
    image.update();
    image_selection = image;
    image_selection.update();
    image_width = image.getWidth();
    image_height = image.getHeight();
    image_destination.allocate(image_width, image_height, OF_IMAGE_COLOR);
    image_destination.update();
    filter();
}

void TextureRenderer::buttonUranusPicker() {

    image.load("images/uranus.jpg");
    image.resize(500, 250);
    image.update();
    image_selection = image;
    image_selection.update();
    image_width = image.getWidth();
    image_height = image.getHeight();
    image_destination.allocate(image_width, image_height, OF_IMAGE_COLOR);
    image_destination.update();
    filter();

}

void TextureRenderer::buttonNeptunePicker() {

    image.load("images/neptune.jpg");
    image.resize(500, 250);
    image.update();
    image_selection = image;
    image_selection.update();
    image_width = image.getWidth();
    image_height = image.getHeight();
    image_destination.allocate(image_width, image_height, OF_IMAGE_COLOR);
    image_destination.update();
    filter();
}

void TextureRenderer::buttonPlutonPicker() {

    image.load("images/pluton.jpg");
    image.resize(504, 252);
    image.update();
    image_selection = image;
    image_selection.update();
    image_width = image.getWidth();
    image_height = image.getHeight();
    image_destination.allocate(image_width, image_height, OF_IMAGE_COLOR);
    image_destination.update();
    filter();
}

void TextureRenderer::keyReleased(int key) {
    if (mesh_sphere_toggle) {
        image = image_selection; // permet de revenir a l'image d'origine et ne pas emplier les filtres 

        switch (key) {

        case 'i': // touche 1 
            kernel_type = ConvolutionKernel::identity;
            kernel_name = "identity";
            break;

        case 'e': // touche 2
            kernel_type = ConvolutionKernel::emboss;
            kernel_name = "emboss";
            break;

        case 's': // touche 3
            kernel_type = ConvolutionKernel::sharpen;
            kernel_name = "sharpen";
            break;

        case 'd': // touche 4
            kernel_type = ConvolutionKernel::edge_detect;
            kernel_name = "edge_detect";
            break;

        default:
            break;

        }

    
   
    // appliquer le filtre de convolution 
    filter();
    }
}




