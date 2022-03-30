/**
 * \IFT3100H21 
 * \file topologieRenderer.cpp
 * \author Stéphane Boulanger
 * \brief Classe responsable de la topologie de l'application
 * \version 0.1
 * \date 2022-03-27
 */


#include "topologieRenderer.h"

void TopologieRenderer::setupRenderer(const std::string& name) {

	parameters.clear();
	parameters.setName(name);

   // position de la caméra 
    cam.setPosition(ofPoint(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 1500));

    // menu gui pour la topologie
    parameters.add(bezier_toggle.setup("Spline de bezier", false)->getParameter());

    ofSetFrameRate(60);
    ofSetBackgroundColor(0);
    ofSetSphereResolution(32);
    ofDisableDepthTest();
    
    is_key_press_up = false;
    is_key_press_down = false;
    is_key_press_left = false;
    is_key_press_right = false;

    // paramètres
    line_resolution = 100;
    line_width_outline = 4.0f;
    line_width_curve = 8.0f;
    radius = 32.0f;
    scale = 10.0f;
    offset = 64.0f;
    motion_speed = 250.0f;

    // initialisation des sommets de la ligne
    for (index = 0; index <= line_resolution; ++index)
        line_renderer.addVertex(ofPoint());
    for (index_2 = 0; index_2 <= line_resolution; ++index_2)
        line_renderer_2.addVertex(ofPoint());

    // initialisation de la scène
    reset();

}

void TopologieRenderer::updateRenderer() {

  
    if (same_point_4) ctrl_point5 = ctrl_point4;
    if (same_point_5) ctrl_point4 = ctrl_point5;   

    time_current = ofGetElapsedTimef();
    time_elapsed = time_current - time_last;
    time_last = time_current;

    if (is_key_press_up)
        selected_ctrl_point->y -= delta_y * time_elapsed;
    if (is_key_press_down)
        selected_ctrl_point->y += delta_y * time_elapsed;
    if (is_key_press_left)
        selected_ctrl_point->x -= delta_x * time_elapsed;
    if (is_key_press_right)
        selected_ctrl_point->x += delta_x * time_elapsed;

    
    for (index = 0; index <= line_resolution; ++index) {

        bezier_cubic(
            index / (float)line_resolution,
            ctrl_point1.x, ctrl_point1.y, ctrl_point1.z,
            ctrl_point2.x, ctrl_point2.y, ctrl_point2.z,
            ctrl_point3.x, ctrl_point3.y, ctrl_point3.z,
            ctrl_point4.x, ctrl_point4.y, ctrl_point4.z,
            position.x, position.y, position.z);

        bezier_cubic_2(
            index / (float)line_resolution,
            ctrl_point5.x, ctrl_point5.y, ctrl_point5.z,
            ctrl_point6.x, ctrl_point6.y, ctrl_point6.z,
            ctrl_point7.x, ctrl_point7.y, ctrl_point7.z,
            ctrl_point8.x, ctrl_point8.y, ctrl_point8.z,
            position_2.x, position_2.y, position_2.z);


        // affecter les positions des points sur la courbe
        line_renderer[index] = position;
        line_renderer_2[index] = position_2;
    }
}

void TopologieRenderer::generateDraw() {
}

