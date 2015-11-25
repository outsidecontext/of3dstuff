#include "ofApp.h"

#define UV_SPHERE_RES 120
#define NOISE_MODE_COUNT 13
int niceNoiseMods[NOISE_MODE_COUNT] = {3,4,5,6,7,10,12,20,24,30,120,240,241};
int niceNoiseModI = 0;

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    depthOfField.setup(ofGetWidth(), ofGetHeight());
    cam.setDistance(100);
    sphere.set(30, UV_SPHERE_RES);
    sphereBase.set(30, UV_SPHERE_RES);
    ofSetSmoothLighting(true);
    
    phongShader.load("Shaders/Phong/Phong.vert", "Shaders/Phong/Phong.frag");
    
    // cylinder
    //cylinder.set(30, 80, 80, 80, 20, false, OF_PRIMITIVE_TRIANGLE_STRIP);
    //cylinderBase.set(30, 80, 80, 80, 20, false, OF_PRIMITIVE_TRIANGLE_STRIP);
    // or icosphere
    cylinder.set(30, 6);
    cylinderBase.set(30, 6);

    
    pointLight.setDiffuseColor( ofColor(0.f, 255.f, 0.f));
    pointLight.setSpecularColor( ofColor(255.f, 255.f, 0.f));
    pointLight.setPointLight();
    
    spotLight.setDiffuseColor( ofColor(250.f, 250.f, 250.f));
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
    material.setShininess( 30 );
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    
    bDirLight = true;
    bSpotLight = true;
    bPointLight = false;
    
    ofDisableArbTex();
    ofLogoImage.load("of.png");
    bUseTexture = true;
    
    // GUI
    isGuiVisible = true;
    gui.setup();
    
    noiseParams.setName("settings");
    noiseParams.add(noiseIn.set("input", ofVec3f(0), ofVec3f(-100), ofVec3f(100)));
    noiseParams.add(noiseInDiv.set("divide in", 500, 1, 1000));
    noiseParams.add(noiseOutMult.set("mult out", 10, 1, 100));
    noiseParams.add(isNoiseFromNormal.set("from normals", false));
    noiseParams.add(noiseVertMod.set("vert mod", 30, 1, 241));
    gui.add(noiseParams);
    
    dofParams.add(isDofEnabled.set("enabled", false));
    dofParams.add(focalDistance.set("focal distance", 23, 0, 150));
    dofParams.add(focalRange.set("focal range", 4, 0, 10));
    dofParams.add(blurAmount.set("blur amount", 0.7, 0, 3));
    gui.add(dofParams);
    
    gui.add(isPhongShaderOn.set("phong shader", false));
    gui.add(isCylinderActive.set("cyclinder", false));
    gui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::update(){
    pointLight.setPosition(cos(ofGetElapsedTimef()*.6f) * 60,
                           sin(ofGetElapsedTimef()*.8f) * 60,
                           -cos(ofGetElapsedTimef()*.8f) * 60);
    spotLight.setPosition( 60, 60, 200);
    spotLight.lookAt(ofVec3f(30, 0, -50));
    
    auto mesh = sphereBase.getMeshPtr();
    auto MeshOut = sphere.getMeshPtr();
    updateMesh(mesh, MeshOut);
    if (isCylinderActive) {
        mesh = cylinderBase.getMeshPtr();
        MeshOut = cylinder.getMeshPtr();
        updateMesh(mesh, MeshOut);
    }
    if (ofGetKeyPressed('r')) {
        if (isCylinderActive) setNormals(*MeshOut);
        else updateNormals(MeshOut);
    }
    
    // update depth of field
    depthOfField.setBlurAmount(blurAmount);
    depthOfField.setFocalDistance(focalDistance);
    depthOfField.setFocalRange(focalRange);
    
}

