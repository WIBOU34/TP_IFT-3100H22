/**
 * \IFT3100H21 
 * \file topologieRenderer.cpp
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
    
    point_1.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle);
    parameters_point_controle.add(point_1.setup("Point 1")->getParameter());
    point_2.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle);
    parameters_point_controle.add(point_2.setup("Point 2")->getParameter());
    point_3.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle);
    parameters_point_controle.add(point_3.setup("Point 3")->getParameter());
    point_4.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle);
    parameters_point_controle.add(point_4.setup("Point 4")->getParameter());
    point_5.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle);
    parameters_point_controle.add(point_5.setup("Point 5")->getParameter());
    point_6.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle);
    parameters_point_controle.add(point_6.setup("Point 6")->getParameter());
    point_7.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle);
    parameters_point_controle.add(point_7.setup("Point 7")->getParameter());
    point_8.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle);
    parameters_point_controle.add(point_8.setup("Point 8")->getParameter());
    point_9.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle);
    parameters_point_controle.add(point_9.setup("Point 9")->getParameter());
    point_10.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle);
    parameters_point_controle.add(point_10.setup("Point 10")->getParameter());
    point_11.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle);
    parameters_point_controle.add(point_11.setup("Point 11")->getParameter());
    point_12.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle);
    parameters_point_controle.add(point_12.setup("Point 12")->getParameter());
    point_13.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle);
    parameters_point_controle.add(point_13.setup("Point 13")->getParameter());
    point_14.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle);
    parameters_point_controle.add(point_14.setup("Point 14")->getParameter());
    point_15.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle);
    parameters_point_controle.add(point_15.setup("Point 15")->getParameter());
    point_16.addListener(this, &SurfaceBezierRenderer::buttonSelectionPointControle);
    parameters_point_controle.add(point_16.setup("Point 16")->getParameter());


    //set the width and height for our mesh and initial rendering values
    width = 20;
    height = 20;
    // set our rendering styles to false
    b_messyMesh = false;
    b_perlinMesh = false;
    b_drawWireFrame = true;
    // set the initial values to use for our perlinNoise
    perlinRange = 1.0;
    perlinHeight = 5.0;

    //ofBackground(31); // set the window background 
    mainCam.setPosition(0, 0, 30); // set initial position for easyCam 3D viewer


    // here we make the points inside our mesh
    // add one vertex to the mesh across our width and height
    // we use these x and y values to set the x and y co-ordinates of the mesh, adding a z value of zero to complete the 3d location of each vertex
    // source -> https://openframeworks.cc/documentation/3d/ofMesh/
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            mainMesh.addVertex(ofPoint(x - width / 2, y - height / 2, 0));    // make a new vertex
            mainMesh.addColor(ofFloatColor(0, 100, 0));  // add a color at that vertex
        }
    }

    // here we loop through and join the vertices together as indices to make rows of triangles to make the wireframe grid
    // source -> https://openframeworks.cc/documentation/3d/ofMesh/
    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width - 1; x++) {
            mainMesh.addIndex(x + y * width);                  // 0
            mainMesh.addIndex((x + 1) + y * width);            // 1
            mainMesh.addIndex(x + (y + 1) * width);            // 10

            mainMesh.addIndex((x + 1) + y * width);            // 1
            mainMesh.addIndex((x + 1) + (y + 1) * width);      // 11
            mainMesh.addIndex(x + (y + 1) * width);            // 10
        }
    }


    // courbe de bezier 
    is_key_press_up = false;
    is_key_press_down = false;
    is_key_press_left = false;
    is_key_press_right = false;

    line_resolution = 20;
    line_width_outline = 4.0f;
    line_width_curve = 8.0f;
    radius = 0.5f;
    scale = 10.0f;
    offset = 64.0f;
    motion_speed = 10.0f;

    // initialisation des sommets des lignes
    for (index = 0; index <= line_resolution; ++index) {
        line_renderer.addVertex(ofPoint());
        line_renderer_2.addVertex(ofPoint());
        line_renderer_3.addVertex(ofPoint());
        line_renderer_4.addVertex(ofPoint());
        line_renderer_5.addVertex(ofPoint());
        line_renderer_6.addVertex(ofPoint());
        line_renderer_7.addVertex(ofPoint());
        line_renderer_8.addVertex(ofPoint());

        line_int_1.addVertex(ofPoint());

    }

    vertex_bezier_1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
    vertex_bezier_2 = { 380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399 };
    vertex_bezier_3 = { 0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 300, 320, 340, 360, 380 };
    vertex_bezier_4 = { 19, 39, 59, 79, 99, 119, 139, 159, 179, 199, 219, 239, 259, 279, 299, 319, 339, 359, 379, 399 };
    vertex_bezier_5 = { 6, 26, 46, 66, 86, 106, 126, 146, 166, 186, 206, 226, 246, 266, 286, 306, 326, 346, 366, 386 };
    vertex_bezier_6 = { 13, 33, 53, 73,  93, 113, 133, 153, 173, 193, 213, 233, 253, 273, 293, 313, 333, 353, 373, 393 };
    vertex_bezier_7 = { 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259 };
    vertex_bezier_8 = { 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159 };


    vertex_bezier_int_1 = { 1, 21, 41, 61, 81, 101, 121, 141, 161, 181, 201, 221, 241, 261, 281, 301, 321, 341, 361, 381 };



    // initialisation de la scène
    reset();
}

void SurfaceBezierRenderer::updateRenderer() {

    // courbe de bezier 
    time_current = ofGetElapsedTimef();
    time_elapsed = time_current - time_last;
    time_last = time_current;

    // unir les coins communs
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

   

    // key movement assignation
    if (is_key_press_up) {
        selected_ctrl_point->z += delta_y * time_elapsed; 
    }
    if (is_key_press_down) {
        selected_ctrl_point->z -= delta_y * time_elapsed;
    }
    if (is_key_press_left) {
        selected_ctrl_point->x -= delta_x * time_elapsed;
    }
    if (is_key_press_right) {
        selected_ctrl_point->x += delta_x * time_elapsed;
    }



    // bezier parameter 
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

        bezier_cubic(
            index / (float)line_resolution,
            ctrl_point9.x, ctrl_point9.y, ctrl_point9.z,
            ctrl_point10.x, ctrl_point10.y, ctrl_point10.z,
            ctrl_point11.x, ctrl_point11.y, ctrl_point11.z,
            ctrl_point12.x, ctrl_point12.y, ctrl_point12.z,
            position_3.x, position_3.y, position_3.z);
        //test
        /*bezier_cubic(
            index / (float)line_resolution,
            ctrl_point9.x, ctrl_point9.y, ctrl_point9.z,
            ctrl_point10.x, ctrl_point10.y, ctrl_point10.z,
            ctrl_point11.x, ctrl_point11.y, ctrl_point11.z,
            ctrl_point12.x, ctrl_point12.y, ctrl_point12.z,
            position_9.x, position_9.y, position_9.z);*/
        //
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
        line_renderer[index] = position;
        line_renderer_2[index] = position_2;
        line_renderer_3[index] = position_3;
        line_renderer_4[index] = position_4;
        line_renderer_5[index] = position_5;
        line_renderer_6[index] = position_6;
        line_renderer_7[index] = position_7;
        line_renderer_8[index] = position_8;
        line_int_1[index] = position_9;
    }

    // affecter les positions des mesh sur la courbe 
    for (int i = 0; i < 20; i++) {
        mainMesh.setVertex(vertex_bezier_1[i], line_renderer[i]);
        mainMesh.setVertex(vertex_bezier_2[i], line_renderer_2[i]);
        mainMesh.setVertex(vertex_bezier_3[i], line_renderer_3[i]);        
        mainMesh.setVertex(vertex_bezier_4[i], line_renderer_4[i]);
        mainMesh.setVertex(vertex_bezier_5[i], line_renderer_5[i]);
        mainMesh.setVertex(vertex_bezier_6[i], line_renderer_6[i]);
        mainMesh.setVertex(vertex_bezier_7[i], line_renderer_7[i]);
        mainMesh.setVertex(vertex_bezier_8[i], line_renderer_8[i]);
        mainMesh.setVertex(vertex_bezier_int_1[i], line_int_1[i]);

       
        //ofVec3f t(line_int_1[i].x, line_int_1[i].y, line_int_1[i].z);
        
        //line_int_1.addVertex(mainMesh.getVertex(vertex_bezier_int_1[i]));
       
        
        //mainMesh.setVertex(vertex_bezier_int_1[i], line_int_1[i]);
        
    }

    
    

}

