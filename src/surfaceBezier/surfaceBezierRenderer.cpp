/**
 * \IFT3100H21 
 * \file surfaceBezierRenderer.cpp
 * \author Stéphane Boulanger
 * \brief Classe responsable de la surface de Bezier de l'application
 * \version 0.1
 * \date 2022-04-05
 */

#include "surfaceBezierRenderer.h"

void SurfaceBezierRenderer::setupRenderer(const std::string& name) {
	parameters.clear();
	parameters.setName(name);

    // menu gui pour la topologie
    parameters.add(surface_bezier_toggle.setup("Afficher la surface", false)->getParameter());

    // menu pour selectionner point de controle 
    parameters_point_controle.setName("Point"); 
    
    point_1.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle_1);
    parameters_point_controle.add(point_1.setup("Bas gauche")->getParameter());
    point_2.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle_2);
    parameters_point_controle.add(point_2.setup("Bas milieu gauche")->getParameter());
    point_3.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle_3);
    parameters_point_controle.add(point_3.setup("Bas milieu droit")->getParameter());
    point_4.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle_4);
    parameters_point_controle.add(point_4.setup("Bas droit")->getParameter());
    point_5.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle_5);
    parameters_point_controle.add(point_5.setup("Haut gauche")->getParameter());
    point_6.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle_6);
    parameters_point_controle.add(point_6.setup("Haut milieu gauche")->getParameter());
    point_7.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle_7);
    parameters_point_controle.add(point_7.setup("Haut milieu droit")->getParameter());
    point_8.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle_8);
    parameters_point_controle.add(point_8.setup("Haut droit")->getParameter());
    point_9.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle_9);
    parameters_point_controle.add(point_9.setup("Gauche milieu bas")->getParameter());
    point_10.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle_10);
    parameters_point_controle.add(point_10.setup("Gauche milieu haut")->getParameter());
    point_11.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle_11);
    parameters_point_controle.add(point_11.setup("Droit milieu bas")->getParameter());
    point_12.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle_12);
    parameters_point_controle.add(point_12.setup("Droit milieu haut")->getParameter());
    point_13.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle_13);
    parameters_point_controle.add(point_13.setup("Milieu haut gauche")->getParameter());
    point_14.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle_14);
    parameters_point_controle.add(point_14.setup("Milieu haut droit")->getParameter());
    point_15.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle_15);
    parameters_point_controle.add(point_15.setup("Milieu bas gauche")->getParameter());
    point_16.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle_16);
    parameters_point_controle.add(point_16.setup("Milieu bas droit")->getParameter());

    // initialisation des valeurs et paramètres de l'image sur la texture du mesh  
    ofDisableArbTex();
    image.load("paper.jpg");
    image.resize(400, 400);
    texture.clear();
    texture.allocate(400, 400, GL_RGB);
    texture.loadData(image.getPixels());

    // initialisation de width et height pour le mesh 
    width = 20;
    height = 20;
    
    // création du mesh 
    mainMesh = ofMesh::plane(400, 400, 4, 4);

    // parametres de l'affichage à l'ouverture de l'application    
    draw_wireframe = false;
    affiche_image = true;   
    is_key_press_up = false;
    is_key_press_down = false;
    is_key_press_left = false;
    is_key_press_right = false;

    // parametre pour set la position de la EasyCam 
    mainCam.setPosition(0, 0, 550);        
   
    // courbe de bezier 
    line_resolution = 20; 
    radius = 5.0f;    
    motion_speed = 40.0f;

    // initialisation des sommets des lignes
    for (index = 0; index <= line_resolution; ++index) {
        line_renderer_1.addVertex(ofPoint());
        line_renderer_2.addVertex(ofPoint());
        line_renderer_3.addVertex(ofPoint());
        line_renderer_4.addVertex(ofPoint());
        line_renderer_5.addVertex(ofPoint());
        line_renderer_6.addVertex(ofPoint());
        line_renderer_7.addVertex(ofPoint());
        line_renderer_8.addVertex(ofPoint()); 
    }

    // initialisation des vertex dans sa courbe de Bezier 
    vertex_bezier_1 = { 0, 1, 2, 3 };
    vertex_bezier_2 = { 12, 13, 14, 15 };
    vertex_bezier_3 = { 0, 4, 8, 12 };
    vertex_bezier_4 = { 3, 7, 11, 15 };
    vertex_bezier_5 = { 1, 5, 9, 13 };
    vertex_bezier_6 = { 2, 6, 10, 14 };
    vertex_bezier_7 = { 8, 9, 10, 11 };
    vertex_bezier_8 = { 4, 5, 6, 7 };

    // initialisation de la scène
    reset();
}