void TopologieRenderer::render() {


    if (bezier_toggle) {
        cam.begin();
        cam.setVFlip(true);

        ofFill();

        // dessiner les positions initiales
        ofSetColor(63, 63, 63);

        //ofDrawEllipse(initial_position1.x, initial_position1.y, radius / 2.0f, radius / 2.0f);
       // ofDrawEllipse(initial_position2.x, initial_position2.y, radius / 2.0f, radius / 2.0f);
        //ofDrawEllipse(initial_position3.x, initial_position3.y, radius / 2.0f, radius / 2.0f);
        //ofDrawEllipse(initial_position4.x, initial_position4.y, radius / 2.0f, radius / 2.0f);
        //ofDrawEllipse(initial_position5.x, initial_position5.y, radius / 2.0f, radius / 2.0f);

        // dessiner la ligne contour
        ofSetColor(0, 0, 255);
        ofSetLineWidth(line_width_outline);
        ofDrawLine(ctrl_point1.x, ctrl_point1.y, ctrl_point2.x, ctrl_point2.y);
        ofDrawLine(ctrl_point3.x, ctrl_point3.y, ctrl_point4.x, ctrl_point4.y);
        ofDrawLine(ctrl_point2.x, ctrl_point2.y, ctrl_point3.x, ctrl_point3.y);
        ofDrawLine(ctrl_point4.x, ctrl_point4.y, ctrl_point1.x, ctrl_point1.y);


        // dessiner la courbe originale 
        ofSetColor(0, 255, 0);
        ofSetLineWidth(line_width_curve);
        line_renderer.draw();


        // dessiner les points de contrôle
        ofSetColor(255, 0, 0);
        ofDrawEllipse(ctrl_point1.x, ctrl_point1.y, radius, radius);
        ofSetColor(200);
        string e1 = "1";
        ofDrawBitmapString(e1, ctrl_point1.x -7, ctrl_point1.y+7);

        ofSetColor(255, 0, 0);
        ofDrawEllipse(ctrl_point2.x, ctrl_point2.y, radius, radius);
        ofSetColor(200);
        string e2 = "2";
        ofDrawBitmapString(e2, ctrl_point2.x -9, ctrl_point2.y+7);

        ofSetColor(255, 0, 0);
        ofDrawEllipse(ctrl_point3.x, ctrl_point3.y, radius, radius);
        ofSetColor(200);
        string e3 = "3";
        ofDrawBitmapString(e3, ctrl_point3.x - 9, ctrl_point3.y + 7);

        //ofSetColor(255, 0, 0);
        //ofDrawEllipse(ctrl_point4.x, ctrl_point4.y, radius, radius);
        //ofSetColor(200);
        //string e4 = "4";
        //ofDrawBitmapString(e4, ctrl_point4.x - 9, ctrl_point4.y + 7);
       
       

        //----------------------------------------------------------------------------------------------------------

         // dessiner les positions initiales
        ofSetColor(63, 63, 63);

        //ofDrawEllipse(initial_position6.x + 770, initial_position6.y, radius / 2.0f, radius / 2.0f);
        //ofDrawEllipse(initial_position7.x + 770, initial_position7.y, radius / 2.0f, radius / 2.0f);
        //ofDrawEllipse(initial_position8.x+ 770, initial_position8.y, radius / 2.0f, radius / 2.0f);
        //ofDrawEllipse(initial_position9.x + 770, initial_position9.y, radius / 2.0f, radius / 2.0f);
        //ofDrawEllipse(initial_position10.x + 770, initial_position10.y, radius / 2.0f, radius / 2.0f);

        // dessiner la ligne contour
        ofSetColor(0, 0, 255);
        ofSetLineWidth(line_width_outline);
        ofDrawLine(ctrl_point5.x, ctrl_point5.y, ctrl_point6.x, ctrl_point6.y);
        ofDrawLine(ctrl_point7.x, ctrl_point7.y, ctrl_point8.x, ctrl_point8.y);
        ofDrawLine(ctrl_point6.x, ctrl_point6.y, ctrl_point7.x, ctrl_point7.y);
        ofDrawLine(ctrl_point8.x, ctrl_point8.y, ctrl_point5.x, ctrl_point5.y);

        // dessiner la courbe originale 
        ofSetColor(0, 255, 0);
        ofSetLineWidth(line_width_curve);
        line_renderer_2.draw();


        // dessiner les points de contrôle
        ofSetColor(255, 0, 0);
        ofDrawEllipse(ctrl_point5.x, ctrl_point5.y, radius, radius);
        ofSetColor(200);
        string e5 = "4";
        ofDrawBitmapString(e5, ctrl_point5.x-9, ctrl_point5.y+7);

        ofSetColor(255, 0, 0);
        ofDrawEllipse(ctrl_point6.x, ctrl_point6.y, radius, radius);
        ofSetColor(200);
        string e6 = "5";
        ofDrawBitmapString(e6, ctrl_point6.x - 9, ctrl_point6.y + 7);

        ofSetColor(255, 0, 0);
        ofDrawEllipse(ctrl_point7.x, ctrl_point7.y, radius, radius);
        ofSetColor(200);
        string e7 = "6";
        ofDrawBitmapString(e7, ctrl_point7.x - 9, ctrl_point7.y + 7);

        ofSetColor(255, 0, 0);
        ofDrawEllipse(ctrl_point8.x, ctrl_point8.y, radius, radius);
        ofSetColor(200);
        string e8 = "7";
        ofDrawBitmapString(e8, ctrl_point8.x - 9, ctrl_point8.y + 7);

        //----------------------------------------------------------------------------------------------------------

        cam.end();

        // draw instruction de controle des points pour l'utilisateur 
        ofSetColor(200);
        string msg = "Utiliser les touches 1 a 7 pour choisir un point de controle\net les fleches pour les deplacer\nr:reset";
        ofDrawBitmapString(msg, 400, 20);

    }
}


