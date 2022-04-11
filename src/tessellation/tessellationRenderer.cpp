/**
 * \IFT3100H21 
 * \file surfaceBezierRenderer.cpp
 * \author Stéphane Boulanger
 * \brief Classe responsable de la tessellation de l'application
 * \version 0.1
 * \date 2022-04-05
 */

#include "tessellationRenderer.h"

void TessellationRenderer::buttonShaderTessellation() {
    if (!load_shader) {
        load_shader = true;
        shader_tess.setupShaderFromFile(GL_VERTEX_SHADER, "shader/tess_410_vert.glsl");
        shader_tess.setupShaderFromFile(GL_FRAGMENT_SHADER, "shader/tess_410_frag.glsl");
        shader_tess.setupShaderFromFile(GL_GEOMETRY_SHADER_EXT, "shader/tess_410_geom.glsl");
        shader_tess.setupShaderFromFile(GL_TESS_CONTROL_SHADER, "shader/tess_410_cont.glsl");
        shader_tess.setupShaderFromFile(GL_TESS_EVALUATION_SHADER, "shader/tess_410_eval.glsl");
        shader_tess.linkProgram();
        glPatchParameteri(GL_PATCH_VERTICES, 4);

        ofLog() << "SHADER IS LOADED";
    }
    else {
        load_shader = false;
        shader_tess.unload();
        ofLog() << "SHADER UNLOADED";
    }
}

void TessellationRenderer::setupRenderer(const std::string& name) {
	parameters.clear();
	parameters.setName(name);

    parameters.add(tessellation_toggle.setup("Tessellation", false)->getParameter());

    // panel pour la tesselation
    parameters_tessellation.setName("Parametres");

    shader_tess_button.addListener(this, &TessellationRenderer::buttonShaderTessellation);
    parameters_tessellation.add(shader_tess_button.setup("Shader")->getParameter());

    parameters_tessellation.add(tess_level_slider.set("Niveau tessellation", 1.0, 1.0, 5.0));
    parameters_tessellation.add(tess_resolution_slider.set("Resolution", 4.0, 2.0, 10.0));

    // setup de la tessellation
    shader_tess.setGeometryInputType(GL_TRIANGLES);
    shader_tess.setGeometryOutputType(GL_LINE_STRIP);
    shader_tess.setGeometryOutputCount(4);
    glPatchParameteri(GL_PATCH_VERTICES, 2);
}

void TessellationRenderer::updateRenderer() {
    
    radius = 180.f;
    sphere.setRadius(radius);
    sphere.setResolution(tess_resolution_slider);
    sphereMesh = sphere.getMesh();
    sphereVbo.setMesh(sphereMesh, GL_DYNAMIC_DRAW);
}

void TessellationRenderer::generateDraw()
{
}

void TessellationRenderer::render() {

    if (tessellation_toggle) {

        ofSetColor(ofColor::green);
        ofSetBackgroundColor(0);

        ofEnableDepthTest();
        cam_tex.begin();

        shader_tess.begin();

        shader_tess.setUniform1f("u_tessLevelOuter", tess_level_slider);

        ofPushMatrix();
        ofRotate(ofGetElapsedTimef() * 10, 0, 1, 0);
        sphereVbo.drawElements(GL_PATCHES, sphereVbo.getNumIndices());
        ofPopMatrix();

        shader_tess.end();

        cam_tex.end();
        ofDisableDepthTest();






    }
}