void SurfaceBezierRenderer::updateRenderer() {

    // courbe de bezier 
    time_current = ofGetElapsedTimef();
    time_elapsed = time_current - time_last;
    time_last = time_current;

    // unir les points de contoles communs
    ctrl_point9 = ctrl_point1;
    ctrl_point12 = ctrl_point5;
    ctrl_point16 = ctrl_point8;
    ctrl_point13 = ctrl_point4;
    ctrl_point2 = ctrl_point17;
    ctrl_point6 = ctrl_point20;
    ctrl_point3 = ctrl_point21;
    ctrl_point7 = ctrl_point24;
    ctrl_point11 = ctrl_point25;
    ctrl_point19 = ctrl_point26;
    ctrl_point23 = ctrl_point27;
    ctrl_point15 = ctrl_point28;
    ctrl_point10 = ctrl_point29;
    ctrl_point18 = ctrl_point30;
    ctrl_point22 = ctrl_point31;
    ctrl_point14 = ctrl_point32;   

    // assignation des key pours le mouvement des courbes 
    if (is_key_press_up) selected_ctrl_point->z += delta_y * time_elapsed; 
    if (is_key_press_down) selected_ctrl_point->z -= delta_y * time_elapsed;
    if (is_key_press_left) selected_ctrl_point->x -= delta_x * time_elapsed;
    if (is_key_press_right) selected_ctrl_point->x += delta_x * time_elapsed;

    // parametres des courbes  
    for (index = 0; index <= line_resolution; ++index) {
        bezier_cubic(
            index / (float)line_resolution,
            ctrl_point1.x, ctrl_point1.y, ctrl_point1.z,
            ctrl_point2.x, ctrl_point2.y, ctrl_point2.z,
            ctrl_point3.x, ctrl_point3.y, ctrl_point3.z,
            ctrl_point4.x, ctrl_point4.y, ctrl_point4.z,
            position_1.x, position_1.y, position_1.z);

        bezier_cubic(
            index / (float)line_resolution,
            ctrl_point5.x, ctrl_point5.y, ctrl_point5.z,
            ctrl_point6.x, ctrl_point6.y, ctrl_point6.z,
            ctrl_point7.x, ctrl_point7.y, ctrl_point7.z,
            ctrl_point8.x, ctrl_point8.y, ctrl_point8.z,
            position_2.x, position_2.y, position_2.z);

        bezier_cubic(
            index / (float)line_resolution,
            ctrl_point9.x, ctrl_point9.y, ctrl_point9.z,
            ctrl_point10.x, ctrl_point10.y, ctrl_point10.z,
            ctrl_point11.x, ctrl_point11.y, ctrl_point11.z,
            ctrl_point12.x, ctrl_point12.y, ctrl_point12.z,
            position_3.x, position_3.y, position_3.z);
        
        bezier_cubic(
            index / (float)line_resolution,
            ctrl_point13.x, ctrl_point13.y, ctrl_point13.z,
            ctrl_point14.x, ctrl_point14.y, ctrl_point14.z,
            ctrl_point15.x, ctrl_point15.y, ctrl_point15.z,
            ctrl_point16.x, ctrl_point16.y, ctrl_point16.z,
            position_4.x, position_4.y, position_4.z);

        bezier_cubic(
            index / (float)line_resolution,
            ctrl_point17.x, ctrl_point17.y, ctrl_point17.z,
            ctrl_point18.x, ctrl_point18.y, ctrl_point18.z,
            ctrl_point19.x, ctrl_point19.y, ctrl_point19.z,
            ctrl_point20.x, ctrl_point20.y, ctrl_point20.z,
            position_5.x, position_5.y, position_5.z);

        bezier_cubic(
            index / (float)line_resolution,
            ctrl_point21.x, ctrl_point21.y, ctrl_point21.z,
            ctrl_point22.x, ctrl_point22.y, ctrl_point22.z,
            ctrl_point23.x, ctrl_point23.y, ctrl_point23.z,
            ctrl_point24.x, ctrl_point24.y, ctrl_point24.z,
            position_6.x, position_6.y, position_6.z);

        bezier_cubic(
            index / (float)line_resolution,
            ctrl_point25.x, ctrl_point25.y, ctrl_point25.z,
            ctrl_point26.x, ctrl_point26.y, ctrl_point26.z,
            ctrl_point27.x, ctrl_point27.y, ctrl_point27.z,
            ctrl_point28.x, ctrl_point28.y, ctrl_point28.z,
            position_7.x, position_7.y, position_7.z);

        bezier_cubic(
            index / (float)line_resolution,
            ctrl_point29.x, ctrl_point29.y, ctrl_point29.z,
            ctrl_point30.x, ctrl_point30.y, ctrl_point30.z,
            ctrl_point31.x, ctrl_point31.y, ctrl_point31.z,
            ctrl_point32.x, ctrl_point32.y, ctrl_point32.z,
            position_8.x, position_8.y, position_8.z);

        bezier_cubic(
            index / (float)line_resolution,
            ctrl_point33.x, ctrl_point33.y, ctrl_point33.z,
            ctrl_point34.x, ctrl_point34.y, ctrl_point34.z,
            ctrl_point35.x, ctrl_point35.y, ctrl_point35.z,
            ctrl_point36.x, ctrl_point36.y, ctrl_point36.z,
            position_9.x, position_9.y, position_9.z);


        // affecter les positions des points sur la courbe
        line_renderer_1[index] = position_1;
        line_renderer_2[index] = position_2;
        line_renderer_3[index] = position_3;
        line_renderer_4[index] = position_4;
        line_renderer_5[index] = position_5;
        line_renderer_6[index] = position_6;
        line_renderer_7[index] = position_7;
        line_renderer_8[index] = position_8;        
    }

    // affecter les positions des vertex sur la courbe 
    mainMesh.setVertex(vertex_bezier_1[0], line_renderer_1[0]);
    mainMesh.setVertex(vertex_bezier_1[1], line_renderer_1[7]);
    mainMesh.setVertex(vertex_bezier_1[2], line_renderer_1[14]);
    mainMesh.setVertex(vertex_bezier_1[3], line_renderer_1[20]);

    mainMesh.setVertex(vertex_bezier_2[0], line_renderer_2[0]);
    mainMesh.setVertex(vertex_bezier_2[1], line_renderer_2[7]);
    mainMesh.setVertex(vertex_bezier_2[2], line_renderer_2[14]);
    mainMesh.setVertex(vertex_bezier_2[3], line_renderer_2[20]);

    mainMesh.setVertex(vertex_bezier_3[0], line_renderer_3[0]);
    mainMesh.setVertex(vertex_bezier_3[1], line_renderer_3[7]);
    mainMesh.setVertex(vertex_bezier_3[2], line_renderer_3[14]);
    mainMesh.setVertex(vertex_bezier_3[3], line_renderer_3[20]);

    mainMesh.setVertex(vertex_bezier_4[0], line_renderer_4[0]);
    mainMesh.setVertex(vertex_bezier_4[1], line_renderer_4[7]);
    mainMesh.setVertex(vertex_bezier_4[2], line_renderer_4[14]);
    mainMesh.setVertex(vertex_bezier_4[3], line_renderer_4[20]);

    mainMesh.setVertex(vertex_bezier_5[0], line_renderer_5[0]);
    mainMesh.setVertex(vertex_bezier_5[1], line_renderer_5[7]);
    mainMesh.setVertex(vertex_bezier_5[2], line_renderer_5[14]);
    mainMesh.setVertex(vertex_bezier_5[3], line_renderer_5[20]);

    mainMesh.setVertex(vertex_bezier_6[0], line_renderer_6[0]);
    mainMesh.setVertex(vertex_bezier_6[1], line_renderer_6[7]);
    mainMesh.setVertex(vertex_bezier_6[2], line_renderer_6[14]);
    mainMesh.setVertex(vertex_bezier_6[3], line_renderer_6[20]);

    mainMesh.setVertex(vertex_bezier_7[0], line_renderer_7[0]);
    mainMesh.setVertex(vertex_bezier_7[1], line_renderer_7[7]);
    mainMesh.setVertex(vertex_bezier_7[2], line_renderer_7[13]);
    mainMesh.setVertex(vertex_bezier_7[3], line_renderer_7[20]);

    mainMesh.setVertex(vertex_bezier_8[0], line_renderer_8[0]);
    mainMesh.setVertex(vertex_bezier_8[1], line_renderer_8[7]);
    mainMesh.setVertex(vertex_bezier_8[2], line_renderer_8[13]);
    mainMesh.setVertex(vertex_bezier_8[3], line_renderer_8[20]);

}

