//
//  ofxPolylineMerger.h
//  ofxPolylineMerger
//
//  Created by pach on 19/01/15.
//  Copyright (c) 2015 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxEasing.h"

class ofxPolylineMerger{
    
public:
    ofxPolylineMerger();
    ~ofxPolylineMerger();
    
    void setup();
    void update();
    void draw();
    
    void setNbPoints(int nbP);
    inline int getNbPoints() { return nbPoints;};
    
    inline ofPolyline getPolyline() {return polyOut;};
    
    void setPoly1(ofPolyline & p1);
    void setPoly2(ofPolyline & p1);
    
    void mergePolyline(float interpolationCoeff, float amntOfNoise, float speed);
    void mergePolyline(ofPolyline &p1, ofPolyline &p2, float interpolationCoeff, float amntOfNoise, float speed);
    
private:
    ofPolyline poly1;
    ofPolyline poly2;
    ofPolyline polyOut;
    
    int nbPoints;
    
    
    int easeState;
    

    
    float clamp;
    float lastInterpCoeff;
};