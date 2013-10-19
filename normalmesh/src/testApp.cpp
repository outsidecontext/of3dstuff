#include "testApp.h"

void testApp::setup() {
    // generate a plane mesh
	int res = 50;    
    ofPlanePrimitive plane(100, 100, res, res);
    mesh = plane.getMesh();
    mesh.setColorForIndices(0, mesh.getNumIndices(), ofColor(30,50,160));
    // material
    material.setShininess(120);
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    // lights
    smoothLighting = true;
    ofSetSmoothLighting(smoothLighting);
    light.setDirectional();
    light.setAmbientColor(ofColor(55.f, 55.f, 55.f));
    light.setDiffuseColor(ofColor(255.f, 255.f, 255.f));
    light.setPosition(0, 0, 0);
    light.setOrientation( ofVec3f(0, 0, 0) );
    
    rotation.set(0,0,0);
    //osc.setup();
    
    tuioClient.start(3333);
    ofAddListener(tuioClient.cursorAdded,this,&testApp::tuioAdded);
	ofAddListener(tuioClient.cursorRemoved,this,&testApp::tuioRemoved);
	ofAddListener(tuioClient.cursorUpdated,this,&testApp::tuioUpdated);
}


void testApp::update() {
    
    bool useTuioTouch = false;
    float blobx, bloby;
    if (useTuioTouch) tuioClient.getMessage();
    list<ofxTuioCursor*> cursors = tuioClient.getTuioCursors();
    list<ofxTuioCursor*>::iterator tit;
    
    //osc.update();
    //rotation.x = ofLerp(rotation.x, osc.acc.x*180, .01);
    //rotation.y = ofLerp(rotation.y, osc.acc.y*180, .01);
    //rotation.z = ofLerp(rotation.z, osc.acc.z*180, .01);
    
    // update light pos based on mouse
    float y = ofMap(ofGetMouseX(), 0, ofGetWidth(), -90, 90);
    //float y = ofMap(osc.slider, 0, 1, -90, 90);
    light.setOrientation( ofVec3f(0, y, 0) );
    
    // update z vertices using noise
	for (int i=0; i<mesh.getNumVertices(); i++) {
        ofVec3f vertex = mesh.getVertex(i);
        float z = ofNoise(
            vertex.x*ofGetElapsedTimef()*0.001,
            vertex.y*ofGetElapsedTimef()*0.00000001
        ) * 6;
        if (useTuioTouch) {
            for (tit=cursors.begin(); tit != cursors.end(); tit++) {
                ofxTuioCursor *blob = (*tit);
                float thresh = 6;
                blobx = ofMap(blob->getX(), 0, 1, -50, 50);
                bloby = ofMap(blob->getY(), 0, 1, 50, -50);
                if (blobx >= vertex.x-thresh && blobx <= vertex.x+thresh
                    && bloby >= vertex.y-thresh && bloby <= vertex.y+thresh) {
                    z = 10;
                }
            }
        }
        vertex.z = ofLerp(vertex.z, z, 0.1);
        mesh.setVertex(i, vertex);
    }
    
    //
    // messing around with normals
    
    // type 1 based on triangles
    // http://devmaster.net/posts/6065/calculating-normals-of-a-mesh
    // and http://www.gamedev.net/topic/634094-calculating-normals-for-triangle-strip-model-not-terrain/
    // This is close but shading looks really square?
    mesh.clearNormals();
    vector<ofVec3f>* normal_buffer = new vector<ofVec3f>[mesh.getNumVertices()];
    for (int j=0; j<mesh.getNumIndices()-2; j++) {
        ofVec3f v0 = mesh.getVertex(mesh.getIndex(j));
        ofVec3f v1 = mesh.getVertex(mesh.getIndex(j+1));
        ofVec3f v2 = mesh.getVertex(mesh.getIndex(j+2));
        ofVec3f U,V;
        // This
        if (j%2==0) {
            //normal = -normal;
            U = (v1-v0);
            V = (v2-v0);
        } else {
            U = (v0-v2);
            V = (v0-v1);
        }
        // Or this?
        U = (v2-v0);
        V = (v1-v0);
        
        ofVec3f normal = U.cross(V);
        
        if (j%2==0) normal *= -1;
        
        // Store the face's normal for each of the vertices that make up the face.
        normal_buffer[mesh.getIndex(j+0)].push_back( normal );
        if (ofGetKeyPressed('c')) {
            normal_buffer[mesh.getIndex(j+1)].push_back( normal );
            normal_buffer[mesh.getIndex(j+2)].push_back( normal );
        }
        
    }
    // loop over all normals for each vertex
    for( int i = 0; i < mesh.getNumVertices(); i++ ){
        ofVec3f normal;
        for( int j = 0; j < normal_buffer[i].size(); j++ )
            normal += normal_buffer[i][j];
        normal /= normal_buffer[i].size();
        normal.normalize();
        mesh.addNormal(normal);
    }
    
    delete [] normal_buffer;
    
    
}