void SurfaceBezierRenderer::generateDraw() { }

void SurfaceBezierRenderer::render() {

    if (surface_bezier_toggle) {
       
        ofFill();
        ofEnableDepthTest();
        mainCam.begin();
        texture.bind();
        ofSetColor(0, 255, 0);

        // draw mesh en wireframe, les vertex points ou l'image en texture 
        if (draw_wireframe) mainMesh.drawWireframe();
        else mainMesh.drawVertices();
        if (affiche_image) mainMesh.draw();

        // dessiner les lignes de contour
        if (control_line) {
            ofSetColor(0, 0, 255);
            ofDrawLine(ctrl_point1.x, ctrl_point1.y, ctrl_point1.z, ctrl_point2.x, ctrl_point2.y, ctrl_point2.z);
            ofDrawLine(ctrl_point3.x, ctrl_point3.y, ctrl_point3.z, ctrl_point4.x, ctrl_point4.y, ctrl_point4.z);
            ofDrawLine(ctrl_point2.x, ctrl_point2.y, ctrl_point2.z, ctrl_point3.x, ctrl_point3.y, ctrl_point3.z);
            ofDrawLine(ctrl_point4.x, ctrl_point4.y, ctrl_point4.z, ctrl_point1.x, ctrl_point1.y, ctrl_point1.z);

            ofDrawLine(ctrl_point5.x, ctrl_point5.y, ctrl_point5.z, ctrl_point6.x, ctrl_point6.y, ctrl_point6.z);
            ofDrawLine(ctrl_point7.x, ctrl_point7.y, ctrl_point7.z, ctrl_point8.x, ctrl_point8.y, ctrl_point8.z);
            ofDrawLine(ctrl_point6.x, ctrl_point6.y, ctrl_point6.z, ctrl_point7.x, ctrl_point7.y, ctrl_point7.z);
            ofDrawLine(ctrl_point8.x, ctrl_point8.y, ctrl_point8.z, ctrl_point5.x, ctrl_point5.y, ctrl_point5.z);

            ofDrawLine(ctrl_point9.x, ctrl_point9.y, ctrl_point9.z, ctrl_point10.x, ctrl_point10.y, ctrl_point10.z);
            ofDrawLine(ctrl_point11.x, ctrl_point11.y, ctrl_point11.z, ctrl_point12.x, ctrl_point12.y, ctrl_point12.z);
            ofDrawLine(ctrl_point10.x, ctrl_point10.y, ctrl_point10.z, ctrl_point11.x, ctrl_point11.y, ctrl_point11.z);
            ofDrawLine(ctrl_point12.x, ctrl_point12.y, ctrl_point12.z, ctrl_point9.x, ctrl_point9.y, ctrl_point9.z);

            ofDrawLine(ctrl_point13.x, ctrl_point13.y, ctrl_point13.z, ctrl_point14.x, ctrl_point14.y, ctrl_point14.z);
            ofDrawLine(ctrl_point15.x, ctrl_point15.y, ctrl_point15.z, ctrl_point16.x, ctrl_point16.y, ctrl_point16.z);
            ofDrawLine(ctrl_point14.x, ctrl_point14.y, ctrl_point14.z, ctrl_point15.x, ctrl_point15.y, ctrl_point15.z);
            ofDrawLine(ctrl_point16.x, ctrl_point16.y, ctrl_point16.z, ctrl_point13.x, ctrl_point13.y, ctrl_point13.z);

            ofDrawLine(ctrl_point17.x, ctrl_point17.y, ctrl_point17.z, ctrl_point18.x, ctrl_point18.y, ctrl_point18.z);
            ofDrawLine(ctrl_point19.x, ctrl_point19.y, ctrl_point19.z, ctrl_point20.x, ctrl_point20.y, ctrl_point20.z);
            ofDrawLine(ctrl_point18.x, ctrl_point18.y, ctrl_point18.z, ctrl_point19.x, ctrl_point19.y, ctrl_point19.z);
            ofDrawLine(ctrl_point20.x, ctrl_point20.y, ctrl_point20.z, ctrl_point17.x, ctrl_point17.y, ctrl_point17.z);

            ofDrawLine(ctrl_point21.x, ctrl_point21.y, ctrl_point21.z, ctrl_point22.x, ctrl_point22.y, ctrl_point22.z);
            ofDrawLine(ctrl_point23.x, ctrl_point23.y, ctrl_point23.z, ctrl_point24.x, ctrl_point24.y, ctrl_point24.z);
            ofDrawLine(ctrl_point22.x, ctrl_point22.y, ctrl_point22.z, ctrl_point23.x, ctrl_point23.y, ctrl_point23.z);
            ofDrawLine(ctrl_point24.x, ctrl_point24.y, ctrl_point24.z, ctrl_point21.x, ctrl_point21.y, ctrl_point21.z);

            ofDrawLine(ctrl_point25.x, ctrl_point25.y, ctrl_point25.z, ctrl_point26.x, ctrl_point26.y, ctrl_point26.z);
            ofDrawLine(ctrl_point27.x, ctrl_point27.y, ctrl_point27.z, ctrl_point28.x, ctrl_point28.y, ctrl_point28.z);
            ofDrawLine(ctrl_point26.x, ctrl_point26.y, ctrl_point26.z, ctrl_point27.x, ctrl_point27.y, ctrl_point27.z);
            ofDrawLine(ctrl_point28.x, ctrl_point28.y, ctrl_point28.z, ctrl_point25.x, ctrl_point25.y, ctrl_point25.z);

            ofDrawLine(ctrl_point29.x, ctrl_point29.y, ctrl_point29.z, ctrl_point30.x, ctrl_point30.y, ctrl_point30.z);
            ofDrawLine(ctrl_point31.x, ctrl_point31.y, ctrl_point31.z, ctrl_point32.x, ctrl_point32.y, ctrl_point32.z);
            ofDrawLine(ctrl_point30.x, ctrl_point30.y, ctrl_point30.z, ctrl_point31.x, ctrl_point31.y, ctrl_point31.z);
            ofDrawLine(ctrl_point32.x, ctrl_point32.y, ctrl_point32.z, ctrl_point29.x, ctrl_point29.y, ctrl_point29.z);
        }
        
        // dessiner la courbe ofPolyline
        if (bez_line) {
            ofSetColor(0, 255, 0);
            line_renderer_1.draw();
            line_renderer_2.draw();
            line_renderer_3.draw();
            line_renderer_4.draw();
            line_renderer_5.draw();
            line_renderer_6.draw();
            line_renderer_7.draw();
            line_renderer_8.draw();
        }

        // dessiner les points de contrôle
        ofSetColor(255, 0, 0);
        if (control_line) {
            ofDrawEllipse(ctrl_point1.x, ctrl_point1.y, ctrl_point1.z, radius, radius);
            ofDrawEllipse(ctrl_point2.x, ctrl_point2.y, ctrl_point2.z, radius, radius);
            ofDrawEllipse(ctrl_point3.x, ctrl_point3.y, ctrl_point3.z, radius, radius);
            ofDrawEllipse(ctrl_point4.x, ctrl_point4.y, ctrl_point4.z, radius, radius);

            ofDrawEllipse(ctrl_point5.x, ctrl_point5.y, ctrl_point5.z, radius, radius);
            ofDrawEllipse(ctrl_point6.x, ctrl_point6.y, ctrl_point6.z, radius, radius);
            ofDrawEllipse(ctrl_point7.x, ctrl_point7.y, ctrl_point7.z, radius, radius);
            ofDrawEllipse(ctrl_point8.x, ctrl_point8.y, ctrl_point8.z, radius, radius);

            ofDrawEllipse(ctrl_point9.x, ctrl_point9.y, ctrl_point9.z, radius, radius);
            ofDrawEllipse(ctrl_point10.x, ctrl_point10.y, ctrl_point10.z, radius, radius);
            ofDrawEllipse(ctrl_point11.x, ctrl_point11.y, ctrl_point11.z, radius, radius);
            ofDrawEllipse(ctrl_point12.x, ctrl_point12.y, ctrl_point12.z, radius, radius);

            ofDrawEllipse(ctrl_point13.x, ctrl_point13.y, ctrl_point13.z, radius, radius);
            ofDrawEllipse(ctrl_point14.x, ctrl_point14.y, ctrl_point14.z, radius, radius);
            ofDrawEllipse(ctrl_point15.x, ctrl_point15.y, ctrl_point15.z, radius, radius);
            ofDrawEllipse(ctrl_point16.x, ctrl_point16.y, ctrl_point16.z, radius, radius);

            ofDrawEllipse(ctrl_point17.x, ctrl_point17.y, ctrl_point17.z, radius, radius);
            ofDrawEllipse(ctrl_point18.x, ctrl_point18.y, ctrl_point18.z, radius, radius);
            ofDrawEllipse(ctrl_point19.x, ctrl_point19.y, ctrl_point19.z, radius, radius);
            ofDrawEllipse(ctrl_point20.x, ctrl_point20.y, ctrl_point20.z, radius, radius);

            ofDrawEllipse(ctrl_point21.x, ctrl_point21.y, ctrl_point21.z, radius, radius);
            ofDrawEllipse(ctrl_point22.x, ctrl_point22.y, ctrl_point22.z, radius, radius);
            ofDrawEllipse(ctrl_point23.x, ctrl_point23.y, ctrl_point23.z, radius, radius);
            ofDrawEllipse(ctrl_point24.x, ctrl_point24.y, ctrl_point24.z, radius, radius);

            ofDrawEllipse(ctrl_point25.x, ctrl_point25.y, ctrl_point25.z, radius, radius);
            ofDrawEllipse(ctrl_point26.x, ctrl_point26.y, ctrl_point26.z, radius, radius);
            ofDrawEllipse(ctrl_point27.x, ctrl_point27.y, ctrl_point27.z, radius, radius);
            ofDrawEllipse(ctrl_point28.x, ctrl_point28.y, ctrl_point28.z, radius, radius);

            ofDrawEllipse(ctrl_point29.x, ctrl_point29.y, ctrl_point29.z, radius, radius);
            ofDrawEllipse(ctrl_point30.x, ctrl_point30.y, ctrl_point30.z, radius, radius);
            ofDrawEllipse(ctrl_point31.x, ctrl_point31.y, ctrl_point31.z, radius, radius);
            ofDrawEllipse(ctrl_point32.x, ctrl_point32.y, ctrl_point32.z, radius, radius);

        }

        mainCam.end();
        ofDisableDepthTest();
       
        // afficher instructions pour l'utilisateur 
        ofSetColor(200);
        string msg = "Utiliser les fleches pour deplacer les points de controle\nr : reset\nq : afficher les lignes de controle\n"
            "w : afficher les vertex / triangulation\na : afficher les courbes de Bezier\ns : afficher image en texture";
        ofDrawBitmapString(msg, 400, 20);
    }
}

