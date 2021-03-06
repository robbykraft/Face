#include "CirclesScene.h"

void CirclesScene::ofCircleSlice(float x,float y, float radius, float lowAngle, float highAngle, bool closed, bool radians){
//    if (!bSetupCircle) setupCircle();
    
    // use smoothness, if requested:
//    if (bSmoothHinted && drawMode == OF_OUTLINE)
//        startSmoothing();
    
    bool angleWrap = (lowAngle > highAngle); // are we doing the 0/360 wrap?
    
    if(!radians){
        lowAngle = ofDegToRad(lowAngle);
        highAngle = ofDegToRad(highAngle);
    }
    
    int numCirclePts = 100;
    float circlePtsScaled[numCirclePts*2];
    int res = numCirclePts;
    float angle = lowAngle;
    float angleRange = ((!angleWrap)?(highAngle - lowAngle):(M_TWO_PI - lowAngle + highAngle));
    float angleAdder = angleRange / (float)res;
    int k = 0;
    for (int i = 0; i < numCirclePts; i++){
        circlePtsScaled[k] = x + cos(angle) * radius;
        circlePtsScaled[k+1] = y - sin(angle) * radius;
        angle += angleAdder;
        k+=2;
    }
    
    //initial points
//    circlePtsScaled[0] = x;
//    circlePtsScaled[1] = y;
    k = 0;
    // now all the points around the circumference
    for (int i = 2; i < numCirclePts; i++){
        circlePtsScaled[k] = circlePtsScaled[k];
        circlePtsScaled[k+1] = circlePtsScaled[k+1];
        k+=2;
    }
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, &circlePtsScaled[0]);
//    glDrawArrays( (drawMode == OF_FILLED) ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, numCirclePts);
    glDrawArrays( GL_LINE_STRIP, 0, numCirclePts);
    
    
    // back to normal, if smoothness is on  
//    if (bSmoothHinted && drawMode == OF_OUTLINE) endSmoothing();  
}



CircleFromThreePoints::CircleFromThreePoints( const ofPoint& inner,
                                             const ofPoint& mid,
                                             const ofPoint& outer,
                                             double precision)
{
    // move to frame where inner.x == inner.y == 0;
    ofPoint b( mid.x-inner.x, mid.y-inner.y);
    ofPoint c( outer.x-inner.x, outer.y-inner.y);
    init( b, c, ofPoint( inner.x, inner.y), precision);
}

void CircleFromThreePoints::init( const ofPoint& b, const ofPoint& c,
                                 const ofPoint& offset, double precision)
{
    double b2 = b.lengthSquared();
    double c2 = c.lengthSquared();

    
    double oX(0), oY(0);
    bool solved = false;
    if (fabs(b.x) > fabs(b.y)) {    // solve for y first
        double k = c.x/b.x;
        double div = 2*(k*b.y - c.y);
        if (fabs(div) < precision) theCurvature = 0;  // if the 3 points lie on a line
        else {
            oY = (k*b2 - c2) / div;
            oX = b2/(2*b.x) - b.y/b.x * oY;
            solved = true;
        }
    }
    else {    // solve for x first
        double k = c.y/b.y;
        double div = 2*(k*b.x-c.x);
        if (fabs(div) < precision) theCurvature = 0;  // if the 3 points lie on a line
        else {
            oX = (k*b2 - c2) / div;
            oY = b2/(2*b.y) - b.x/b.y * oX;
            solved = true;
        }
    }
    if (solved) {
        theCurvature = 1./sqrt(oX*oX + oY*oY);
        double xC = oX + offset.x;
        double yC = oY + offset.y;
        theCenter = ofPoint( xC, yC);
        //    thePhi = acos(xC/sqrt(xC*xC + yC*yC));
        
        //    if (xC<0.) thePhi = thePhi - PI;
        //    cout << setiosflags(ios::showpoint | ios::fixed);
        //
        //    cout << "CircleFromThreePoints::init curv = " << theCurvature << endl;
        //    cout << "CircleFromThreePoints::init center prime = " << oX << " " << oY << endl;
        //    cout << "CircleFromThreePoints::init offset = " << offset.x << " " << offset.y << endl;
        //    cout << "CircleFromThreePoints::init center = " << theCenter.x<< " " << theCenter.y << endl;
        //
        //    float d = sqrt(theCenter.x*theCenter.x+theCenter.y*theCenter.y);
        //    cout << "CircleFromThreePoints::init dfloat = " << setw(10) << setprecision(5) << d << endl;
        //    cout << "CircleFromThreePoints::init radius = " << 1/theCurvature << endl;
    }
}