void TopologieRenderer::reset() {

    // initialisation des variables
    framebuffer_width = ofGetWidth();
    framebuffer_height = ofGetHeight();

    // ratios de positionnement dans la fenêtre
    float w_1_8 = framebuffer_width / 8.0f;
    float w_1_4 = framebuffer_width / 4.0f;
    float w_1_2 = framebuffer_width / 2.0f;
    float w_3_4 = framebuffer_width * 3.0f / 4.0f;
    float w_7_8 = framebuffer_width * 7.0f / 8.0f;
    float h_1_5 = framebuffer_height / 5.0f;
    float h_1_3 = framebuffer_height / 3.0f;
    float h_4_5 = framebuffer_height * 4.0f / 5.0f;

    initial_position1 = { w_1_8, h_4_5, 0 };
    initial_position2 = { w_1_4, h_1_3, 0 };
    initial_position3 = { w_1_2, h_1_5, 0 };
    initial_position4 = { w_3_4, h_1_3, 0 };
    initial_position5 = { w_7_8, h_4_5, 0 };

    initial_position6 = { w_1_8, h_4_5, 0 };
    initial_position7 = { w_1_4, h_1_3, 0 };
    initial_position8 = { w_1_2, h_1_5, 0 };
    initial_position9 = { w_3_4, h_1_3, 0 };
    initial_position10 = { w_7_8, h_4_5, 0 };



    // paramètres des points de controles 
    ctrl_point1 = initial_position1;
    ctrl_point2 = initial_position2;
    ctrl_point3 = initial_position4;
    ctrl_point4 = initial_position5;

    selected_ctrl_point = &ctrl_point2;

    ctrl_point5 = ctrl_point4;

    ctrl_point6.x = ctrl_point2.x + 770;
    ctrl_point6.y = ctrl_point2.y + 770;
    ctrl_point6.z = ctrl_point2.z;

    ctrl_point7.x = ctrl_point3.x + 770;
    ctrl_point7.y = ctrl_point3.y;
    ctrl_point7.z = ctrl_point3.z;

    ctrl_point8.x = ctrl_point4.x + 770;
    ctrl_point8.y = ctrl_point4.y;
    ctrl_point8.z = ctrl_point4.z;

    delta_x = motion_speed;
    delta_y = motion_speed;


    ofLog() << "<reset>";
}

void TopologieRenderer::keyPressed(int key) {
    switch (key)
    {
    case OF_KEY_LEFT: // touche ←
        is_key_press_left = true;
        break;

    case OF_KEY_UP: // touche ↑
        is_key_press_up = true;
        break;

    case OF_KEY_RIGHT: // touche →
        is_key_press_right = true;
        break;

    case OF_KEY_DOWN: // touche ↓
        is_key_press_down = true;
        break;

    default:
        break;
    }
}

void TopologieRenderer::keyReleased(int key) {
    switch (key)
    {
    case 48: // touche 0
        ofLog() << "<0>";
        break;

    case 49: // touche 1
        selected_ctrl_point = &ctrl_point1;
        ofLog() << "<select control point 1>";
        break;

    case 50: // touche 2
        selected_ctrl_point = &ctrl_point2;
        ofLog() << "<select control point 2>";
        break;

    case 51: // touche 3
        selected_ctrl_point = &ctrl_point3;
        ofLog() << "<select control point 3>";
        break;

    case 52: // touche 4
        selected_ctrl_point = &ctrl_point4;
        same_point_4 = true;
        same_point_5 = false;
        ofLog() << "<select control point 4>";
        break;
    case 53: // touche 5
        //selected_ctrl_point = &ctrl_point5;
        //same_point_4 = false;
        //same_point_5 = true;
        selected_ctrl_point = &ctrl_point6;
        ofLog() << "<select control point 5>";
        break;
    case 54: // touche 5
        selected_ctrl_point = &ctrl_point7;
        ofLog() << "<select control point 6>";
        break;
    case 55: // touche 5
        selected_ctrl_point = &ctrl_point8;
        ofLog() << "<select control point 7>";
        break;
    //case 56: // touche 8
        //selected_ctrl_point = &ctrl_point8;
       // ofLog() << "<select control point 5>";
       // break;



    case 114: // touche r
        reset();
        break;

    case OF_KEY_LEFT: // touche ←
        is_key_press_left = false;
        break;

    case OF_KEY_UP: // touche ↑
        is_key_press_up = false;
        break;

    case OF_KEY_RIGHT: // touche →
        is_key_press_right = false;
        break;

    case OF_KEY_DOWN: // touche ↓
        is_key_press_down = false;
        break;

    default:
        break;
    }
}
