#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDOF.h"
#include "ofxFFTLive.h"
#include "ofxFFTFile.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void updateMesh(ofMesh* mesh, ofMesh* MeshOut);
    void updateNormals(ofMesh* mesh);
    void setNormals( ofMesh &mesh );
    
    void createSphere(ofVboMesh *vbo, float radius, unsigned int rings, unsigned int sectors);
    void createGeoSphere(ofVboMesh *vbo, int stacks=32, int slices=32);
    
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
    
    
    ofxDOF depthOfField;
    ofShader phongShader;
    ofShader toonShader;
    
    ofSpherePrimitive sphereBase;
    ofSpherePrimitive sphere;
    ofIcoSpherePrimitive icoSphereBase;
    ofIcoSpherePrimitive icoSphere;
    
    ofEasyCam cam;
    ofLight spotLight;
    ofMaterial material;
    bool bSmoothLighting;
    
    
    //////////////////////////////////////////////////////////////////////////////////
    // FFT
    //////////////////////////////////////////////////////////////////////////////////
    ofxFFTLive fftLive;
    ofxFFTFile fftFile;
    vector<float> levels;
    int numLevels;
    bool audioMirror;
    
    
    //////////////////////////////////////////////////////////////////////////////////
    // GUI
    //////////////////////////////////////////////////////////////////////////////////
    bool isGuiVisible;
    ofxPanel gui;
    
    ofParameter<ofVec3f> noiseIn;
    ofParameter<float> noiseInDiv;
    ofParameter<float> noiseOutMult;
    ofParameter<bool> isNoiseFromNormal;
    ofParameter<bool> isNoiseAnimated;
    ofParameter<int> noiseVertMod;
    ofParameterGroup noiseParams;
    
    // TODO: add shadow?
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
    ofParameter<bool> isToonShaderOn;
    ofParameterGroup sphereParams;
    
    ofParameter<bool> isDofEnabled;
    ofParameter<float> focalDistance;
    ofParameter<float> focalRange;
    ofParameter<float> blurAmount;
    ofParameterGroup dofParams;
};