int CirclesScene::ClockWise(vector < ofPoint > pts){
    int i,j,k;
    int count = 0;
    double z;
    int n = pts.size();
    
    if (n < 3)
        return(0);
    
    for (i=0;i<n;i++) {
        j = (i + 1) % n;
        k = (i + 2) % n;
        z  = (pts[j].x - pts[i].x) * (pts[k].y - pts[j].y);
        z -= (pts[j].y - pts[i].y) * (pts[k].x - pts[j].x);
        if (z < 0)
            count--;
        else if (z > 0)
            count++;
    }
    if (count > 0)
        return(COUNTERCLOCKWISE);
    else if (count < 0)
        return(CLOCKWISE);
    else
        return(0);
}



void CirclesScene::traceAngleToAngle(circle & circle, ofPoint from, ofPoint to, ofPolyline & line, bool bClockwise, string name){
    
    
    vector < ofPoint > pts;
    pts.push_back(from);
    pts.push_back(circle.pos);
    pts.push_back(to);
    
    int clockwise = ClockWise(pts);
    
    ///ofDrawBitmapStringHighlight(name +  " " + (clockwise == CLOCKWISE ? "clockwise" : "counter") + " want " + (bClockwise ? "clockwise" : "counter"), circle.pos);
    
    
    ofPoint diffA = (from - circle.pos);
    ofPoint diffB = (to - circle.pos);
    
    float angleA = atan2( diffA.y, diffA.x );
    float  angleB = atan2( diffB.y, diffB.x );
    
    float angleDiff = angleB - angleA;
    if (angleDiff < -PI) angleDiff += TWO_PI;
    if (angleDiff > PI) angleDiff -= TWO_PI;
    
    if ( (clockwise == CLOCKWISE && !bClockwise) ||
        (clockwise == COUNTERCLOCKWISE && bClockwise)){
        
        
        float diff = TWO_PI - fabs(angleDiff);
        if (angleDiff > 0){
            diff *= -1;
        }
        angleDiff = diff;
    }
    //    if (angleDiff < -PI) angleDiff += TWO_PI;
    //    if (angleDiff > PI) angleDiff -= TWO_PI;
    //
    //
    //    if (det > 0 && !bClockwise){
    //
    //        if (angleDiff > 0){
    //            angleDiff = -TWO_PI + angleDiff;
    //        } else {
    //             //angleDiff = -TWO_PI + angleDiff;
    //        }
    //
    //        //angleDiff *= -1;
    //    }
    //
    //
    //    ofDrawBitmapStringHighlight(name +  " " + (bClockwise ? "clockwise" : "counter"), circle.pos);
    //
    for (int i = 0; i < 100; i++){
        float angle = angleA + (angleDiff/100.0) * i;
        line.addVertex(  circle.pos + circle.radius * ofPoint(cos(angle), sin(angle)));
    }
}



#define C_S_NUM_CIRCLES 3

//--------------------------------------------------------------
void CirclesScene::setup(){

    ofSetCircleResolution(100);
    
    for (int i = 0; i < C_S_NUM_CIRCLES; i++){
        circle temp;
        temp.radius = ofRandom(30,200);
        float angle = ofMap(i,0,10, 0, TWO_PI);
        temp.pos = ofPoint(0,0) + 200 * ofPoint(cos(angle), sin(angle));
       // temp.pos = ofPoint( );
        temp.vel = ofPoint(0,0);
        circles.push_back(temp);
    }
}


void CirclesScene::reset(){
    resetMoment = ofGetElapsedTimef();
    faceSmoothSlow = faceNose;
    faceSmoothFast = faceNose;
    faceEnergy = 0;
}


