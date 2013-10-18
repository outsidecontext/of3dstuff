//
//  OscReceiver.h
//  futuretv
//
//  Created by Chris Mullany on 16/09/2013.
//  Made for AllofUs
//

#pragma once
#include "ofMain.h"
#include "ofxOsc.h"

#define PORT 3333
#define NUM_MSG 20

class OscReceiver {
public:
    OscReceiver();
    
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
    
    ofVec3f acc;
    float slider;
    
private:
    ofxOscReceiver receiver;
    
};