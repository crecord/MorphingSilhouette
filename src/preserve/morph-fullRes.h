//
//  morph.h
//  example
//
//  Created by Caroline Record on 8/1/17.
//
//
#include "ofMain.h"
#include "ofxPolylineMerger.h"
#include "ofxOpenCv.h"
#include "ofxEasing.h"
#include "ofxGui.h"
#include "ofxCsv.h"
#include "item.h"
#include "handleSerial.h"

class morph{
    
public:
    
    void setup(string pathToImages, int x, int y);
    void update();
    void drawGui(int x,int y);
    void drawMorph(int x,int y);
    void triggerNext();
    void saveGuiSettings();
    bool bHide;
    void checkIntersection(ofPolyline ply);

private:

    
    ofPolyline pngToPolyline(ofImage img);
    ofVec2f getSz(ofImage img);
    
    int nextSill(int num);
    
    void generatePolyline();
    void populateVector();
    
    ofxPolylineMerger pMerge;
    ofPolyline p1;
    ofPolyline p2;
    float interpolateCoeff;
    ofPath filled;
    
    // find the contours of a png that is loaded in.
    ofxCvColorImage	colorImg;
    ofxCvGrayscaleImage grayImg;
    ofxCvContourFinder contourFinder;
    ofFbo drawNoColor;
    ofFbo drawTrailing;
    ofFbo drawTrailingBlurX;
    ofFbo drawTrailingBlurY;
    
    ofFbo motionBlur;
    
    ofDirectory dir;
    ofDirectory csvDir;
   // vector<ofImage> images;
    //vector<ofPolyline> polys;
    //vector<ofxCsv> blurbs;
    
    vector<item> items;
    
    ofPolyline midTrans;
    
    int transformFrom;
    int transformToo;
    
    
    //to trigger the image to appear
    
    
    int alpha;
    int alphaPainting;
    
    // make them all into gui things
    ofParameter<float> percentTrans;
    ofParameter<float> amountOfNoise;
    ofParameter<float> amontOfQuiver;
    
    ofParameter<int> durationOfTrans;
    // transition fully into the object
    ofParameter<int> durOfImgTrans;
    ofParameter<ofColor> color;
    ofParameter<ofColor> color2;
    ofParameter<ofColor> textColor;
    ofParameter<int> underImgMargin;
    ofParameter<int> underTitle;
    ofParameter<int> lineSpaceing;
    ofParameter<int> slurpAlpha;
    ofParameter<float> slurpNoise;
    ofParameter<float> slurpQuiver;
    ofParameter<float> amontOfGaus;
    ofParameter<int> filterThresh;
    
    ofParameter<int> upperMask;
    ofParameter<int> lowerMask;
    ofParameter<int> leftMask;
    ofParameter<int> rightMask;
    
    ofParameter<float> renderScale;
    ofParameter<int> rotation;
    ofParameter<ofVec2f> blobOffset;
    ofParameter<bool> flipVert;
    ofParameter<bool> flipHor;
    
    ofParameter<int> sensorThresh;
    ofParameter<int> motionDifference;
    
    ofxPanel gui;
    
    ofxPanel guiExcited;
    
    ofParameter<int> excitedThresh;
    ofParameter<int> durOfTransIntoExcite;
    ofParameter<int> durOfTransOutExcite;
    
    ofParameter<float> percentTransExcit;
    ofParameter<float> amountOfNoiseExcit;
    ofParameter<float> amontOfQuiverExcit;
    
    ofParameter<int> durationOfTransExcit;
    // transition fully into the object
    
    ofParameter<ofColor> color2Excit;
    ofParameter<ofColor> colorExcit;
    
    ofParameter<int> slurpAlphaExcit;
    ofParameter<float> slurpNoiseExcit;
    ofParameter<float> slurpQuiverExcit;
    ofParameter<float> amontOfGausExcit;
    ofParameter<int> filterThreshExcit;
    
    float globalPercentTrans;
    float globalAmountOfNoise;
    float globalAmontOfQuiver;
    
    int globalDurationOfTrans;
    ofColor colorOfBlob;
    ofColor colorOfBackground;
    
    int globalSlurpAlpha;
    float globalSlurpNoise;
    float globalSlurpQuiver;
    float globalAmontOfGaus;
    int globalFilterThresh;
    
    bool isExcite;
    bool isTransIntoExcite;
    bool isTransOutOfExcite;
    
    int startTimeOfExciteFade;
    
    // so that if we need to fade out suddenly in the middle of a fade in it will be continuous
    int leftOverFadeTime;
    //ofxEasingQuart easingQuart;
    //ofxTween::ofxEasingType easingType;
    
    //ofxEasingSine easing;
    float clamp;
    
    // the state system
    int state;
    int startTimeOfState;
    bool isSetupState;
    
    // timekeeping for constant transformations
    int startMark;
    ofColor colOfForm;
    int quantityOfNoise;
    
    // Do the Gaussian
    float quiv;
    int countOfTransforms;
    
    bool doText; 
    
    
    ofTrueTypeFont title;
    ofTrueTypeFont body;
    
    int alphaText;
    vector<int> randomIndices;
    
    int startTimeUnused;
    
    ofShader shade;
    ofShader blurX;
    ofShader blurY;
    // check intersection
    int orientation(ofVec2f p, ofVec2f q, ofVec2f r);
    bool doIntersect(ofVec2f p1, ofVec2f q1, ofVec2f p2, ofVec2f q2);
    void lookForInter();
    
    bool isIntersect;
    vector<ofPoint> mergedPoints;
    
    //try out contour detecting it. 
    ofPath pathToPath(ofPath pth);
    
    
    handleSerial ardTalk;
    bool isTriggered;
    
     ofPolyline cur;
    
};