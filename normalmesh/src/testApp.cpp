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
    
    // shader and FBO
    bokehShader.load("shaders/bokeh.vert", "shaders/bokeh.frag");
    setupFbo();
    
    bias = 30.0;
    focus = .8;
    
    rotation.set(0,0,0);
    //osc.setup();
    
    tuioClient.start(3333);
    ofAddListener(tuioClient.cursorAdded,this,&testApp::tuioAdded);
	ofAddListener(tuioClient.cursorRemoved,this,&testApp::tuioRemoved);
	ofAddListener(tuioClient.cursorUpdated,this,&testApp::tuioUpdated);
}

void testApp::setupFbo(){
    ofFbo::Settings s;
    s.width = ofNextPow2(ofGetWidth());
    s.height = ofNextPow2(ofGetHeight());
    //s.textureTarget = GL_TEXTURE_2D;
    s.useDepth = true;
    s.depthStencilInternalFormat = GL_DEPTH_COMPONENT24;
    s.depthStencilAsTexture = true;
    s.numSamples = 4;
    fbo.allocate(s);
}


void testApp::update() {
    
    bool useTuioTouch = true;
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
    if (ofGetKeyPressed('g')) light.setOrientation( ofVec3f(0, y, 0) );
    
    // update z vertices using noise
	for (int i=0; i<mesh.getNumVertices(); i++) {
        ofVec3f vertex = mesh.getVertex(i);
        float z = ofNoise(
            vertex.x*ofGetElapsedTimef()*0.01,
            vertex.y*ofGetElapsedTimef()*0.01,
            vertex.z*0.01
        ) * 6;
        if (useTuioTouch) {
            for (tit=cursors.begin(); tit != cursors.end(); tit++) {
                ofxTuioCursor *blob = (*tit);
                float thresh = 3;
                blobx = ofMap(blob->getX(), 0, 1, -50, 50);
                bloby = ofMap(blob->getY(), 0, 1, 50, -50);
                if (blobx >= vertex.x-thresh && blobx <= vertex.x+thresh
                    && bloby >= vertex.y-thresh && bloby <= vertex.y+thresh) {
                    z = 6;
                }
            }
        }
        vertex.z = ofLerp(vertex.z, z, 0.1);
        
        //float r = ofMap(x, <#float inputMin#>, <#float inputMax#>, <#float outputMin#>, <#float outputMax#>)
        ofFloatColor colour = ofFloatColor(vertex.x/100.0, vertex.y/100.0, z/6);
        mesh.setColor(i, colour);
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
    
    fbo.begin();
    ofClear(0);
    
	cam.begin();
    ofPushMatrix();
    //ofRotateX(rotation.x);
    //ofRotateY(rotation.y);
    //ofRotateZ(rotation.z);
    
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
    
    fbo.end();
    
    // draw the bokeh shader
    drawShader();
    
    if(ofGetKeyPressed('d')){
        fbo.getTextureReference().draw(10, 10, 300, 300);
        fbo.getDepthTexture().draw(320, 10, 300, 300);
        ofSetColor(255, 0, 0);
        ofDrawBitmapString("bias: " + ofToString(bias), 20, 20);
        ofDrawBitmapString("focus: " + ofToString(focus), 20, 40);
        ofSetColor(255);
        tuioClient.drawCursors();
    }
	
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

void testApp::drawShader(){
    bokehShader.begin();
	glActiveTexture(GL_TEXTURE0);
    fbo.getTextureReference().bind();
	glActiveTexture(GL_TEXTURE1);
    fbo.getDepthTexture().bind();
    bokehShader.setUniform1i("tex0", 0);
    bokehShader.setUniform1i("tex1", 1);
    bokehShader.setUniform1f("blurclamp", 100.0f);
    bokehShader.setUniform1f("bias", bias);
    bokehShader.setUniform1f("focus", focus);
    float w = fbo.getWidth();
    float h = fbo.getHeight();
	// draw full screen quad 
	glBegin(GL_QUADS);  
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, h);
	glVertex3f(0, h, 0);  
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	glVertex3f(0, 0, 0);  
	glMultiTexCoord2f(GL_TEXTURE0, w, 0.0f);
	glVertex3f(w, 0, 0);  
	glMultiTexCoord2f(GL_TEXTURE0, w, h);
	glVertex3f(w, h, 0);  
	glEnd();
    fbo.getDepthTexture().bind();
	glActiveTexture(GL_TEXTURE0);
    fbo.getTextureReference().unbind();
	bokehShader.end();
}


void testApp::keyPressed(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 's':
            smoothLighting = !smoothLighting; 
            ofSetSmoothLighting(smoothLighting);
            break;
        case 'P':
            bias += 10;
            break;
        case 'p':
            bias -= 10;
            break;
        case 'L':
            focus += .01;
            break;
        case 'l':
            focus -= .01;
            break;
        default:
            break;
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
	setupFbo();
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
