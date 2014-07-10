#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class testApp : public ofBaseApp{
public:
        
    void setup();
    void update();
    void draw();
    
    void drawScene();
    void enableLights();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    ofFbo fbo, noiseFbo;
    ofEasyCam cam;
    ofSpherePrimitive sphere;
    ofShader bumpShader;
    ofShader noiseShader;
    ofImage colormap, bumpmap, normalMap, specularMap;
    
    ofxPanel gui;
    ofxFloatSlider lightX;
    ofxFloatSlider lightY;
    ofxFloatSlider lightZ;
    ofxFloatSlider lightShininess;
    ofxFloatSlider bumpScale;
    
    ofxFloatColorSlider lightAmbient;
    ofxFloatColorSlider lightDiffuse;
    ofxFloatColorSlider lightSpecular;
    ofxFloatColorSlider matAmbient;
    ofxFloatColorSlider matDiffuse;
    ofxFloatColorSlider matSpecular;
    ofxFloatSlider attenuationLin, attenuationConst, attenuationQuad;
};