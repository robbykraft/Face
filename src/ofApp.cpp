#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // MASKS
    CLMFT.setup();
//    ofSetWindowShape(CLMFT.grabber.getWidth(), CLMFT.grabber.getHeight());
    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    
    // SCREEN AND RESOLUTION
    windowCenter = ofPoint(ofGetScreenWidth()*.5, ofGetScreenHeight()*.5);
    float sw = (RESOLUTION_SCREEN_WIDTH/(float)RESOLUTION_CAMERA_WIDTH);
    float sh = (RESOLUTION_SCREEN_HEIGHT/(float)RESOLUTION_CAMERA_HEIGHT);
    if(sw > sh)   minCameraFitScale = sw;
    else          minCameraFitScale = sh;
    facePointsFrameScale = ofPoint(RESOLUTION_CAMERA_HEIGHT * minCameraFitScale,
                                   RESOLUTION_CAMERA_WIDTH * minCameraFitScale);
    
    ofSetFullscreen(true);

    
    // SCENES
    sceneManager.setup();
    attractScreen.setup();
    
    edgeImage.load("faded-edge.png");
    
    
    // GUI
    gui.setup();
    // appearance
    gui.add(screenBrightness.setup("camera brightness", .7, 0, 1));
    gui.add(faceDarkeningScale.setup("face causes dimming", .7, 0, 1));
    gui.add(lineThicknessSlider.setup("line weight", 2, 0.5, 5));
    gui.add(faceFoundZoomScale.setup("zoom in on face", .15, .02, .6));
    // animation
    gui.add(attractScreenWaitTime.setup("scr. saver delay", 15, 5, 120));
    gui.add(sceneDurationSlider.setup("scene duration", sceneManager.SCENE_INTERVAL, 5, 30));
    // admin and debug
    gui.add(cameraRotationToggle.setup("flip camera", false));
    gui.add(showFace.setup("show face dots", false));
    gui.add(enableMasterScale.setup("scale window", false));
    gui.add(masterScale.setup("  - scale", 1, .1, 2));
    // handlers
    lineThicknessSlider.addListener(this, &ofApp::lineThicknessSliderListener);
    sceneDurationSlider.addListener(this, &ofApp::sceneDurationSliderListener);
    faceFoundZoomScale.addListener(this, &ofApp::faceFoundZoomScaleListener);
    cameraRotationToggle.addListener(this, &ofApp::cameraRotationToggleListener);
    // gui position
    gui.setPosition(windowCenter);
    
    
    CLMFT.faceFoundZoomScale = faceFoundZoomScale;
    ofSetLineWidth(lineThicknessSlider);
}

