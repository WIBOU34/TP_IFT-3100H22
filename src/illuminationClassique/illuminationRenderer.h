// IFT3100H21_Lighting/renderer.h
// Classe responsable du rendu de l'application.

#pragma once

#include "../renderer.h"
#include "ofxAssimpModelLoader.h"
#include "ofMath.h"
#include "ofxGui.h"

struct objet3DShader {
	of3dPrimitive objet;
	ofMaterial material;
};

enum class ShaderType { defaut, lambert, gouraud, phong, blinn_phong };

class IlluminationRenderer : public RendererBase {
public:
	std::vector<objet3DShader> vecObjets = std::vector<objet3DShader>(3);
	objet3DShader* selectedObject = nullptr;
	ofxGuiGroup parameters;
	//ofParameterGroup parameters;
	ofCamera* camera;

	ofParameter<bool> drawObjetcs;

	ofParameter<int> indexObject;
	ofParameter<int> indexShader;
	ofxLabel lblShaderActive;

	ofParameter<float> constantPoint;
	ofParameter<float> linearPoint;
	ofParameter<float> quadraticPoint;
	ofParameter<float> constantSpot;
	ofParameter<float> linearSpot;
	ofParameter<float> quadraticSpot;
	ofxLabel attenuation;

	ofFloatColor light_ambient;

	ofLight light_directional;
	ofLight light_point;
	ofLight light_spot;

	ofMaterial material_cube;
	ofMaterial material_sphere;
	ofMaterial material_teapot;

	ShaderType shader_active;

	ofShader shader_color_fill;
	ofShader shader_lambert;
	ofShader shader_gouraud;
	ofShader shader_phong;
	ofShader shader_blinn_phong;

	ofShader* shader;
	string shader_name;
	ofLight* selectedLight;

	ofxAssimpModelLoader teapot;

	ofQuaternion orientation_directional;
	ofQuaternion orientation_spot;

	ofVec3f position_cube;
	ofVec3f position_sphere;
	ofVec3f position_teapot;

	ofxGuiGroup gPoint;
	ofxGuiGroup gDirectional;
	ofxGuiGroup gSpot;
	ofxGuiGroup gMaterial;

	ofxButton setObsidian;
	ofxButton setBronze;
	ofxButton setGold;
	ofxButton setSilver;
	ofParameter<ofColor> ambientColor;
	ofParameter<ofColor> diffuseColorPoint;
	ofParameter<ofColor> specularColorPoint;
	ofParameter<ofColor> diffuseColorSpot;
	ofParameter<ofColor> specularColorSpot;
	ofParameter<ofColor> diffuseColorDir;
	ofParameter<ofColor> specularColorDir;

	ofParameter<bool> lightPointEnabled;
	ofParameter<bool> lightSpotEnabled;
	ofParameter<bool> lightDirEnabled;
	ofParameter<bool> lightAmbientEnabled;
	ofParameter<bool> usingDefaultShader;

	ofParameter<ofVec3f> pLookAtDirectional;
	ofParameter<ofVec3f> pPositionDirectional;
	ofParameter<ofVec3f> pPositionPoint;
	ofParameter<ofVec3f> pPositionSpot;

	float camera_offset;

	float oscillation;

	float scale_cube;
	float scale_sphere;
	float scale_teapot;

	float center_x;
	float center_y;

	float speed_motion;

	float oscillation_frequency;
	float oscillation_amplitude;

	float offset_x;
	float offset_z;

	float delta_x;
	float delta_z;

	float initial_x;
	float initial_z;

	bool is_active_ligh_ambient;
	bool is_active_light_directional;
	bool is_active_light_point;
	bool is_active_light_spot;

	bool use_smooth_lighting;

	void reset();

	void lighting_on();
	void lighting_off();

	float oscillate(float time, float frequency, float amplitude);

protected:
	void setupRenderer(const std::string& name);
	void updateRenderer();
	void generateDraw();
	void render();
private:
	const std::string LIGHT_POINT = "light_point";
	const std::string LIGHT_DIRECTIONAL = "light_directional";
	const std::string LIGHT_SPOT = "light_spot";
	void updateLight();
	void setMaterialObsidian();
	void setMaterialBronze();
	void setMaterialGold();
	void setMaterialSilver();
	void setMaterialObject(const ofMaterial& material);
};