//--------------------------------------------------------------
void CirclesScene::update(){

    // FACE TRACK
    faceSmoothSlow = 0.9 * faceSmoothSlow + 0.1 * faceNose;
    faceSmoothFast = 0.3 * faceSmoothFast + 0.7 * faceNose;
    speed = ofDist(faceSmoothFast.x, faceSmoothFast.y, faceSmoothSlow.x, faceSmoothSlow.y);
    float speedToAdd = speed;
    if(speedToAdd > 30)
        speedToAdd = 30;
    faceEnergy+=speedToAdd*.2;
    faceEnergy *= .9;

    
    
    for (int i = 0; i < circles.size(); i++){
        circles[i].updateStart();
    }
    for (int i = 0; i < circles.size(); i++){
        // move to center
        ofPoint diff = ofPoint(0,0) - circles[i].pos;
        circles[i].addForce(diff/300.0);
        
        
        ofPoint diff2 = ofPoint(0,0) - circles[i].pos;
        if (diff2.length() < ofGetMouseX()){
            circles[i].addForce(-diff/600.0);
        }
        
    }
    ofPoint diff = faceNose - lastFacePosition;
    float d = sqrt( powf(diff.x, 2) + powf(diff.y, 2) );
    
    if (d > smoothFaceVelocity)
        smoothFaceVelocity = d;
//    printf("%f\n", smoothFaceVelocity);
    float a = 1;
    float b = 1;
    float c = 1;
    
    circles[0].radius = 100 + .05*faceEnergy * 50 * a * sin(ofGetElapsedTimef()*0.8+1.0);
    circles[1].radius = 50 +  .05*faceEnergy * 45 * b * sin(ofGetElapsedTimef());
    circles[2].radius = 50 +  .05*faceEnergy * 40 * c * sin(ofGetElapsedTimef()*1.2);
//    circles[3].radius = 50 + 40 * c * sin(ofGetElapsedTimef()*0.5)      + 15 * sin(ofGetElapsedTimef()*0.5);
//    circles[4].radius = 50 + 40 * c * sin(ofGetElapsedTimef()*0.3)      + 15 * sin(ofGetElapsedTimef()*0.3);
    
    lastFacePosition = faceNose;
    smoothFaceVelocity -= 1;

    for (int i = 0; i < circles.size(); i++){
        circles[i].updateEnd();
    }
    
    
    vector < int > randomOrder;
    for (int i = 0; i < circles.size(); i++){
        randomOrder.push_back(i);
    }
    ofRandomize(randomOrder);
    
    for (int i = 0; i < circles.size(); i++){
        // repel from each other
        for (int j = 0; j < i; j++){
            circles[randomOrder[i]].repelFrom(circles[randomOrder[j]]);
        }
    }
}

