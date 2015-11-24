#pragma once

#include "ofMain.h"
#include "OscReceiver.h"
//#include "ofxTuio.h"

class testApp : public ofBaseApp {
public:
    void setup();
    void setupFbo();
    void update();
    void draw();
    void drawShader();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofFbo fbo;
    ofShader bokehShader;
    float bias, focus;
    
    ofEasyCam cam;
    ofMesh mesh;
    ofVboMesh vboMesh;
    ofLight light;
    ofMaterial material;
    bool smoothLighting;
    
    ofVec3f rotation;
    OscReceiver osc;
    
    // TUIO
//    void	tuioAdded(ofxTuioCursor & tuioCursor);
//	void	tuioRemoved(ofxTuioCursor & tuioCursor);
//	void	tuioUpdated(ofxTuioCursor & tuioCursor);

//    ofxTuioClient   tuioClient;
};