void SurfaceBezierRenderer::generateDraw() {


}

void SurfaceBezierRenderer::render() {

    if (surface_bezier_toggle) {
       
        ofFill();
        ofEnableDepthTest();
        mainCam.begin();


        // choose to draw our mesh as wireframe or point cloud
        if (b_drawWireFrame) mainMesh.drawWireframe();
        else mainMesh.drawVertices();

        // dessiner la ligne contour
        ofSetColor(0, 0, 255);
        if (control_line) {


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
        // dessiner la courbe 
        ofSetColor(0, 255, 0);
        line_renderer.draw();
        line_renderer_2.draw();
        line_renderer_3.draw();
        line_renderer_4.draw();
        line_renderer_5.draw();
        line_renderer_6.draw();
        line_renderer_7.draw();
        line_renderer_8.draw();

        //line_int_1.draw();

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

       
        ofSetColor(200);
        string msg = "Utiliser les fleches pour deplacer les points de controle\nr : reset\nq : afficher les lignes de controle\n"
            "w : afficher les vertex";
        ofDrawBitmapString(msg, 400, 20);

    }
}

void SurfaceBezierRenderer::reset() {


    // initialisation des variables

    //  BAS --------------------------------------------------------------

    initial_position1 = { -10, -10, 0 };
    initial_position2 = { -4, -10, 0 };
    initial_position3 = { 3, -10, 0 };
    initial_position4 = { 9, -10, 0 };

    ctrl_point1 = initial_position1;
    ctrl_point2 = initial_position2;
    ctrl_point3 = initial_position3;
    ctrl_point4 = initial_position4;

    // HAUT --------------------------------------------------------------

    initial_position5 = { -10, 9, 0 };
    initial_position6 = { -4, 9, 0 };
    initial_position7 = { 3, 9, 0 };
    initial_position8 = { 9, 9, 0 };

    ctrl_point5 = initial_position5;
    ctrl_point6 = initial_position6;
    ctrl_point7 = initial_position7;
    ctrl_point8 = initial_position8;

    // GAUCHE --------------------------------------------------------------

    initial_position9 = { -10, -10, 0 };
    initial_position10 = { -10, -3, 0 };
    initial_position11 = { -10, 3, 0 };
    initial_position12 = { -10, 9, 0 };

    ctrl_point9 = initial_position9;
    ctrl_point10 = initial_position10;
    ctrl_point11 = initial_position11;
    ctrl_point12 = initial_position12;

    // DROIT --------------------------------------------------------------

    initial_position13 = { 10, -10, 0 };
    initial_position14 = { 9, -3, 0 };
    initial_position15 = { 9, 3, 0 };
    initial_position16 = { 10, 9, 0 };

    ctrl_point13 = initial_position13;
    ctrl_point14 = initial_position14;
    ctrl_point15 = initial_position15;
    ctrl_point16 = initial_position16;

    // MILIEU VERTICALE GAUCHE --------------------------------------------------------------

    initial_position17 = { -4, -10, 0 };
    initial_position18 = { -4, -3, 0 };
    initial_position19 = { -4, 3, 0 };
    initial_position20 = { -4, 9, 0 };

    ctrl_point17 = initial_position17;
    ctrl_point18 = initial_position18;
    ctrl_point19 = initial_position19;
    ctrl_point20 = initial_position20;

    // MILIEU VERTICALE DROIT --------------------------------------------------------------

    initial_position21 = { 3, -10, 0 };
    initial_position22 = { 3, -3, 0 };
    initial_position23 = { 3, 3, 0 };
    initial_position24 = { 3, 9, 0 };

    ctrl_point21 = initial_position21;
    ctrl_point22 = initial_position22;
    ctrl_point23 = initial_position23;
    ctrl_point24 = initial_position24;

    // MILIEU HORIZONTALE HAUT -------------------------------------------------------------- 

    initial_position25 = { -10, 2, 0 };
    initial_position26 = { -4, 2, 0 };
    initial_position27 = { 3, 2, 0 };
    initial_position28 = { 9, 2, 0 };

    ctrl_point25 = initial_position25;
    ctrl_point26 = initial_position26;
    ctrl_point27 = initial_position27;
    ctrl_point28 = initial_position28;

    // MILIEU HORIZONTALE -------------------------------------------------------------- 

    initial_position29 = { -10, -3, 0 };
    initial_position30 = { -4, -3, 0 };
    initial_position31 = { 3, -3, 0 };
    initial_position32 = { 9, -3, 0 };

    ctrl_point29 = initial_position29;
    ctrl_point30 = initial_position30;
    ctrl_point31 = initial_position31;
    ctrl_point32 = initial_position32;
    //--------------------------------------------------------------

    ctrl_point33 = { -9, -10, 0 };
    ctrl_point34 = { -9, -3, 0 };
    ctrl_point35 = { -9, 3, 0 };
    ctrl_point36 = { -9, 9, 0 };

    selected_ctrl_point = &ctrl_point2;

    delta_x = motion_speed;
    delta_y = motion_speed;

    ofLog() << "<reset>";
}

void SurfaceBezierRenderer::keyPressed(int key) {
    if (surface_bezier_toggle) {
        switch (key) { // use a switch statement to evaluate which key is pressed
        case 'f':
            ofToggleFullscreen();
            break;

        case ' ':
            b_messyMesh = !b_messyMesh;
            break;

        case 'w':
            b_drawWireFrame = !b_drawWireFrame;
            break;

        case 'p':
            b_perlinMesh = !b_perlinMesh;
            break;


        case 'q': // touche q
            if (!control_line) control_line = true;
            else control_line = false;

            break;

        case 97: // touche a


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

        case 49: // touche 1
            selected_ctrl_point = &ctrl_point17;
            break;

        case 50: // touche 2
            selected_ctrl_point = &ctrl_point21;
            break;

        case 51: // touche 3
            selected_ctrl_point = &ctrl_point30;
           
            break;

        case 52: // touche 4
            selected_ctrl_point = &ctrl_point31;
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

void SurfaceBezierRenderer::buttonSelectionPointControle() {
    
    ofLog() << "test ok !";
}
