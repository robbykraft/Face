#include "ConicsScene.h"

//--------------------------------------------------------------

void ConicsScene::setup(){
    for(int i = 0; i < NUM_CONES; i++){
        conicsL[i].setRadius(20 + i*5);
        conicsR[i].setRadius(20 + i*5);
    }
    numCones = 1;

    center = ofPoint(ofGetWidth()*.5, ofGetHeight()*.5);

    plane = center;
    planeNormal = ofVec3f(0, 100, 40);
}

//--------------------------------------------------------------
void ConicsScene::update(){
    
    plane = plane * .95 + faceCenterSmooth * .05;

    float SPEED = .2;
    float diff = .3 * sinf(ofGetElapsedTimef()*.2);
    for(int i = 0; i < numCones; i++){
        conicsL[i].setPosition( center + ofVec3f(-50 + 50*sinf(ofGetElapsedTimef()), 50, 0) );
        conicsL[i].setLookAt( center + ofVec3f(0, 0, 0) );
        conicsR[i].setPosition( center + ofVec3f(50, 50, 0) );
        conicsR[i].setLookAt( center + ofVec3f(0, 0, 0) );

//        conicsL[i].setPosition( faceLeftEye + ofVec3f(50, 50, 200) );
//        conicsL[i].setLookAt( faceLeftEye );
//        conicsR[i].setPosition( faceRightEye + ofVec3f(-50, 50, 200) );
//        conicsR[i].setLookAt( faceRightEye + ofVec3f(0, 0, 200) );

//        conicsL[i].setPosition( ofPoint(0 + i * 10,
//                                       0,
//                                       200 + 190 * sin(ofGetElapsedTimef() + i/100.0)) );
//        conicsL[i].setLookAt( faceLeftEye );
//        conicsR[i].setPosition( ofPoint(0 + i * 10,
//                                        0,
//                                        200 + 190 * sin(ofGetElapsedTimef() + i/100.0)) );
//        conicsR[i].setLookAt( faceRightEye );

//        conics[i].setPosition( ofPoint(i * 10,0, 200 + 190 * sin(ofGetElapsedTimef() + i/100.0)));
        //conics[i].setHeight(300 + 250 * sinf(2*i+ofGetElapsedTimef()));
//        conics[i].setRadius(ofGetMouseX() + (ofGetMouseX()*0.9) * ofSignedNoise(ofGetElapsedTimef()*.3, i/10.0));
        
//        conics[i].setLookAt( ofPoint(200 + 100 * sin(ofGetElapsedTimef() + i/100.0), 0 + 50 * ofNoise(ofGetElapsedTimef()*0.1, i/200.0)));
//        conics[i].setLookAt( ofPoint(200 + 100 * sin(ofGetElapsedTimef() + i/100.0), 0 + 50 * ofNoise(ofGetElapsedTimef()*0.1, i/200.0)));
        
    }
    if(numCones < NUM_CONES)
        numCones++;
}

void ConicsScene::reset(){
    resetMoment = ofGetElapsedTimef();
    numCones = 1;
}

//--------------------------------------------------------------
void ConicsScene::draw(){
    
    ofClear(0);
    
//    ofMultMatrix(faceScaleMatrix);
    
    ofSetLineWidth(1);
    
    ofSetColor(255, 255);
//    ofDrawAxis(20);
//    ofSetColor(0, 255, 0, 255);
//    ofDrawSphere(ofVec3f(0, 0, 0), 10);
    
//    ofSetColor(0, 0, 255, 180);
//    ofDrawSphere(faceNose, 10);
//    ofDrawSphere(faceMouth, 10);
//    ofDrawSphere(faceLeftEye, 10);
//    ofDrawSphere(faceRightEye, 10);
//    printf("%f", faceNose.x);
    
//    ofSetColor(0, 255, 0, 180);
//    ofDrawSphere(plane, 10);

    
    //    ofDrawAxis(20);
    
    //    ofSetColor(255, 10);
    //    for(int i = 0 ;i < NUM_CONES; i++)
    //        conics[i].draw();
    
    ofSetColor(255, 255);
    
//    plane = faceNose;
//    for(int i = 0 ;i < numCones; i++){
//        conicsL[i].drawIntersectionsWithPlane(center + plane, planeNormal);
//        conicsR[i].drawIntersectionsWithPlane(center + plane, planeNormal);
//    }
    //    ofSetColor(255, 0, 0, 255);
    //    for(int i = 0 ;i < NUM_CONES; i++)
    //        conics[i].drawIntersectionsWithPlane(movingPlane, movingPlaneNormal);
    
//    for(int i = 0 ;i < numCones; i++){
//        if(coneAnimate){
//            conics[i].setLookAt( ofVec3f(cosf(ofGetElapsedTimef()*coneLookPeriod + i*coneLookPhase),
//                                         sinf(ofGetElapsedTimef()*coneLookPeriod + i*coneLookPhase),
//                                         1) );
//            conics[i].setPosition( ofVec3f(coneMagnitude * cosf(ofGetElapsedTimef()*conePeriod + i*conePhase),
//                                           coneMagnitude * sinf(ofGetElapsedTimef()*conePeriod + i*conePhase),
//                                           coneZMagnitude * sinf(ofGetElapsedTimef()*coneZPeriod + i*coneZPhase)) );
//        }
//        plane = ofVec3f(planeXY*cosf(ofGetElapsedTimef()*planeXYPeriod + i*planeXYPhase),
//                        planeXY*sinf(ofGetElapsedTimef()*planeXYPeriod + i*planeXYPhase),
//                        planeZ + (planeZ*.9) * sinf(ofGetElapsedTimef()*planeZPeriod + i*planeZPhase) );
//        
//        planeNormal = ofVec3f(planeXYTilt * cosf(ofGetElapsedTimef()*planeXYTiltPeriod+i*planeXYTiltPhase),
//                              planeXYTilt * sinf(ofGetElapsedTimef()*planeXYTiltPeriod+i*planeXYTiltPhase),
//                              planeZTilt+ (planeZTilt*.9) * sinf(ofGetElapsedTimef()*planeZTiltPeriod + i*planeZTiltPhase) );
//        
//        conics[i].drawIntersectionsWithPlane(plane, planeNormal);
//    }
    
}