void SurfaceBezierRenderer::reset() {

   // initialisation des variables
    
   //  BAS --------------------------------------------------------------

    initial_position1 = { -200, -200, 0 };
    initial_position2 = { -66.6667, -200, 0 };
    initial_position3 = { 66.6667, -200, 0 };
    initial_position4 = { 200, -200, 0 };

    ctrl_point1 = initial_position1;
    ctrl_point2 = initial_position2;
    ctrl_point3 = initial_position3;
    ctrl_point4 = initial_position4;

    // HAUT --------------------------------------------------------------

    initial_position5 = { -200, 200, 0 };
    initial_position6 = { -66.6667, 200, 0 };
    initial_position7 = { 66.6667, 200, 0 };
    initial_position8 = { 200, 200, 0 };

    ctrl_point5 = initial_position5;
    ctrl_point6 = initial_position6;
    ctrl_point7 = initial_position7;
    ctrl_point8 = initial_position8;

    // GAUCHE --------------------------------------------------------------

    initial_position9 = { -200, -200, 0 };
    initial_position10 = { -200, -66.6667, 0 };
    initial_position11 = { -200, 66.6667, 0 };
    initial_position12 = { -200, 200, 0 };

    ctrl_point9 = initial_position9;
    ctrl_point10 = initial_position10;
    ctrl_point11 = initial_position11;
    ctrl_point12 = initial_position12;

    // DROIT --------------------------------------------------------------

    initial_position13 = { 200, -200, 0 };
    initial_position14 = { 200, -66.6667, 0 };
    initial_position15 = { 200, 66.6667, 0 };
    initial_position16 = { 200, 200, 0 };

    ctrl_point13 = initial_position13;
    ctrl_point14 = initial_position14;
    ctrl_point15 = initial_position15;
    ctrl_point16 = initial_position16;

    // MILIEU VERTICALE GAUCHE --------------------------------------------------------------

    initial_position17 = { -66.6667, -200, 0 };
    initial_position18 = { -66.6667, -66.6667, 0 };
    initial_position19 = { -66.6667, 66.6667, 0 };
    initial_position20 = { -66.6667, 200, 0 };

    ctrl_point17 = initial_position17;
    ctrl_point18 = initial_position18;
    ctrl_point19 = initial_position19;
    ctrl_point20 = initial_position20;

    // MILIEU VERTICALE DROIT --------------------------------------------------------------

    initial_position21 = { 66.6667, -200, 0 };
    initial_position22 = { 66.6667, -66.6667, 0 };
    initial_position23 = { 66.6667, 66.6667, 0 };
    initial_position24 = { 66.6667, 200, 0 };

    ctrl_point21 = initial_position21;
    ctrl_point22 = initial_position22;
    ctrl_point23 = initial_position23;
    ctrl_point24 = initial_position24;

    // MILIEU HORIZONTALE HAUT -------------------------------------------------------------- 

    initial_position25 = { -200, 66.6667, 0 };
    initial_position26 = { -66.6667, 66.6667, 0 };
    initial_position27 = { 66.6667, 66.6667, 0 };
    initial_position28 = { 200, 66.6667, 0 };

    ctrl_point25 = initial_position25;
    ctrl_point26 = initial_position26;
    ctrl_point27 = initial_position27;
    ctrl_point28 = initial_position28;

    // MILIEU HORIZONTALE BAS-------------------------------------------------------------- 

    initial_position29 = { -200, -66.6667, 0 };
    initial_position30 = { -66.6667, -66.6667, 0 };
    initial_position31 = { 66.6667, -66.6667, 0 };
    initial_position32 = { 200, -66.6667, 0 };

    ctrl_point29 = initial_position29;
    ctrl_point30 = initial_position30;
    ctrl_point31 = initial_position31;
    ctrl_point32 = initial_position32;

    //--------------------------------------------------------------

    selected_ctrl_point = &ctrl_point2;

    delta_x = motion_speed;
    delta_y = motion_speed; 

}

