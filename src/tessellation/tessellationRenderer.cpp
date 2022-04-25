/**
 * \IFT3100H21 
 * \file tessellationRenderer.cpp
 * \author Stéphane Boulanger
 * \brief Classe responsable de la tessellation de l'application
 * \version 0.1
 * \date 2022-04-05
 */

#include "tessellationRenderer.h"



void TessellationRenderer::setupRenderer(const std::string& name) {
	parameters.clear();
	parameters.setName(name);
    parameters.add(tessellation_toggle.setup("Activer la tessellation", false)->getParameter()); 
    
    // panel pour modifier la tesselation     
    parameters_tessellation.setName("Parametres");
    box_tess_button.addListener(this, &TessellationRenderer::buttonBoxTessellation);
    parameters_tessellation.add(box_tess_button.setup("Box")->getParameter());
    sphere_tess_button.addListener(this, &TessellationRenderer::buttonSphereTessellation);
    parameters_tessellation.add(sphere_tess_button.setup("Sphere")->getParameter());
    parameters_tessellation.add(tess_level_slider.set("Niveau tessellation", 1.0, 1.0, 5.0));
    parameters_tessellation.add(tess_resolution_slider.set("Resolution", 10.0, 2.0, 10.0));
    parameters_tessellation.add(patch_vertice_slider.set("Vertices", 4.0, 0.0, 8.0));

    // setup de la tessellation
    shader_tess.setGeometryInputType(GL_TRIANGLES);
    shader_tess.setGeometryOutputType(GL_LINE_STRIP);
    shader_tess.setGeometryOutputCount(4);
    glPatchParameteri(GL_PATCH_VERTICES, 2);

    // load des shaders 
    shader_tess.setupShaderFromFile(GL_VERTEX_SHADER, "shader/tess_410_vert.glsl");
    shader_tess.setupShaderFromFile(GL_FRAGMENT_SHADER, "shader/tess_410_frag.glsl");
    shader_tess.setupShaderFromFile(GL_GEOMETRY_SHADER_EXT, "shader/tess_410_geom.glsl");
    shader_tess.setupShaderFromFile(GL_TESS_CONTROL_SHADER, "shader/tess_410_cont.glsl");
    shader_tess.setupShaderFromFile(GL_TESS_EVALUATION_SHADER, "shader/tess_410_eval.glsl");
    shader_tess.linkProgram();
    
}

void TessellationRenderer::updateRenderer() {
    
    glPatchParameteri(GL_PATCH_VERTICES, patch_vertice_slider);

    radius = 180.0f;
    sphere.setRadius(radius);
    sphere.setResolution(tess_resolution_slider);
    sphereMesh = sphere.getMesh();
    sphereVbo.setMesh(sphereMesh, GL_DYNAMIC_DRAW);
   
    box.set(300);
    box.setResolution(tess_resolution_slider);
    boxMesh = box.getMesh();
    boxVbo.setMesh(boxMesh, GL_DYNAMIC_DRAW);
    
}

void TessellationRenderer::generateDraw() {}

void TessellationRenderer::render() {

    if (tessellation_toggle) {

        ofSetColor(ofColor::green);
        ofSetBackgroundColor(0);

        ofEnableDepthTest();
        cam_tess.begin();

        shader_tess.begin();

        shader_tess.setUniform1f("tess_level", tess_level_slider);

        ofPushMatrix();
        ofRotate(ofGetElapsedTimef() * 10, 0, 1, 0);
        if(sphere_active)sphereVbo.drawElements(GL_PATCHES, sphereVbo.getNumIndices());
        if(box_active)boxVbo.drawElements(GL_PATCHES, boxVbo.getNumIndices());   
        ofPopMatrix();

        shader_tess.end();

        cam_tess.end();
        ofDisableDepthTest();


    }
}

void TessellationRenderer::buttonBoxTessellation() {    
        box_active = true;
        sphere_active = false; 
}

void TessellationRenderer::buttonSphereTessellation() {
       sphere_active = true;
       box_active = false;
}