//
//  morph.h
//  example
//
//  Created by Caroline Record on 8/1/17.
//
//
#include "ofMain.h"
#include "polylineMerger.h"
#include "ofxOpenCv.h"
#include "ofxEasing.h"
#include "ofxCsv.h"
#include "item.h"
#include "handleSerial.h"
#include "ofxFboBlur.h"
#include "guiManager.h"


class morph{
    
public:
    
    void setup(int x, int y);
    void update();
    void drawGui(int x,int y);
    void drawMorph(int x,int y);
    void triggerNext();
    void saveGuiSettings();
    bool bHide;
    void resetValues();
    guiManager gManager;
    handleSerial ardTalk;
    int falseImgPos;
    int nextSill(int num);
    
private:

    
    
    ofPolyline pngToPolyline(ofImage img);
    ofVec2f getSz(ofImage img);
    
    
    
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

    
    ofFbo motionBlur;
    
    ofDirectory dir;
    ofDirectory csvDir;

    
    vector<item> items;
    
    ofPolyline midTrans;
    
    int transformFrom;
    int transformToo;
    
    
    //to trigger the image to appear
    
    
    int alpha;
    int alphaPainting;
    
    
    bool isExcite;
    bool isTransIntoExcite;
    bool isTransOutOfExcite;
    
    int startTimeOfExciteFade;
    
  
    
    // so that if we need to fade out suddenly in the middle of a fade in it will be continuous
    int leftOverFadeTime;
      
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

    
    int alphaText;
    vector<int> randomIndices;
    
    int startTimeUnused;
    
    ofShader shade;
    ofShader threshShade;
    
    
    void pathToPath();
    void drawWithGL (ofPolyline pols, int res);
    
    
    
    bool isTriggered;
    
     ofPolyline cur;
    
    bool isSensor;
    
    ofxFboBlur gpuBlur1;
    ofxFboBlur gpuBlur2;
    
    int fadeSlurpToo;
    
    void applyScale(int wid, int height, int scale, bool isNoOffset=false);
    
    
    int rightOffset;
    int leftOffset;
    int upperOffset;
    int lowerOffset;
    
    ofVec2f largestBlobDimensions;
    
    int fboWidth;
    int fboHeight;
    int fboXpos;
    int fboYPos;
    
    
    bool isPaintingSquiggleTime;
    
};