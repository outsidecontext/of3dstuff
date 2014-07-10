#include "testApp.h"


void testApp::setup(){
    ofDisableArbTex();
    
    string dir = "images/earth-high/";
    colormap.loadImage(dir + "texture.png");
    bumpmap.loadImage(dir + "bump.jpg");
    normalMap.loadImage(dir + "normal.png");
    specularMap.loadImage(dir + "specular.png");
    //bumpShader.setGeometryOutputType(GL_TRIANGLE_STRIP); // type: GL_POINTS, GL_LINE_STRIP or GL_TRIANGLE_STRIP
    bumpShader.load("shaders/displace.vert", "shaders/displace.frag");
    
    gui.setup();
    gui.add(lightX.setup("light.x", 330, -3000, 3000));
    gui.add(lightY.setup("light.y", 300, -3000, 3000));
    gui.add(lightZ.setup("light.z", -90, -3000, 3000));
    gui.add(lightShininess.setup("light.shininess", 64, 0, 128));
    gui.add(bumpScale.setup("bump", 1, 0, 60));
    // light controls, set some default light/material colours
    ofFloatColor min = ofFloatColor(0.0);
    ofFloatColor max = ofFloatColor(1.0);
    gui.add(lightAmbient.setup("light ambient", ofFloatColor(0.5, 0.5, 0.5, 1.0), min, max));
    gui.add(lightDiffuse.setup("light diff", ofFloatColor(0.95, 0.95, 0.95, 1.0), min, max));
    gui.add(matAmbient.setup("mat ambient", ofFloatColor(0.3, 0.3, 0.3, 1.0), min, max));
    gui.add(matDiffuse.setup("mat diff", ofFloatColor(0.96, 0.96, 0.96, 1.0), min, max));
    
    sphere.set(200, 200);
    
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
    
    ofEnableDepthTest();
    cam.begin();
    enableLights();
    
    // Bump & normal shader which takes into account glLightfv and glMaterialfv settings
    bumpShader.begin();
    bumpShader.setUniform1i( "NumEnabledLights", 1 );
    bumpShader.setUniformTexture("colormap", colormap, 1);
    bumpShader.setUniformTexture("bumpmap", bumpmap, 2);
    bumpShader.setUniformTexture("normalMap", normalMap, 3);
    bumpShader.setUniformTexture("glossMap", specularMap, 4);
    bumpShader.setUniform1i("maxHeight", bumpScale);
    sphere.draw();
    // This will show the ofSphere normals BEFORE they're recalculated on the GPU
    if (ofGetKeyPressed('n')) sphere.drawNormals(2);
    glDisable(GL_LIGHTING);
    bumpShader.end();
    
    // Debug draw the light position and colour
    ofSetColor((ofFloatColor)lightDiffuse);
    ofDrawSphere(lightX, lightY, lightZ, 10);
    ofSetColor(255);
    
    cam.end();
    ofDisableDepthTest();
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
    
    // light settings, some are GUI controlled
    GLfloat position[] = { lightX, lightY, lightZ, 1 };
    ofFloatColor l = lightAmbient;
    GLfloat light_ambient[] = { l.r, l.g, l.b, l.a };
    l = lightDiffuse;
    GLfloat light_diffuse[] = { l.r, l.g, l.b, l.a };
    
    // material settings, some are GUI controlled
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_emission[] = { 0.4, 0.7, 1.0, 0.0 };
    l = matAmbient;
    GLfloat mat_ambient[] = { l.r, l.g, l.b, l.a };
    l = matDiffuse;
    GLfloat mat_diffuse[] = { l.r, l.g, l.b, l.a };
    
    // light components
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    //glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0f );
	//glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f );
	//glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.00015f );
    
    // material components
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, lightShininess);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat );
    
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
