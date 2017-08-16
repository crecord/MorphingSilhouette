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
#include "ofxTween.h"
#include "ofxGui.h"
#include "ofxCsv.h"

class morph{
    
public:
    
    void setup(string pathToImages, int x, int y);
    void update();
    void drawGui(int x,int y);
    void drawMorph(int x,int y);
    void triggerNext();
    void saveGuiSettings();
    bool bHide;

private:
    
    ofPolyline pngToPolyline(ofImage img);
    int nextSill(int num);
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
    
    ofShader shade;
    ofFbo drawNoColor;
    
    ofDirectory dir;
    ofDirectory csvDir;
    vector<ofImage> images;
    vector<ofPolyline> polys;
    vector<ofxCsv> blurbs;
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
    ofParameter<ofColor> textColor;
    ofParameter<int> underImgMargin;
    ofParameter<int> underTitle;
    ofParameter<int> lineSpaceing;

    ofxPanel gui;

    
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
    float quiv;
    int countOfTransforms;
    
    bool doText; 
    
    
    ofTrueTypeFont title;
    ofTrueTypeFont body;
    
    int alphaText;
    vector<int> randomIndices;
    
    int startTimeUnused;

};