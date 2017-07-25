#pragma once

#include "ofMain.h"
#include "ofxPolylineMerger.h"
#include "ofxOpenCv.h"
#include "ofxLeapMotion2.h"
#include "ofxTween.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        ofPolyline pngToPolyline(ofImage img);
        int nextSill(int num);
        ofVec2f getSz(ofImage img);
        void exit(); 
    
    void generatePolyline();
    
    ofxPolylineMerger pMerge;
    
    ofPolyline p1;
    ofPolyline p2;
    float interpolateCoeff;
    
    float lastTime;
    float interpolateTime;
    float time;
    
    ofPath filled;
    
    // find the contours of a png that is loaded in.
    ofxCvColorImage	colorImg;
    ofxCvGrayscaleImage grayImg;
    ofxCvContourFinder contourFinder;
    
    ofImage bird;
    ofImage rock;
    
    ofShader shade;
    ofFbo drawNoColor;
    
    ofDirectory dir;
    vector<ofImage> images;
    vector<ofPolyline> polys;
    ofPolyline midTrans; 
    
    int transformFrom;
    int transformToo;
    
    
    // to help trigger the image change at the peek and valley
    bool isExpand;
    bool isContract;
    
    //to trigger the image to appear
    
    bool isImgWaitTime;
    bool isImgTime;
    float startTimeFrozen;
    float timeFrozen;
    
    int alpha;
    
    bool isImageFade;
    float startTimeImageFade;
    
    ofxLeapMotion leap;
    vector <ofxLeapMotionSimpleHand> simpleHands;
    
    vector <int> fingersFound;
    
    ofVideoGrabber vidGrabber;
    
    
    // new system of keeping time!
    int startMark;
    //int durationOfTrans;
    //float percentTrans;
    //float amountOfNoise;
    
    // make them all into gui things
    ofParameter<float> percentTrans;
    ofParameter<float> amountOfNoise;
    ofParameter<int> durationOfTrans;
    // transition fully into the object
    ofParameter<int> durOfImgTrans;
    ofParameter<ofColor> color;
    
    ofxPanel gui;
    bool bHide;
    
    
    //ofxEasingSine easing;
    float clamp;
    ofxTween::ofxEasingType easingType;
    
    // transition fully into the object
    
    
};
