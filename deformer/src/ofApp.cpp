#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    cam.setDistance(100);
    sphere.set(30, 80);
    sphereBase.set(30, 80);
    
    pointLight.setDiffuseColor( ofColor(0.f, 255.f, 0.f));
    pointLight.setSpecularColor( ofColor(255.f, 255.f, 0.f));
    pointLight.setPointLight();
    
    spotLight.setDiffuseColor( ofColor(255.f, 0.f, 0.f));
    spotLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    spotLight.setSpotlight();
    // angle range between 0 - 90 in degrees
    spotLight.setSpotlightCutOff( 50 );
    // range 0 - 128, zero is even illumination, 128 is max falloff
    spotLight.setSpotConcentration( 45 );
    
    directionalLight.setDiffuseColor(ofColor(0.f, 0.f, 255.f));
    directionalLight.setSpecularColor(ofColor(255.f, 255.f, 255.f));
    directionalLight.setDirectional();
    directionalLight.setOrientation( ofVec3f(0, 90, 0) );
    
    bShiny = true;
    // 0 - 128
    material.setShininess( 10 );
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    
    bPointLight = bDirLight = true;
    bSpotLight = false;
    
    ofDisableArbTex();
    ofLogoImage.load("of.png");
    bUseTexture = true;
    
    // GUI
    isGuiVisible = true;
    noiseParams.setName("settings");
    noiseParams.add(noiseIn.set("input", ofVec3f(0), ofVec3f(-100), ofVec3f(100)));
    noiseParams.add(noiseInDiv.set("divide in", 100, 1, 1000));
    noiseParams.add(noiseOutMult.set("mult out", 10, 1, 100));
    noiseParams.add(isNoiseFromNormal.set("from normals", false));
    gui.setup(noiseParams);
    gui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::update(){
    pointLight.setPosition(cos(ofGetElapsedTimef()*.6f) * 60,
                           sin(ofGetElapsedTimef()*.8f) * 60,
                           -cos(ofGetElapsedTimef()*.8f) * 60);
    spotLight.setOrientation( ofVec3f( 0, cos(ofGetElapsedTimef()) * RAD_TO_DEG, 0) );
    spotLight.setPosition( mouseX, mouseY, 200);
    
    auto mesh = sphereBase.getMeshPtr();
    auto updateMesh = sphere.getMeshPtr();
    
    for (int i=0; i<mesh->getNumVertices(); i++) {
        auto vertex = mesh->getVertex(i);
        auto normal = mesh->getNormal(i);
        // calc noise distortion
        auto noiseVert = vertex / noiseInDiv;
        if (isNoiseFromNormal) {
            // use normals
            //noiseVert = normal / noiseInDiv;
            // Sydney Opera House
            float scalar = i % 40;
            // fixed spiral
            //float scalar = 0;
            //if (i % 80 == 0) scalar = 1;
            noiseVert = ofVec3f(scalar, scalar, scalar) * noiseInDiv;
        }
        float noise = ofNoise(noiseVert.z*noiseIn.get().x, noiseVert.x*noiseIn.get().y, noiseVert.y*noiseIn.get().z) -0.5;
        vertex += normal * (noise * noiseOutMult);
        // update vert
        updateMesh->setVertex(i, vertex);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackgroundGradient(ofColor(255), ofColor(100));
    ofEnableDepthTest();
    ofEnableLighting();
    cam.begin();
    material.begin();
    
    // lights
    if (bPointLight) pointLight.enable();
    if (bSpotLight) spotLight.enable();
    if (bDirLight) directionalLight.enable();
    
    
    ofSetColor(255);
    //vbo.drawWireframe();
    if (ofGetKeyPressed('w')) sphere.draw(OF_MESH_WIREFRAME);
    else sphere.draw();
    
    // lights
    if (!bPointLight) pointLight.disable();
    if (!bSpotLight) spotLight.disable();
    if (!bDirLight) directionalLight.disable();
    
    material.end();
    cam.end();
    ofDisableLighting();
    
//    ofSetColor( pointLight.getDiffuseColor() );
//    if(bPointLight) pointLight.draw();
//    
//    ofSetColor(255, 255, 255);
//    ofSetColor( spotLight.getDiffuseColor() );
//    if(bSpotLight) spotLight.draw();
    
    
    ofDisableDepthTest();
    ofSetColor(255);
    if (isGuiVisible) gui.draw();
}

void ofApp::createSphere(ofVboMesh *vbo, float radius, unsigned int rings, unsigned int sectors)
{
    ofPrimitiveMode p;
    vbo->setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
    float sphereCompleteness = 1.0; // 1.0 for whole sphere, 0.5 for hemisphere
    float const R = 1./(float)(rings-1) * sphereCompleteness;
    float const S = 1./(float)(sectors-1);
    int r, s;
    
    for(r = 0; r < rings; r++) {
        for(s = 0; s < sectors; s++) {
            float const y = sin( -M_PI_2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );
            vbo->addTexCoord( ofVec2f(s*S, r*R) );
            vbo->addVertex(ofVec3f(x * radius, y * radius, z * radius));
            vbo->addNormal(ofVec3f(x,y,z));
        }
    }
    
    
    for(r = 0; r < rings; r++) {
        for(s = 0; s < sectors; s++) {
            vbo->addIndex( r * sectors + s );
            vbo->addIndex( r * sectors + (s+1));
            vbo->addIndex( (r+1) * sectors + (s+1));
            vbo->addIndex( (r+1) * sectors + s);
        }  
    }  
}

void ofApp::createGeoSphere(int stacks, int slices) {
    
    //add vertices
    vbo.addVertex(ofVec3f(0,0,1));
    
    for (int i=1; i<stacks; i++) {
        double phi = PI * double(i)/stacks;
        double cosPhi = cos(phi);
        double sinPhi = sin(phi);
        for (int j=0; j<slices; j++) {
            double theta = TWO_PI * double(j)/slices;
            vbo.addVertex(ofVec3f(cos(theta)*sinPhi, sin(theta)*sinPhi, cosPhi));
        }
    }
    vbo.addVertex(ofVec3f(0,0,-1));
    
    //top row triangle fan
    for (int j=1; j<slices; j++) {
        vbo.addTriangle(0,j,j+1);
    }
    vbo.addTriangle(0,slices,1);
    
    //triangle strips
    for (int i=0; i < stacks-2; i++) {
        int top = i*slices + 1;
        int bottom = (i+1)*slices + 1;
        
        for (int j=0; j<slices-1; j++) {
            vbo.addTriangle(bottom+j, bottom+j+1, top+j+1);
            vbo.addTriangle(bottom+j, top+j+1, top+j);
        }
        
        vbo.addTriangle(bottom+slices-1, bottom, top);
        vbo.addTriangle(bottom+slices-1, top, top+slices-1);
    }
    
    //bottom row triangle fan
    int last = vbo.getNumVertices()-1;
    for (int j=last-1; j>last-slices; j--) {
        vbo.addTriangle(last,j,j-1);
    }
    vbo.addTriangle(last,last-slices,last-1);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case ' ':
            isGuiVisible = !isGuiVisible;
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case '1':
            bPointLight = !bPointLight;
            break;
        case '2':
            bSpotLight = !bSpotLight;
            break;
        case '3':
            bDirLight = !bDirLight;
            break;
        case 's':
            bShiny	= !bShiny;
            if (bShiny) material.setShininess( 120 );
            else material.setShininess( 10 );
            break;
        case 'x':
            bSmoothLighting = !bSmoothLighting;
            ofSetSmoothLighting(bSmoothLighting);
            break;
        case 't':
            bUseTexture = !bUseTexture;
            break;
        case OF_KEY_UP:
            // setSpotlightCutOff is clamped between 0 - 90 degrees //
            spotLight.setSpotlightCutOff(spotLight.getSpotlightCutOff()+1);
            break;
        case OF_KEY_DOWN:
            // setSpotlightCutOff is clamped between 0 - 90 degrees //
            spotLight.setSpotlightCutOff(spotLight.getSpotlightCutOff()-1);
            break;
        case OF_KEY_RIGHT:
            // setSpotConcentration is clamped between 0 - 128 //
            spotLight.setSpotConcentration(spotLight.getSpotConcentration()+1);
            break;
        case OF_KEY_LEFT:
            // setSpotConcentration is clamped between 0 - 128 //
            spotLight.setSpotConcentration(spotLight.getSpotConcentration()-1);
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