//--------------------------------------------------------------
void ofApp::update(){
    // SCENES
    sceneManager.update();

    // MASKS
    CLMFT.update();

    // build face rotation/scale matrix to put the face in the center of the screen
    faceScaleMatrix.makeIdentityMatrix();
    faceScaleMatrix.translate( CLMFT.faceCenterSmooth.y * facePointsFrameScale.y,
                              -CLMFT.faceCenterSmooth.x * facePointsFrameScale.x,
                              0.0);
    faceScaleMatrix.scale(CLMFT.faceScaleSmooth, CLMFT.faceScaleSmooth, CLMFT.faceScaleSmooth);
    
    // deliver info to the scene manager
    sceneManager.faceFound = CLMFT.faceFound;
    sceneManager.faceScaleMatrix = faceScaleMatrix;
    sceneManager.faceCenterSmooth = ofPoint(-CLMFT.faceCenterSmooth.y * facePointsFrameScale.y,
                                            CLMFT.faceCenterSmooth.x * facePointsFrameScale.x);
    sceneManager.faceScaleSmooth = CLMFT.faceScaleSmooth;
    // face parts
    sceneManager.faceNose = ofPoint(-CLMFT.faceNose.y * facePointsFrameScale.y,
                                    CLMFT.faceNose.x * facePointsFrameScale.x);// - center;
    sceneManager.faceMouth = ofPoint(-CLMFT.faceMouth.y * facePointsFrameScale.y,
                                     CLMFT.faceMouth.x * facePointsFrameScale.x);// - center;
    sceneManager.faceLeftEye = ofPoint(-CLMFT.faceLeftEye.y * facePointsFrameScale.y,
                                       CLMFT.faceLeftEye.x * facePointsFrameScale.x);// - center;
    sceneManager.faceRightEye = ofPoint(-CLMFT.faceRightEye.y * facePointsFrameScale.y,
                                        CLMFT.faceRightEye.x * facePointsFrameScale.x);// - center;
    
    if(CLMFT.faceFound){
        lastFaceDetection = ofGetElapsedTimef();
    }
    if(ofGetElapsedTimef() > lastFaceDetection + attractScreenWaitTime){
        attractScreenBrightness = (ofGetElapsedTimef() - (lastFaceDetection + attractScreenWaitTime)) / 3.0;
    }
    else if(attractScreenBrightness > 0.0){
        attractScreenBrightness -= .05;
    }

    if(attractScreenBrightness < 0) attractScreenBrightness = 0;
    if(attractScreenBrightness > 1.0) attractScreenBrightness = 1.0;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    ofPushMatrix(); // window translate to center
        ofTranslate(windowCenter);
        if(enableMasterScale){
            ofScale(masterScale, masterScale);
        }
    
        ofPushMatrix();  // roving face-found zoom
            ofMultMatrix(faceScaleMatrix);

            ofPushMatrix();  // facePointsFrameScale, aspect fit camera onto screen
                // scale camera to fit inside of screen
                ofScale(minCameraFitScale, minCameraFitScale);
                // (sceneManager.masterFade)  in place of  (CLMFT.faceEnergy)
                ofSetColor(255 * screenBrightness - (sceneManager.masterFade) * 255 * screenBrightness * faceDarkeningScale, 255);
                CLMFT.drawCameraFeed();
                ofSetColor(255, (sceneManager.masterFade) * 255);
                edgeImage.draw(-RESOLUTION_CAMERA_WIDTH * .5,
                               -RESOLUTION_CAMERA_HEIGHT * .5,
                               RESOLUTION_CAMERA_WIDTH,
                               RESOLUTION_CAMERA_HEIGHT);
                if(showFace){
                    ofSetColor(255, 255);
                    CLMFT.drawFacePoints();
                }
            ofPopMatrix(); // facePointsFrameScale, aspect fit camera onto screen
    
//            ofPushMatrix();
//                ofRotate(-90 * CLMFT.cameraRotation);
//                ofSetColor(0, 128, 255);
//                ofDrawCircle(CLMFT.faceLeftEye * facePointsFrameScale, 10);
//                ofDrawCircle(CLMFT.faceRightEye * facePointsFrameScale, 10);
//                ofDrawCircle(CLMFT.faceNose * facePointsFrameScale, 10);
//                ofDrawCircle(CLMFT.faceMouth * facePointsFrameScale, 10);
//            ofPopMatrix();
    
        ofPopMatrix();  // roving face-found zoom
    
    
        // SCENES
        ofSetColor(255, 255);
        ofPushMatrix();
            sceneManager.draw();
        ofPopMatrix();

//        // ATTRACT SCREEN
//        if(attractScreenBrightness != 0.0){
//            ofPushMatrix();
//                ofSetColor(255, 100 * attractScreenBrightness);
//                attractScreen.update();
//                attractScreen.draw();
//            ofPopMatrix();
//        }
    
        // black border around screen
        if(enableMasterScale){
            ofNoFill();
            ofSetColor(0, 255);
            ofDrawRectangle(-ofGetScreenWidth() * .5, -ofGetScreenHeight() * .5, ofGetScreenWidth(), ofGetScreenHeight());
            ofFill();
        }
    
    ofPopMatrix(); // window translate to center
    
    if(showGUI){
        ofPushMatrix();
        ofSetColor(255, 255);
        gui.draw();
        ofPopMatrix();
    }
    
    // DEBUG TEXT
    ofSetColor(255,255);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);
    ofDrawBitmapString(ofToString(CLMFT.faceFound), 20, 40);
//    ofDrawBitmapString(ofToString(maskEnergy), 20, 60);
    ofDrawBitmapString(ofToString(CLMFT.faceEnergy), 20, 60);
//    ofDrawBitmapString(ofToString(faceScaleSmooth), 20, 80);
//    ofDrawBitmapString(ofToString(faceRect.getCenter().x), 20, 100);
//    ofDrawBitmapString(ofToString(ofGetWidth()*.5), 20, 120);
}


ofVec3f ofApp::worldToScreen(ofVec3f WorldXYZ, ofMatrix4x4 additionalTransform) {
    ofRectangle viewport = ofGetCurrentRenderer()->getCurrentViewport();
    //    OF_MATRIX_MODELVIEW, OF_MATRIX_PROJECTION,
    ofMatrix4x4 world = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW);
    ofVec3f CameraXYZ = WorldXYZ * additionalTransform * world;
    ofVec3f ScreenXYZ;
    ScreenXYZ.x = (CameraXYZ.x + 1.0f) / 2.0f * viewport.width + viewport.x;
    ScreenXYZ.y = (1.0f - CameraXYZ.y) / 2.0f * viewport.height + viewport.y;
    ScreenXYZ.z = CameraXYZ.z;
    return ScreenXYZ;
}

void ofApp::lineThicknessSliderListener(float &lineThicknessSlider){
    ofSetLineWidth(lineThicknessSlider);
}

void ofApp::sceneDurationSliderListener(float &sceneDurationSlider){
    sceneManager.SCENE_INTERVAL = sceneDurationSlider;
}
void ofApp::cameraRotationToggleListener(bool &cameraRotationToggle){
    if(cameraRotationToggle == 1)
        CLMFT.cameraRotation = 1;
    else if(cameraRotationToggle == 0)
        CLMFT.cameraRotation = -1;
}
void ofApp::faceFoundZoomScaleListener(float &faceFoundZoomScale){
    CLMFT.faceFoundZoomScale = faceFoundZoomScale;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' '){
        showGUI = !showGUI;
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
