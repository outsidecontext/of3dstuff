#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofDisableArbTex();
    //texture.setup("images/earth", 2500, 1250);
    texture.setup("images/5824", 512, 512);
    colormap.loadImage("images/earth-texture.jpg");
    bumpmap.loadImage("images/earth-normal.jpg");
    bumpShader.load("shaders/displace.vert", "shaders/displace.frag");
    normalShader.load("shaders/NormalMapShader.vert", "shaders/NormalMapShader.frag");
    
    gui.setup();
    gui.add(lightX.setup("light.x", 0, -3000, 3000));
    gui.add(lightY.setup("light.y", 0, -3000, 3000));
    gui.add(lightZ.setup("light.z", 0, -3000, 3000));
    gui.add(lightShininess.setup("light.shininess", 64, 0, 128));
    gui.add(bumpScale.setup("bump", 1, 0, 20));
    plane.set(50, 50, 25, 25);
    
    sphere.set(100, 100);
    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor(ofFloatColor(255.0, 0.0, 0.0));
    pointLight.setPointLight();
}

//--------------------------------------------------------------
void testApp::update(){
                  
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofEnableDepthTest();
    cam.begin();
    
    //texture.begin();
    //ofSphere(0, 0, 0, 100);
    //texture.end();
    
    bumpShader.begin();
    bumpShader.setUniformTexture("colormap", colormap, 1);
    bumpShader.setUniformTexture("bumpmap", bumpmap, 2);
    bumpShader.setUniform1i("maxHeight", bumpScale);
    ofEnableLighting();
    enableLights();
    sphere.draw();
    //sphere.drawNormals(2);
    ofDisableLighting();
    bumpShader.end();
    
    /*
    normalShader.begin();
    normalShader.setUniformTexture("bumpmap", colormap, 1);
    normalShader.setUniform1f("farThreshold", 255.0f);
    normalShader.setUniform1i("isThresholdOnly", 0);
    sphere.draw();
    normalShader.end();
     */
    
    
    cam.end();
    ofDisableDepthTest();
    gui.draw();
}

//--------------------------------------------------------------
void testApp::enableLights(){
    
    pointLight.enable();
    pointLight.setPosition(lightX, lightY, lightZ);
    
    /*
    // light params
    GLfloat position[]   = { lightX, lightY, lightZ, 1 };
    GLfloat ambient[]    = { .05, .05, .05, 1 };
    GLfloat diffuse[]    = { 1, 1, 1, .5 };
    GLfloat specular[]   = { 1, 1, 1, 1 };
    GLfloat reflection[] = { 1, 1, 1, 1 };
    GLfloat material[]   = { 1, 1, 1, 1 };
    
    // light components
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    // material components
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, reflection);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, lightShininess);
     */
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
        
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

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
