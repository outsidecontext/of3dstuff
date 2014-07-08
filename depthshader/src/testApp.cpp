#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {	
	quality = OF_IMAGE_QUALITY_WORST;
	maxSize = 2048;
	glitchStart = .0;
	shaderZOffset = 0;
	shaderZScaling = 2;
	rotSmooth = 0.8;
    
	videoGrabber.initGrabber(640, 480);
	
	shader.load("shaders/zdisplace.vert", "shaders/zdisplace.frag");
    
    fbo.allocate(ofGetWidth(), ofGetHeight());
    
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    int w = (int) fbo.getWidth();
	int h = (int) fbo.getHeight();
	int step = 4;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			ofPoint texCoords;
			texCoords = fbo.getTextureReference().getCoordFromPoint(x, y);
            mesh.addTexCoord(texCoords);
            mesh.addVertex(ofVec3f(x, y, 0));
			texCoords = fbo.getTextureReference().getCoordFromPoint(x, y + step);
            mesh.addTexCoord(texCoords);
            mesh.addVertex(ofVec3f(x, y+step, 0));
		}
	}
    
	
	ofBackground(255, 255, 255, 1);
	reset();
}

//--------------------------------------------------------------
void testApp::reset() {
	generation = 0;
	//img.allocate(512, 512, OF_IMAGE_COLOR_ALPHA);
	img.loadImage("a.jpg");
	// pick random for bleeding
	addX = ofRandom(0, 16);
	addY = ofRandom(0, 16);
	subX = ofRandom(0, addX);
	subY = ofRandom(0, addY);
}

//--------------------------------------------------------------
void testApp::update() {
	
	videoGrabber.update();
	
	string curFilename = "compressed.jpg";
	
	int size = img.getWidth();
	img.saveImage(curFilename, quality);
	
	if(ofGetKeyPressed('g')) {
		// this portion glitches the jpeg file
		// first loading the file
		ofBuffer file = ofBufferFromFile(curFilename);
		int fileSize = file.size();
		char * buffer = file.getBinaryBuffer();
		
		// pick a byte offset that is somewhere near the end of the file
		int whichByte = (int) ofRandom(fileSize * glitchStart, fileSize);
		// and pick a bit in that byte to turn on
		int whichBit = ofRandom(8);
		char bitMask = 1 << whichBit;
		// using the OR operator |, if the bit isn't already on this will turn it on
		buffer[whichByte] |= bitMask;
		
		// write the file out like nothing happened
		ofBufferToFile(curFilename, file);
		img.loadImage(curFilename);
	}
    
    fbo.begin();
    //videoGrabber.draw(0, 0, fbo.getWidth(), fbo.getHeight());
    img.draw(0, 0, fbo.getWidth(), fbo.getHeight());
    fbo.end();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw() {
	
    drawWithShader();
    
//    ofSetColor(60);
//    ofPlanePrimitive plane = ofPlanePrimitive(ofGetWidth(), ofGetHeight(), 100, 100);
//    ofPushMatrix();
//    ofTranslate(ofGetWidth()*.5, ofGetHeight()*.5);
//    if (ofGetKeyPressed('r')) {
//        rotX = ofLerp(mouseX, rotX, rotSmooth);
//        rotY = ofLerp(mouseY, rotY, rotSmooth);
//    }
//    glRotatef(rotX, 0, 1, 0);
//    glRotatef(-rotY, 1, 0, 0);
//    plane.drawWireframe();
//    ofPopMatrix();
    
	//ofSetColor(0);
	//ofRect(5, 5, 290, 45);
	//ofSetColor(255);
	//ofDrawBitmapString("Currently on generation " + ofToString(generation), 10, 20);
	//ofDrawBitmapString("Click to reset, hold 'g' to glitch.", 10, 40);
	//ofDrawBitmapString(ofToString(ofGetFrameRate()) + " fps", 10, 20);
}

void testApp::drawWithShader(){
	
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	
	// Centre and rotate
    int w = (int) fbo.getWidth();
    int h = (int) fbo.getHeight();
    glTranslatef(ofGetWidth() / 2, ofGetHeight() / 2 + 100, 0);
    if (ofGetKeyPressed('r')) {
        rotX = ofLerp(mouseX, rotX, rotSmooth);
        rotY = ofLerp(mouseY, rotY, rotSmooth);
    }
    glRotatef(rotX, 0, 1, 0);
    glRotatef(-rotY, 1, 0, 0);
    glTranslatef(-w / 2, -h / 2, -0);
	
    
    
    
	// Start shader and bind textures
	shader.begin();
	glActiveTexture(GL_TEXTURE0);
	fbo.getTextureReference().bind();
	
	// Set shader params
	shader.setUniform1i("tex0", int(GLuint(0)));
    
    float scale = ofMap(ofGetMouseX(), 0, ofGetWidth(), 1, 500);
    // force scale
//    scale = 1;
    
    shader.setUniform1f("scale", scale);
    
	// draw mesh
    if (ofGetKeyPressed('p')) {
        mesh.drawVertices();
    } else if (ofGetKeyPressed('w')) {
        mesh.drawWireframe();
    } else {
        mesh.draw();
    }
    
//    mesh.drawWireframe();
    
	// end shader
	shader.end();
	// unbind textures
	glActiveTexture(GL_TEXTURE0);
	fbo.getTextureReference().unbind();
	
	// tidy up
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	reset();
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
	
}
