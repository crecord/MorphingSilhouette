#pragma once

#include "ofMain.h"
#include "ofxPolylineMerger.h"
#include "ofxOpenCv.h"
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

    
    //to trigger the image to appear
    
    
    int alpha;
    
    // make them all into gui things
    ofParameter<float> percentTrans;
    ofParameter<float> amountOfNoise;
    ofParameter<float> amontOfQuiver;
    ofParameter<float> amontOfGaus;
    ofParameter<int> durationOfTrans;
    // transition fully into the object
    ofParameter<int> durOfImgTrans;
    ofParameter<ofColor> color;
    ofParameter<ofColor> color2;
    ofxPanel gui;
    bool bHide;
    
    //ofxEasingSine easing;
    float clamp;
    ofxTween::ofxEasingType easingType;
    
    // the state system
    int state;
    int startTimeOfState;
    bool isSetupState; 
    
    // timekeeping for constant transformations
    int startMark;
    
    
    ofColor colOfForm;
    int quantityOfNoise;
    
    
    // Do the Gaussian
    ofShader shaderBlurX;
    ofShader shaderBlurY;
    ofFbo fboBlurOnePass;
    ofFbo fboBlurTwoPass;
    float quiv;
    
    int countOfTransforms; 
    
};