void ofApp::updateMesh(ofMesh* mesh, ofMesh* MeshOut) {
    for (int i=0; i<mesh->getNumVertices(); i++) {
        auto vertex = mesh->getVertex(i);
        auto normal = mesh->getNormal(i);
        // calc noise distortion
        auto noiseVert = vertex / noiseInDiv;
        if (isNoiseFromNormal) {
            // use normals
            noiseVert = MeshOut->getNormal(i) / noiseInDiv;
            // Sydney Opera House
            //float scalar = i % noiseVertMod;
            //noiseVert = ofVec3f(scalar, scalar, scalar) * noiseInDiv;
        }
        // simplex noise creates uneven shapes
        float noise = ofNoise(noiseVert.z*noiseIn.get().z, noiseVert.x*noiseIn.get().x, noiseVert.y*noiseIn.get().y) -0.5;
        // sin multiplier gives more even lumps
        //noise = sin(noiseVert.z*noiseIn.get().x) * sin(noiseVert.x*noiseIn.get().y) * sin(noiseVert.y*noiseIn.get().z);
        // cos multiplier gives more even lumps
        //noise = cos(noiseVert.z*noiseIn.get().x) * cos(noiseVert.x*noiseIn.get().y) * cos(noiseVert.y*noiseIn.get().z);
        
        //if (noise > 0.2) noise = 0.5;
        //if (noise < -0.2) noise = -0.5;
        
        auto nextVertex = vertex += normal * (noise * noiseOutMult);
        auto lastVert = MeshOut->getVertex(i);
        vertex.x = ofLerp(lastVert.x, nextVertex.x, 0.1);
        vertex.y = ofLerp(lastVert.y, nextVertex.y, 0.1);
        vertex.z = ofLerp(lastVert.z, nextVertex.z, 0.1);
        
        // update vert
        MeshOut->setVertex(i, vertex);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    if (isDofEnabled) depthOfField.begin();
    ofBackgroundGradient(ofColor(255), ofColor(100));
    ofEnableDepthTest();
    cam.begin();
    
    // lights
    ofEnableLighting();
    if (bPointLight) pointLight.enable();
    if (bSpotLight) spotLight.enable();
    if (bDirLight) directionalLight.enable();
    
    ofSetColor(255);
    if (isPhongShaderOn) {
        phongShader.begin();
        phongShader.setUniform1i("numLights", 1);
    }
    
    material.begin();
    sphere.pan(.1);
    cylinder.pan(.1);
    
    if (ofGetKeyPressed('w')) {
        if (isCylinderActive) cylinder.draw(OF_MESH_WIREFRAME);
        else sphere.draw(OF_MESH_WIREFRAME);
    }
    else {
        if (isCylinderActive) {
            cylinder.draw();
        }
        else sphere.draw();
    }
    material.end();
    if (isPhongShaderOn) phongShader.end();
    
    // lights / DOF
    if (!bPointLight) pointLight.disable();
    if (!bSpotLight) spotLight.disable();
    if (!bDirLight) directionalLight.disable();
    ofDisableLighting();
    
    // normals debug
    auto mesh = sphere.getMeshPtr();
    if(ofGetKeyPressed('n')){
        vector<ofVec3f> n = mesh->getNormals();
        vector<ofVec3f> v = mesh->getVertices();
        float normalLength = 3;
        
        for(int i=0; i < n.size() ;i++){
            int rowEnd = (UV_SPHERE_RES*2) + 1;
            if (i % rowEnd == 0) ofSetColor(255, 255, 0);
            else if ( (i+1) % rowEnd == 0 ) ofSetColor(0, 255, 255);
            else ofSetColor(255);
            //ofDrawBitmapString(ofToString(i), v[i].x+n[i].x*normalLength, v[i].y+n[i].y*normalLength ,v[i].z+n[i].z*normalLength);
            ofDrawLine(v[i].x, v[i].y, v[i].z,
                   v[i].x+n[i].x*normalLength, v[i].y+n[i].y*normalLength ,v[i].z+n[i].z*normalLength);
        }
    }
    
    cam.end();
    
    if (isDofEnabled) {
        depthOfField.end();
        if(ofGetKeyPressed('d')){
            depthOfField.drawFocusAssist(0, 0);
        }
        else{
            depthOfField.getFbo().draw(0, 0);
        }
    }
    
    ofDisableDepthTest();
    ofSetColor(255);
    if (isGuiVisible) gui.draw();
}


// This works for OF_PRIMITIVE_TRIANGLE_STRIP
// based on discussions here:
// http://devmaster.net/posts/6065/calculating-normals-of-a-mesh
// and http://www.gamedev.net/topic/634094-calculating-normals-for-triangle-strip-model-not-terrain/
//
void ofApp::updateNormals(ofMesh* mesh) {
    
    mesh->clearNormals();
    vector<ofVec3f>* normal_buffer = new vector<ofVec3f>[mesh->getNumVertices()];
    
    // store normals for each triangle/face
    for (int j=0; j<mesh->getNumIndices()-2; j++) {
        // cache the 3 verts that make up the current triangle/face
        ofVec3f v0 = mesh->getVertex(mesh->getIndex(j));
        ofVec3f v1 = mesh->getVertex(mesh->getIndex(j+1));
        ofVec3f v2 = mesh->getVertex(mesh->getIndex(j+2));
        
        // calculate the triangle/face normal e.g. a right angle from the face
        ofVec3f normal = (v1-v0).cross(v2-v0);
        
        // invert each even normal
        // this is because vert ordering alternates between CW and CCW
        if (j%2==0) normal *= -1;
        
        // Store the normal for each of the vertices that make up the face.
        normal_buffer[mesh->getIndex(j+0)].push_back( normal );
        normal_buffer[mesh->getIndex(j+1)].push_back( normal );
        normal_buffer[mesh->getIndex(j+2)].push_back( normal );
    }
    
    // TODO add normal per index instead?
    
    // now loop over all normals for each vertex
    // sum and normalise each one to get the average normal
    int n = mesh->getNumVertices();
    for( int i = 0; i < n; i++ ){
        ofVec3f normal;
        
        int index = i;
        // add together all the normals for the current vert
        for( int j = 0; j < normal_buffer[index].size(); j++ )
            normal += normal_buffer[index][j];
        
        // this is a bit funky but it works...
        // the vertices along the seam of the UV sphere overlap and produce slightly different normals
        // we need to average the normals along this seam
        // first work out the end of a strip using ( the sphere resolution * 2 ) + 1
        // then use mod to work out if this is the first or last vert of a strip
        // if it is, add the normals from the OTHER end of strip to this one so it can be averaged
        int rowEnd = (UV_SPHERE_RES*2) + 1;
        // start point
        if (i % rowEnd == 0 && i > 2) {
            index = i + (UV_SPHERE_RES*2);
            for( int j = 0; j < normal_buffer[index].size(); j++ )
                normal += normal_buffer[index][j];
        }
        // end point
        if ( (i+1) % rowEnd == 0  && i > 2) {
            index = i - (UV_SPHERE_RES*2);
            for( int j = 0; j < normal_buffer[index].size(); j++ )
                normal += normal_buffer[index][j];
        }
        
        normal.normalize();
        mesh->addNormal(normal);
    }
    
    delete [] normal_buffer;
}

bool ofApp::checkSphereSafeIndex(int i) {
    int rowEnd = (UV_SPHERE_RES*2) + 1;
    return (i % rowEnd != 0);
}

// This works for OF_PRIMITIVE_TRIANGLES
// from https://gist.github.com/patriciogonzalezvivo/5473484
void ofApp::setNormals( ofMesh &mesh ){
    
    //The number of the vertices
    int nV = mesh.getNumVertices();
    
    //The number of the triangles
    int nT = mesh.getNumIndices() / 3;
    
    vector<ofPoint> norm( nV ); //Array for the normals
    
    //Scan all the triangles. For each triangle add its
    //normal to norm's vectors of triangle's vertices
    for (int t=0; t<nT; t++) {
        
        //Get indices of the triangle t
        int i1 = mesh.getIndex( 3 * t );
        int i2 = mesh.getIndex( 3 * t + 1 );
        int i3 = mesh.getIndex( 3 * t + 2 );
        
        //Get vertices of the triangle
        const ofPoint &v1 = mesh.getVertex( i1 );
        const ofPoint &v2 = mesh.getVertex( i2 );
        const ofPoint &v3 = mesh.getVertex( i3 );
        
        //Compute the triangle's normal
        ofPoint dir = ( (v2 - v1).cross( v3 - v1 ) ).normalize();
        
        //Accumulate it to norm array for i1, i2, i3
        norm[ i1 ] += dir;
        norm[ i2 ] += dir;
        norm[ i3 ] += dir;
    }
    
    //Normalize the normal's length
    for (int i=0; i<nV; i++) {
        norm[i].normalize();
        norm[i] *= -1;
    }
    
    //Set the normals to mesh
    mesh.clearNormals();
    mesh.addNormals( norm );
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
            else material.setShininess( 30 );
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
            if (++niceNoiseModI > NOISE_MODE_COUNT-1) niceNoiseModI = 0;
            noiseVertMod = niceNoiseMods[niceNoiseModI];
            spotLight.setSpotConcentration(spotLight.getSpotConcentration()+1);
            break;
        case OF_KEY_LEFT:
            if (--niceNoiseModI < 0) niceNoiseModI = NOISE_MODE_COUNT-1;
            noiseVertMod = niceNoiseMods[niceNoiseModI];
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
