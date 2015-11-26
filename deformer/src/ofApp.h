#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDOF.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void updateMesh(ofMesh* mesh, ofMesh* MeshOut);
    void draw();
    void updateNormals(ofMesh* mesh);
    void setNormals( ofMesh &mesh );
    bool checkSphereSafeIndex(int i);
    void createSphere(ofVboMesh *vbo, float radius, unsigned int rings, unsigned int sectors);
    void createGeoSphere(int stacks=32, int slices=32);
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofVboMesh vbo;
    ofxDOF depthOfField;
    ofShader phongShader;
    
    ofSpherePrimitive sphereBase;
    ofSpherePrimitive sphere;
    ofIcoSpherePrimitive icoSphereBase;
    ofIcoSpherePrimitive icoSphere;
    
    ofEasyCam cam;
    
    
    ofLight pointLight;
    ofLight spotLight;
    ofLight directionalLight;
    
    ofMaterial material;
    ofImage ofLogoImage;
    
    float radius;
    bool bShiny;
    bool bSmoothLighting;
    bool bPointLight, bSpotLight, bDirLight;
    bool bUseTexture;
    
    // GUI
    bool isGuiVisible;
    ofxPanel gui;
    
    ofParameter<ofVec3f> noiseIn;
    ofParameter<float> noiseInDiv;
    ofParameter<float> noiseOutMult;
    ofParameter<bool> isNoiseFromNormal;
    ofParameter<bool> isNoiseAnimated;
    ofParameter<int> noiseVertMod;
    ofParameterGroup noiseParams;
    
    // TODO: add light and material colours
    // TODO: add shadow (FBO trick)
    ofParameter<ofColor> globalAmbient;
    ofParameter<ofVec3f> lightPosition;
    ofParameter<ofColor> lightAmbient;
    ofParameter<ofColor> lightDiffuse;
    ofParameter<ofColor> lightSpecular;
    ofParameterGroup lightParams;
    
    ofParameter<ofColor> matDiffuse;
    ofParameter<ofColor> matAmbient;
    ofParameter<ofColor> matEmissive;
    ofParameter<ofColor> matSpecular;
    ofParameter<float> matShiny;
    ofParameterGroup matParams;
    
    ofParameter<ofColor> bgColourIn;
    ofParameter<ofColor> bgColourOut;
    ofParameterGroup bgParams;
    
    ofParameter<bool> isIcoSphere;
    ofParameter<bool> isPhongShaderOn;
    ofParameterGroup sphereParams;
    
    ofParameter<bool> isDofEnabled;
    ofParameter<float> focalDistance;
    ofParameter<float> focalRange;
    ofParameter<float> blurAmount;
    ofParameterGroup dofParams;
};