//--------------------------------------------------------------
void CirclesScene::draw(){
    
    // SCENE TO WINDOW CONVERSION
    ofTranslate(RESOLUTION_SCENE_WIDTH * .5, RESOLUTION_SCENE_HEIGHT * .5);
    ofScale(RESOLUTION_SCENE_WIDTH / (float)RESOLUTION_WINDOW_WIDTH,
            RESOLUTION_SCENE_HEIGHT / (float)RESOLUTION_WINDOW_HEIGHT);
    
    
//    ofPushMatrix();
//    ofSetColor(0, 128, 255);
//    ofDrawCircle(faceLeftEye * faceScaleMatrix, 10);
//    ofDrawCircle(faceRightEye * faceScaleMatrix, 10);
//    ofDrawCircle(faceNose * faceScaleMatrix, 10);
//    ofDrawCircle(faceMouth * faceScaleMatrix, 10);
//    ofPopMatrix();
    
    ofSetColor(255,255,255,90);
    for (int i = 0; i < circles.size(); i++){
        circles[i].draw();
    }
    
    for (int i = 0; i < circles.size(); i++){
        int i_p_1 = (i+1) % circles.size();
        ofDrawLine(circles[i].pos, circles[i_p_1].pos);
    }
//
//    ofSetColor(255,0,0);
//    ofLine(circles[0].pos, circles[1].pos);
//    
//    ofSetColor(255,255,0);
//    ofLine(circles[0].pos, circles[2].pos);
//    
//    
    ofPoint diffA = (circles[1].pos - circles[0].pos);
    ofPoint diffB = (circles[2].pos - circles[0].pos);
    
    float angleA = atan2( diffA.y, diffA.x );
    float angleB = atan2( diffB.y, diffB.x );
    
    float angleDiff = angleB - angleA;
    if (angleDiff < -PI) angleDiff += TWO_PI;
    if (angleDiff > PI) angleDiff -= TWO_PI;
    
    float newAngle = angleA - angleDiff;

    
    ofPolyline tempLine;
    
    
    for (int i = 0; i < circles.size(); i++){
        
        int i_m_1 = i-1;
        int i_p_1 = i+1;
        if (i_m_1 < 0) i_m_1 += circles.size();
        i_p_1 %= circles.size();
        
        traceAngleToAngle(circles[i], circles[i_m_1].pos, circles[i_p_1].pos, tempLine, i %2 == 1 ? ofGetMousePressed() : !ofGetMousePressed() , "0");
    }
    
    ofSetColor(255,255,255,90);
    
    tempLine = tempLine.getSmoothed(3);
    
    
    //tempLine.getVertices().erase(tempLine.getVertices().begin(), tempLine.getVertices().begin()+40);
    //tempLine.getVertices().erase(tempLine.getVertices().end()-40, tempLine.getVertices().end());
    
    ofSetColor(255,255,255);
    tempLine.draw();
    
    
    ofSetColor(255,255,255, 100);

    ofPoint slope;
    float angle;
    for(int i = 0; i< circles.size(); i++){
        slope = circles[i].pos - (faceLeftEye * faceScaleMatrix);
        angle = atan2(-slope.y, slope.x);
        ofSetColor(255,255,255, 100);
        ofDrawLine(faceLeftEye * faceScaleMatrix,
                   faceLeftEye * faceScaleMatrix + slope * 800);
        ofSetColor(255,255,255, 180);
        ofCircleSlice(circles[i].pos.x, circles[i].pos.y, circles[i].radius * 1.1, angle - .3, angle + .3, false, true);
        
        slope = circles[i].pos - (faceRightEye * faceScaleMatrix);
        angle = atan2(-slope.y, slope.x);
        ofSetColor(255,255,255, 100);
        ofDrawLine(faceRightEye * faceScaleMatrix,
                   faceRightEye * faceScaleMatrix + slope * 800);
        ofSetColor(255,255,255, 180);
        ofCircleSlice(circles[i].pos.x, circles[i].pos.y, circles[i].radius * 1.1, angle - .3, angle + .3, false, true);
        
        slope = circles[i].pos - (faceMouth * faceScaleMatrix);
        angle = atan2(-slope.y, slope.x);
        ofSetColor(255,255,255, 100);
        ofDrawLine(faceMouth * faceScaleMatrix,
                   faceMouth * faceScaleMatrix + slope * 800);
        ofSetColor(255,255,255, 180);
        ofCircleSlice(circles[i].pos.x, circles[i].pos.y, circles[i].radius * 1.1, angle - .3, angle + .3, false, true);
    }
    glLineStipple(1, 0x00FF);
    glEnable(GL_LINE_STIPPLE);
    
//
    ofPoint ptEnd = tempLine.getVertices()[tempLine.getVertices().size()-1];
    
    ofDrawCircle(ptEnd,  3);
    
    
    ofPoint ptBegin = tempLine.getVertices()[0];
    
    ofDrawCircle(ptBegin,  3);
    
    
    ofPoint linePt = circles[0].pos + (circles[2].pos - circles[0].pos)*(0.5+0.5 *sin(ofGetElapsedTimef()));
    ofDrawCircle(linePt,  3);
    
    
    ofPoint midPt = (circles[1].pos + circles[0].pos) / 2.0;
    ofDrawCircle(midPt,  3);
    

    smoothed = 0.7f * smoothed + 0.3 * linePt + (linePt-midPt)*1;
    ofDrawLine(linePt, smoothed);

    glDisable(GL_LINE_STIPPLE);

    
//
//
    
    int indx = tempLine.getVertices().size()-150;
    if( indx < 0)
        indx = tempLine.getVertices().size()-1;
    ofPoint circle3End = tempLine.getVertices()[indx];

    CircleFromThreePoints CFT(circle3End, smoothed, ptBegin);
//
    ofPoint center = CFT.center();
    float radius = 1.0 / CFT.curvature();

    
    ofSetColor(255,255);
    ofDrawCircle(center, radius);
    
    
    circle c;
    c.pos = center;
    c.radius = 1.0 / CFT.curvature();
    traceAngleToAngle(c, ptEnd, ptBegin, tempLine,  false, "0");
    tempLine.setClosed(true);


    ///
    //
    //
    
    ofFill();
    ofSetColor(255, 255, 255, 120);

    tempLine.draw();


    ofBeginShape();
    for (int i = 0; i < tempLine.size(); i++){
        ofVertex(tempLine[i].x, tempLine[i].y);
    }

    ofEndShape();
    
    ofNoFill();
    
//    tempLine.getVertices().erase(tempLine.getVertices().begin() + 200, tempLine.getVertices().end());
//
//    tempLine.setClosed(true);
//    traceAngleToAngle(circles[1], circles[0].pos, circles[2].pos, tempLine, !ofGetMousePressed());
//    traceAngleToAngle(circles[2], circles[1].pos, circles[0].pos, tempLine, ofGetMousePressed());
    
//    traceAngleToAngle(circles[1], circles[0].pos, circles[2].pos, tempLine, true);
//    
//    traceAngleToAngle(circles[2], circles[1].pos, circles[0].pos, tempLine, false);
//    
    
    //newAngle = angleA - angleDiff;
    
    
//    for (int i = 0; i < 100; i++){
//        float angle = angleA + (angleDiff/100.0) * i;
//        tempLine.addVertex(   circles[1].pos + circles[1].radius * ofPoint(cos(angle), sin(angle)));
//    }
//    
//    
 
}

//--------------------------------------------------------------
void CirclesScene::keyPressed(int key){

    
    circles.clear();
    
    for (int i = 0; i < 4; i++){
        circle temp;
        temp.radius = ofRandom(30,200);
        float angle = ofMap(i,0,10, 0, TWO_PI);
        temp.pos = ofPoint(500,500) + 200 * ofPoint(cos(angle), sin(angle));
        // temp.pos = ofPoint( );
        temp.vel = ofPoint(0,0);
        circles.push_back(temp);
    }

}


