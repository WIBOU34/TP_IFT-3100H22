/**
 * \IFT3100H21 
 * \file topologieRenderer.cpp
 * \author Stéphane Boulanger
 * \brief Classe responsable de la topologie de l'application
 * \version 0.1
 * \date 2022-03-27
 */


#include "courbeBezierRenderer.h"

void CourbeBezierRenderer::setupRenderer(const std::string& name) {

	parameters.clear();
	parameters.setName(name);

   // position de la caméra 
    cam.setPosition(ofPoint(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 1500));

    // menu gui pour la topologie
    parameters.add(bezier_toggle.setup("Spline de bezier", false)->getParameter());

    //ofSetFrameRate(60);
    //ofSetBackgroundColor(0);    
    ofDisableDepthTest();
    
    is_key_press_up_topo = false;
    is_key_press_down_topo = false;
    is_key_press_left_topo = false;
    is_key_press_right_topo = false;

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

void CourbeBezierRenderer::updateRenderer() {
     
    ctrl_point5 = ctrl_point4;

    time_current = ofGetElapsedTimef();
    time_elapsed = time_current - time_last;
    time_last = time_current;

    if (is_key_press_up_topo) selected_ctrl_point->y -= delta_y * time_elapsed; 
    if (is_key_press_down_topo) selected_ctrl_point->y += delta_y * time_elapsed;
    if (is_key_press_left_topo) selected_ctrl_point->x -= delta_x * time_elapsed;
    if (is_key_press_right_topo) selected_ctrl_point->x += delta_x * time_elapsed;

    
    for (index = 0; index <= line_resolution; ++index) {

        bezier_cubic(
            index / (float)line_resolution,
            ctrl_point1.x, ctrl_point1.y, ctrl_point1.z,
            ctrl_point2.x, ctrl_point2.y, ctrl_point2.z,
            ctrl_point3.x, ctrl_point3.y, ctrl_point3.z,
            ctrl_point4.x, ctrl_point4.y, ctrl_point4.z,
            position.x, position.y, position.z);

        bezier_cubic(
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

void CourbeBezierRenderer::generateDraw() {
}

void CourbeBezierRenderer::render() {


    if (bezier_toggle) {
       
        cam.begin();
        cam.setVFlip(true);

        ofFill();

        // dessiner la ligne contour
        ofSetColor(0, 0, 255);
        if (hide_control_line) {
            ofDrawLine(ctrl_point1.x, ctrl_point1.y, ctrl_point2.x, ctrl_point2.y);
            ofDrawLine(ctrl_point3.x, ctrl_point3.y, ctrl_point4.x, ctrl_point4.y);
            ofDrawLine(ctrl_point2.x, ctrl_point2.y, ctrl_point3.x, ctrl_point3.y);
            ofDrawLine(ctrl_point4.x, ctrl_point4.y, ctrl_point1.x, ctrl_point1.y);
            ofDrawLine(ctrl_point5.x, ctrl_point5.y, ctrl_point6.x, ctrl_point6.y);
            ofDrawLine(ctrl_point7.x, ctrl_point7.y, ctrl_point8.x, ctrl_point8.y);
            ofDrawLine(ctrl_point6.x, ctrl_point6.y, ctrl_point7.x, ctrl_point7.y);
            ofDrawLine(ctrl_point8.x, ctrl_point8.y, ctrl_point5.x, ctrl_point5.y);
        }
        

        // dessiner les deux courbes  
        ofSetColor(0, 255, 0);
        line_renderer.draw();
        line_renderer_2.draw();

        // dessiner les points de contrôle
        ofSetColor(255, 0, 0);
        ofDrawEllipse(ctrl_point1.x, ctrl_point1.y, radius, radius);
        ofDrawEllipse(ctrl_point2.x, ctrl_point2.y, radius, radius);
        ofDrawEllipse(ctrl_point3.x, ctrl_point3.y, radius, radius);
        ofDrawEllipse(ctrl_point6.x, ctrl_point6.y, radius, radius);
        ofDrawEllipse(ctrl_point7.x, ctrl_point7.y, radius, radius);
        ofDrawEllipse(ctrl_point8.x, ctrl_point8.y, radius, radius);
        if (noeud) ofDrawEllipse(ctrl_point4.x, ctrl_point4.y, radius, radius);

        cam.end();

        // draw instruction de controle des points pour l'utilisateur 
        ofSetColor(200);
        string msg = "Utiliser les touches 1 a 6 pour choisir un point de controle\n"
                     "et les fleches pour les deplacer\nr : reset\nc : afficher les lignes de controle\n"
                     "n : afficher et controler le noeud";
        ofDrawBitmapString(msg, 400, 20);

    }
}


void CourbeBezierRenderer::reset() {

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

    ctrl_point6.x = ctrl_point2.x + 1000;
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

void CourbeBezierRenderer::keyPressed(int key) {
    if (bezier_toggle) {
        switch (key)
        {
        case OF_KEY_LEFT: // touche ←
            is_key_press_left_topo = true;
            break;

        case OF_KEY_UP: // touche ↑
            is_key_press_up_topo = true;
            break;

        case OF_KEY_RIGHT: // touche →
            is_key_press_right_topo = true;
            break;

        case OF_KEY_DOWN: // touche ↓
            is_key_press_down_topo = true;
            break;

        default:
            break;
        }
    }
}

void CourbeBezierRenderer::keyReleased(int key) {
    if (bezier_toggle) {
        switch (key)
        {
        case 49: // touche 1
            selected_ctrl_point = &ctrl_point1;
            break;

        case 50: // touche 2
            selected_ctrl_point = &ctrl_point2;
            break;

        case 51: // touche 3
            selected_ctrl_point = &ctrl_point3;
            break;

        case 52: // touche 4
            selected_ctrl_point = &ctrl_point6;

            break;
        case 53: // touche 5
            selected_ctrl_point = &ctrl_point7;
            break;

        case 54: // touche 6
            selected_ctrl_point = &ctrl_point8;
            break;

        case 99: // touche c
            if (!hide_control_line) hide_control_line = true;
            else hide_control_line = false;
            break;

        case 110: // touche n
            if (!noeud) {
                noeud = true;
                selected_ctrl_point = &ctrl_point4;
            }
            else {
                noeud = false;
                selected_ctrl_point = &ctrl_point2;
            }
            break;

        case 114: // touche r
            reset();
            break;

        case OF_KEY_LEFT: // touche ←
            is_key_press_left_topo = false;
            break;

        case OF_KEY_UP: // touche ↑
            is_key_press_up_topo = false;
            break;

        case OF_KEY_RIGHT: // touche →
            is_key_press_right_topo = false;
            break;

        case OF_KEY_DOWN: // touche ↓
            is_key_press_down_topo = false;
            break;

        default:
            break;
        }
    }
}
