#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp {
	
public:
    void reset();
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofImage img;
    int maxSize;
    int generation;
    ofImageQualityType quality;
    int addX, addY, subX, subY;
    float glitchStart;
	
	void drawWithShader();
    
    ofFbo fbo;
	
	ofShader shader;
	ofVboMesh mesh;
	float rotX, rotY, rotSmooth, shaderZScaling, shaderZOffset, alpha, lightX, lightY, lightZ;
	bool useColourTex;
	
	ofVideoGrabber videoGrabber;
};
