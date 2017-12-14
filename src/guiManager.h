//
//  guiManager.hpp
//  example
//
//  Created by Caroline Record on 12/13/17.
//
//

#include "ofMain.h"
#include "ofxCsv.h"
#include "ofxGui.h"


class guiManager{
    
public:
    
    
    
    
    void setup(string name);
    void draw();
    void scaleValues(bool isGettingExcited, float mappedValue);
    void scaleExciteValues(bool isScalingToExcited, float mappedVal, bool isInTrigMode);
    void initGlobalMovements(bool isExcite, bool isInTrigMode);
    void saveSettings();
    void overRide();
    
    int guiState; 
    
    // the gui controlled variables 
    
    
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
    float globalFilterThresh;
    
    float globalBtMotionBlur;
    float globalTpMotionBlur;
    float globalfinalPassBlur;
    
    
    // make them all into gui things
    ofParameter<float> percentTrans;
    ofParameter<float> amountOfNoise;
    ofParameter<float> amontOfQuiver;
    
    ofParameter<int> durationOfTrans;
    // transition fully into the object
    ofParameter<int> durOfImgTrans;
    ofParameter<ofColor> color;
    ofParameter<ofColor> color2;
    ofParameter<ofColor> color3;
    
    ofParameter<ofColor> textColor;
    
    ofParameter<int> underImgMargin;
    ofParameter<int> underTitle;
    ofParameter<int> lineSpaceing;
    ofParameter<int> slurpAlpha;
    ofParameter<float> slurpNoise;
    ofParameter<float> slurpQuiver;
    ofParameter<float> amontOfGaus;
    ofParameter<float> filterThresh;
    
    ofParameter<float> BtMotionBlur;
    ofParameter<float> TpMotionBlur;
    ofParameter<float> finalPassBlur;
    
    
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
    ofParameter<float> filterThreshExcit;
    
    ofParameter<float> BtMotionBlurExcit;
    ofParameter<float> TpMotionBlurExcit;
    ofParameter<float> finalPassBlurExcit;
    
private:
    
    ofxPanel guiUniversal;
    ofxPanel guiUniversalExcited;
    
    // inside specific
    ofxPanel guiSensorThresholding;
    // dependent on vertical and orientation
    ofxPanel guiOrientation;
    
    
    bool isOverallOverride;
    bool isExcitedOverride;
    bool isThresholdOverride;
    bool isOrientOverride;
    
    
    string curatorName;
    
    bool isInside;
    bool isVertical;
    bool isLeftScreen;
   
    
    vector<string> allNames = { "Benz","Bridgett", "Fraley","Legacy","Michals","Needles","Samuels","Sebak", "Smith", "Rogers", "Gurman", "Pell", "German" };
    vector<string> inside = { "Fraley", "Rogers", "Gurman", "Pell", "German" };
    vector<string> vertical = { "Michals", "Smith", "Gurman" };
    vector<string> left = { "Sebak", "Samuels", "Pell" };

   
    
    
};