void testApp::draw() {
	ofBackgroundGradient(ofColor(30), ofColor(0));
    
	cam.begin();
    
    ofPushMatrix();
    ofRotateX(rotation.x);
    ofRotateY(rotation.y);
    ofRotateZ(rotation.z);
    
	ofEnableDepthTest();
    //material.begin();
    ofEnableLighting();
    light.enable();
    
    // normals debug
    if(ofGetKeyPressed('n')){
        ofDisableLighting();
        vector<ofVec3f> n = mesh.getNormals();
        vector<ofVec3f> v = mesh.getVertices();
        float normalLength = 5.;
        ofSetColor(255);
        for(int i=0; i < n.size() ;i++){
            ofLine(v[i].x,v[i].y,v[i].z,
                   v[i].x+n[i].x*normalLength,v[i].y+n[i].y*normalLength,v[i].z+n[i].z*normalLength);
            ofLine(.98*v[i].x,.98*v[i].y,.98*v[i].z,
                   .98*v[i].x+n[i].x*normalLength*.2,.98*v[i].y+n[i].y*normalLength*.2,.98*v[i].z+n[i].z*normalLength*.2);
            ofLine(.98*v[i].x+n[i].x*normalLength*.2,.98*v[i].y+n[i].y*normalLength*.2,.98*v[i].z+n[i].z*normalLength*.2,
                   v[i].x+n[i].x*normalLength*.2,v[i].y+n[i].y*normalLength*.2,v[i].z+n[i].z*normalLength*.2);
        }
        ofEnableLighting();
    }
    
    if(ofGetKeyPressed('w')) mesh.drawWireframe();
    else mesh.draw();
    //light.draw();
	
    light.disable();
    ofDisableLighting();
	ofDisableDepthTest();
    //material.end();
    ofPopMatrix();
	cam.end();
    
    tuioClient.drawCursors();
	
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}


void testApp::keyPressed(int key){
	if(key == 'f') {
		ofToggleFullscreen();
	}
    else if (key == 's'){
        smoothLighting = !smoothLighting; 
        ofSetSmoothLighting(smoothLighting);
    }
}


void testApp::keyReleased(int key){
	
}


void testApp::mouseMoved(int x, int y){
	
}


void testApp::mouseDragged(int x, int y, int button){
	
}


void testApp::mousePressed(int x, int y, int button){
	
}


void testApp::mouseReleased(int x, int y, int button){
	
}


void testApp::windowResized(int w, int h){
	
}


void testApp::gotMessage(ofMessage msg){
	
}


void testApp::dragEvent(ofDragInfo dragInfo){ 
	
}

void testApp::tuioAdded(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
	//cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
}

void testApp::tuioUpdated(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
	//cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << endl;
}

void testApp::tuioRemoved(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
	//cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
}