void SurfaceBezierRenderer::keyPressed(int key) {
    if (surface_bezier_toggle) {
        switch (key) { 

        case 'w':
            draw_wireframe = !draw_wireframe;
            break;

        case 'q': // touche q
            if (!control_line) control_line = true;
            else control_line = false;
            break;

        case 'a': // touche a       
            if (!bez_line) bez_line = true;
            else bez_line = false;
            break;

        case 's': // touche s       
            if (!affiche_image) affiche_image = true;
            else affiche_image = false;
            break;
        
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
}

void SurfaceBezierRenderer::keyReleased(int key) {
    if (surface_bezier_toggle) {
        switch (key)
        {

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
        }
    }
}

void SurfaceBezierRenderer::buttonSelectionPointControle_1() {
    selected_ctrl_point = &ctrl_point1;
}

void SurfaceBezierRenderer::buttonSelectionPointControle_2() {
    selected_ctrl_point = &ctrl_point17;
}

void SurfaceBezierRenderer::buttonSelectionPointControle_3() {
    selected_ctrl_point = &ctrl_point21;
}

void SurfaceBezierRenderer::buttonSelectionPointControle_4() {
    selected_ctrl_point = &ctrl_point4;
}

void SurfaceBezierRenderer::buttonSelectionPointControle_5() {
    selected_ctrl_point = &ctrl_point5;
}

void SurfaceBezierRenderer::buttonSelectionPointControle_6() {
    selected_ctrl_point = &ctrl_point20;
}

void SurfaceBezierRenderer::buttonSelectionPointControle_7() {
    selected_ctrl_point = &ctrl_point24;
}

void SurfaceBezierRenderer::buttonSelectionPointControle_8() {
    selected_ctrl_point = &ctrl_point8;
}

void SurfaceBezierRenderer::buttonSelectionPointControle_9() {
    selected_ctrl_point = &ctrl_point29;
}

void SurfaceBezierRenderer::buttonSelectionPointControle_10() {
    selected_ctrl_point = &ctrl_point25;
}

void SurfaceBezierRenderer::buttonSelectionPointControle_11() {
    selected_ctrl_point = &ctrl_point32;
}

void SurfaceBezierRenderer::buttonSelectionPointControle_12() {
    selected_ctrl_point = &ctrl_point28;

}

void SurfaceBezierRenderer::buttonSelectionPointControle_13() {
    selected_ctrl_point = &ctrl_point26;
}

void SurfaceBezierRenderer::buttonSelectionPointControle_14() {
    selected_ctrl_point = &ctrl_point27;
}

void SurfaceBezierRenderer::buttonSelectionPointControle_15() {
    selected_ctrl_point = &ctrl_point30;
}

void SurfaceBezierRenderer::buttonSelectionPointControle_16() {
    selected_ctrl_point = &ctrl_point31;
}

