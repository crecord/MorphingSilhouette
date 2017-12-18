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
    
    void createSnapShot();  
    void scaleIntoImageValue(float mappedVal);
    void scaleIntoExcited(float mappedVal);
    void scaleIntoNormal(float mappedVal);
    
    void scaleExciteValues(bool isScalingToExcited, float mappedVal, bool isInTrigMode);
    void initGlobalMovements(int state);
    void saveSettings();
    void overRide();
    
    int guiState; 
    
    // the gui controlled variables 
    
    
    float globalPercentTrans;
    float globalAmountOfNoise;
    float globalAmontOfQuiver;
    float globalDensity;
    
    int globalDurationOfTrans;
    ofColor colorOfBlob;
    ofColor colorOfBackground;
    ofColor colorOfHighlight;
    
    int globalSlurpAlpha;
    float globalSlurpNoise;
    float globalSlurpQuiver;
    
    float globalBlurOneOffset;
    int globalBlurOnePasses;
    
    float globalFilterThresh;
    
    float globalBtMotionBlur;
    float globalTpMotionBlur;
    
    float globalBlurTwoOffset;
    int globalBlurTwoPasses;
    
    float globalfinalPassBlur;
    
    
    // the variables needed to take a snapshot
    float snapshotGlobalPercentTrans;
    float snapshotGlobalAmountOfNoise;
    float snapshotGlobalAmontOfQuiver;
    float snapshotGlobalDensity;
    
    int snapshotGlobalDurationOfTrans;
    ofColor snapshotColorOfBlob;
    ofColor snapshotColorOfBackground;
    ofColor snapshotColorOfHighlight;
    
    int snapshotGlobalSlurpAlpha;
    float snapshotGlobalSlurpNoise;
    float snapshotGlobalSlurpQuiver;
    
    float snapshotGlobalBlurOneOffset;
    int snapshotGlobalBlurOnePasses;
    
    float snapshotGlobalFilterThresh;
    
    float snapshotGlobalBtMotionBlur;
    float snapshotGlobalTpMotionBlur;
    
    float snapshotGlobalBlurTwoOffset;
    int snapshotGlobalBlurTwoPasses;
    
    float snapshotGlobalfinalPassBlur;
    
    
    // make them all into gui things
    ofParameter<float> percentTrans;
    ofParameter<float> amountOfNoise;
    ofParameter<float> amontOfQuiver;
    ofParameter<float> density;
    
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
    
    ofParameter<int> textScale;
    ofParameter<bool> textFlipHor;
    ofParameter<bool> textFlipVert;
    ofParameter<bool> isImageAnchor;
    ofParameter<ofVec2f> textPos;
    
    
    
    ofParameter<int> slurpAlpha;
    ofParameter<float> slurpNoise;
    ofParameter<float> slurpQuiver;
    
    ofParameter<float> blurOneOffset;
    ofParameter<int> blurOnePasses;
    
    ofParameter<float> filterThresh;
    
    ofParameter<float> BtMotionBlur;
    ofParameter<float> TpMotionBlur;
    
    ofParameter<float> blurTwoOffset;
    ofParameter<int> blurTwoPasses;
    
    ofParameter<int> upperMask;
    ofParameter<int> lowerMask;
    ofParameter<int> leftMask;
    ofParameter<int> rightMask;
    
    ofParameter<float> renderScale;
    ofParameter<int> rotation;
    ofParameter<int> rotationText;
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
    ofParameter<float> densityExcit;
    
    ofParameter<int> durationOfTransExcit;
    // transition fully into the object
    
    ofParameter<ofColor> color3Excit;
    ofParameter<ofColor> color2Excit;
    ofParameter<ofColor> colorExcit;
    
    ofParameter<int> slurpAlphaExcit;
    ofParameter<float> slurpNoiseExcit;
    ofParameter<float> slurpQuiverExcit;
    ofParameter<float> blurOneOffsetExcit;
    ofParameter<int> blurOnePassesExcit;
    ofParameter<float> filterThreshExcit;
    
    ofParameter<float> BtMotionBlurExcit;
    ofParameter<float> TpMotionBlurExcit;
   
    ofParameter<float> blurTwoOffsetExcit;
    ofParameter<int> blurTwoPassesExcit;
    
    

    ofParameter<ofColor> colorImage;
    ofParameter<float> amountOfNoiseImage;
    ofParameter<float> amountOfQuiverImage;
    ofParameter<float> densityImage;
    ofParameter<float> blurOneOffsetImage;
    ofParameter<int> blurOnePassesImage;
    ofParameter<float> filterThreshImage;
    
    ofParameter<float> BtMotionBlurImage;
    ofParameter<float> TpMotionBlurImage;
    
    ofParameter<float> blurTwoOffsetImage;
    ofParameter<int> blurTwoPassesImage;
    
    ofTrueTypeFont body;
    
private:
    
    ofxPanel guiUniversal;
    ofxPanel guiUniversalExcited;
    ofxPanel guiUniversalImage;
    
    // inside specific
    ofxPanel guiSensorThresholding;
    // dependent on vertical and orientation
    ofxPanel guiOrientation;
    
    
    bool isOverallOverride;
    bool isExcitedOverride;
    bool isThresholdOverride;
    bool isOrientOverride;
    bool isImageOverride;
    
    string curatorName;
    bool isInside;
    bool isVertical;
    bool isLeftScreen;
   
    vector<string> allNames = { "Benz","Bridgett", "Fraley","Legacy","Michals","Needles","Samuels","Sebak", "Smith", "Rogers", "Gurman", "Pell", "German" };
    vector<string> inside = { "Fraley", "Rogers", "Gurman", "Pell", "German" };
    vector<string> vertical = { "Michals", "Smith", "Gurman" };
    vector<string> left = { "Sebak", "Samuels", "Pell" };
    
    
    
    void textScaleChanged(int & txtScl);
    
};