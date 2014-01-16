//
//  OscReceiver.cpp
//  futuretv
//
//  Created by Chris Mullany on 16/09/2013.
//  Made for AllofUs
//

#include "OscReceiver.h"

OscReceiver::OscReceiver() {
}

void OscReceiver::setup() {
    ofLogNotice() << "OscReceiver::setup: Listening for osc messages on port " << PORT;
	receiver.setup(PORT);
}

void OscReceiver::update() {
    // check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        string msg_data;
        string msg_address;
        string msg_string;
        msg_address = m.getAddress();
        // check the message address
        //accxyz: float:0.369003float:0.118439float:-0.946869
        if (msg_address == "/accxyz") {
            acc.x = m.getArgAsFloat(0);
            acc.y = m.getArgAsFloat(1);
            acc.z = m.getArgAsFloat(2);
        }
        else if(m.getAddress() == "/3/rotary1"){
			// the single argument is a string
			slider = m.getArgAsFloat(0);
		}
        else if(m.getAddress() == "/ping"){
			// PING!
		}
        // unknown message type
        if (true) {
            string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for(int i = 0; i < m.getNumArgs(); i++){
				// get the argument type
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if(m.getArgType(i) == OFXOSC_TYPE_INT32){
					msg_string += ofToString(m.getArgAsInt32(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
					msg_string += ofToString(m.getArgAsFloat(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
					msg_string += m.getArgAsString(i);
				}
				else{
					msg_string += "unknown";
				}
			}
            ofLogWarning() << "OscReceiver::update UNKNOWN MESSAGE " << msg_string;
        }
        
	}
}

void OscReceiver::draw() {
}

void OscReceiver::exit() {
}

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

void OscReceiver::keyPressed (int key) {}

void OscReceiver::mouseDragged(int x, int y, int button) {}

void OscReceiver::mousePressed(int x, int y, int button) {}

void OscReceiver::mouseReleased(int x, int y, int button) {}

void OscReceiver::windowResized(int w, int h) {}

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////

