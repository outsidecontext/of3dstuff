#include "testApp.h"


void testApp::setup(){
    ofDisableArbTex();
    
    colormap.loadImage("images/earth-texture.jpg");
    bumpmap.loadImage("images/earth-normal.jpg");
    //bumpShader.setGeometryOutputType(GL_TRIANGLE_STRIP); // type: GL_POINTS, GL_LINE_STRIP or GL_TRIANGLE_STRIP
    bumpShader.load("shaders/displace.vert", "shaders/displace.frag");
    
    gui.setup();
    gui.add(lightX.setup("light.x", 0, -3000, 3000));
    gui.add(lightY.setup("light.y", 0, -3000, 3000));
    gui.add(lightZ.setup("light.z", 0, -3000, 3000));
    gui.add(lightShininess.setup("light.shininess", 64, 0, 128));
    gui.add(bumpScale.setup("bump", 1, 0, 20));
    
    sphere.set(100, 100);
    
    ofFbo::Settings s;
    s.width = ofNextPow2(ofGetWidth());
    s.height = ofNextPow2(ofGetHeight());
    s.textureTarget = GL_TEXTURE_2D;
    s.useDepth = true;
    //s.depthStencilInternalFormat = GL_DEPTH_COMPONENT24;
    s.depthStencilAsTexture = true;
    fbo.allocate(s);
}


void testApp::update(){
                  
}



void testApp::drawScene() {
    
    bumpShader.begin();
    bumpShader.setUniform1i( "NumEnabledLights", 1 );
    bumpShader.setUniformTexture("colormap", colormap, 1);
    bumpShader.setUniformTexture("bumpmap", bumpmap, 2);
    bumpShader.setUniform1i("maxHeight", bumpScale);
    
    ofEnableDepthTest();
    cam.begin();
    enableLights();
    colormap.getTextureReference().bind();
    sphere.draw();
    if (ofGetKeyPressed('n')) sphere.drawNormals(2);
    colormap.getTextureReference().unbind();
    glDisable(GL_LIGHTING);
    cam.end();
    ofDisableDepthTest();
    
    bumpShader.end();
    
}

void testApp::draw(){
    ofBackground(0);
    /*
    fbo.begin();
    ofClear(0);
    drawScene();
    fbo.end();
    fbo.getDepthTexture().draw(0, 0, 100, 100);*/
    drawScene();
    gui.draw();
}


void testApp::enableLights(){
    
    glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // light params
    GLfloat position[]   = { lightX, lightY, lightZ, 1 };
    GLfloat no_mat[]			= { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[]		= { 0.6, 0.3, 0.4, 1.0 };
    GLfloat mat_diffuse[]		= { 0.3, 0.5, 0.8, 1.0 };
    GLfloat mat_specular[]		= { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_emission[]		= { 0.4, 0.7, 1.0, 0.0 };
    
    // light components
    //glLightfv(GL_LIGHT0, GL_AMBIENT, no_mat);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    //glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0f );
	//glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f );
	//glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.00015f );
    
    // material components
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, lightShininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat );
    
}


void testApp::keyPressed(int key){
        
}


void testApp::keyReleased(int key){

}


void testApp::mouseMoved(int x, int y ){

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
