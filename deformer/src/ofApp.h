#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
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
    ofSpherePrimitive sphereBase;
    ofSpherePrimitive sphere;
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
    
    bool isGuiVisible;
    ofxPanel gui;
    ofParameter<ofVec3f> noiseIn;
    ofParameter<float> noiseInDiv;
    ofParameter<float> noiseOutMult;
    ofParameter<bool> isNoiseFromNormal;
    ofParameterGroup noiseParams;
};