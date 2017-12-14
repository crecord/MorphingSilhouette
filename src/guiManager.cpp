//
//  guiManager.cpp
//  example
//
//  Created by Caroline Record on 12/13/17.
//
//

#include "guiManager.h"


void guiManager::setup(string name){
    
    curatorName = name; 

    // use this info to determine which file you load in and which you save too
    bool isThere = ofContains(allNames,curatorName);
    if(!isThere){
        ofLog()<< "YO! the curator's name is spelled wrong";
    }
    
    
    
    
    
    // ofxPanel guiUniversal;!!!!!!!!!!!!!!!!!!
    guiUniversal.setup("Universal Bolb");
    
    guiUniversal.add(percentTrans.set("amount trans",.6,0,1));
    guiUniversal.add(amountOfNoise.set("noise",10,0,30));
    guiUniversal.add(amontOfQuiver.set("quiver",10,1,50));
    
    guiUniversal.add(durationOfTrans.set("speed",300,100,10000));
    guiUniversal.add(slurpAlpha.set("slurp alpha", 5, 0,255));
    
    guiUniversal.add(slurpNoise.set("slurp noise",10,0,50));
    guiUniversal.add(slurpQuiver.set("slurp quiver",.5,0,3));
    
    guiUniversal.add(amontOfGaus.set("gaus",.5,0,1));
    guiUniversal.add(filterThresh.set("filter thresh",.2,0,1));
    
    
    guiUniversal.add(BtMotionBlur.set("BtMotionBlur",1,-5,5));
    guiUniversal.add(TpMotionBlur.set("TpMotionBlur",0,-5,5));
    guiUniversal.add(finalPassBlur.set("finalPassBlur",0.3,0,1));
    
    guiUniversal.add(durOfImgTrans.set("speed ofImg",200,100,10000));
    guiUniversal.add(color.set("color",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    guiUniversal.add(color2.set("color 2",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    guiUniversal.add(color3.set("color 3",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    guiUniversal.add(textColor.set("text color",ofColor(0),ofColor(0,0),ofColor(255,255)));
    
    
    ///ofxPanel guiUniversalExcited;!!!!!!!!!!!!
    
    guiUniversalExcited.setup("excited Blob");
    guiUniversalExcited.add(durOfTransIntoExcite.set("dur trans to excite",200,100,10000));
    guiUniversalExcited.add(durOfTransOutExcite.set("dur trans out of excite",200,100,10000));
    
    guiUniversalExcited.add(percentTransExcit.set("amount trans",.6,0,1));
    guiUniversalExcited.add(amountOfNoiseExcit.set("noise",10,0,30));
    guiUniversalExcited.add(amontOfQuiverExcit.set("quiver",10,1,50));
    
    guiUniversalExcited.add(durationOfTransExcit.set("speed in",300,100,10000));

    guiUniversalExcited.add(slurpAlphaExcit.set("slurp alpha", 5, 0,255));
    guiUniversalExcited.add(slurpNoiseExcit.set("slurp noise",10,0,50));
    guiUniversalExcited.add(slurpQuiverExcit.set("slurp quiver",.5,0,3));
    
    guiUniversalExcited.add(amontOfGausExcit.set("gaus",.5,0,1));
    guiUniversalExcited.add(filterThreshExcit.set("filter thresh",0.2,0,1));
    
    
    guiUniversalExcited.add(BtMotionBlurExcit.set("BtMotionBlur",1,-5,5));
    guiUniversalExcited.add(TpMotionBlurExcit.set("TpMotionBlur",0,-5,5));
    guiUniversalExcited.add(finalPassBlurExcit.set("finalPassBlur",0.3,0,1));
    
    guiUniversalExcited.add(colorExcit.set("color",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    
    guiUniversalExcited.add(color2Excit.set("back col",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    
    
    // inside specific
    //ofxPanel guiSensorThresholding;!!!!!!!!!!!!!!!!
    guiSensorThresholding.setup("theshold sensors");
    guiSensorThresholding.add(excitedThresh.set("Excite Threshold", 30, 10, 500));
    guiSensorThresholding.add(sensorThresh.set("sensor threshold", 100, 10, 500));
    guiSensorThresholding.add(motionDifference.set("motion difference threshold", 10, 1, 500));
    
    
    
    // dependent on vertical and orientation
    //ofxPanel guiOrientation;!!!!!!!!!!!!!
    guiOrientation.setup("Orientation");
    guiOrientation.add(upperMask.set("Upper Mask", 0, 0, 100));
    guiOrientation.add(lowerMask.set("Lower Mask", 0, 0, 100));
    guiOrientation.add(leftMask.set("Left Mask", 0, 0, 100));
    guiOrientation.add(rightMask.set("Right Mask", 0, 0, 100));
    
    guiOrientation.add(renderScale.set("scale",1,0,3));
    guiOrientation.add(rotation.set("rotate", 0, 0, 3));
    guiOrientation.add(blobOffset.set("offset", ofVec2f(0,0), ofVec2f(-ofGetWidth()/2,-ofGetHeight()/2), ofVec2f(ofGetWidth()/2,ofGetHeight()/2)));
    guiOrientation.add(flipVert.set("flip vertical", false));
    guiOrientation.add(flipHor.set("flip horizontal", false));
    
    guiOrientation.add(underImgMargin.set("under img Margin",0,100,10));
    guiOrientation.add(underTitle.set("under title", 0,100,5));
    
    
    // make it so they won't draw on top of each other
    guiUniversal.setPosition(10,10);
    guiUniversalExcited.setPosition(10,10);
    guiSensorThresholding.setPosition(10,10);
    guiOrientation.setPosition(10,10);
    
    
    
    isOverallOverride = false;
    isExcitedOverride = false;
    isThresholdOverride = false;
    isOrientOverride  = false;
    
    // check if there are any overrides!
    // list all the .xml files
    ofDirectory direct;
    direct.allowExt(".xml");
    direct.listDir("settings/");
    direct.sort();
    
    // loop through them and see if any start with the curators name
    for(int i =0; i < direct.size(); i++){
        string fPath = direct.getPath(i);
        
        vector<string> slices = ofSplitString(fPath, "/");
        vector<string> relvantSlice = ofSplitString(slices.at(slices.size()-1), "_");
        if(relvantSlice.size() > 0){
            if(relvantSlice.at(0) == curatorName){
                string guiName = relvantSlice.at(1);
                if(guiName == "overall"){
                    isOverallOverride = true;
                }
                else if (guiName == "excited"){
                    isExcitedOverride = true;
                }
                else if (guiName == "thresholds"){
                    isThresholdOverride = true;
                }
                else if (guiName == "orientation"){
                    isOrientOverride = true;
                }
            }
        }
    }
    
    
    // if their are flag it 
    
    
    
    // load the correct settings into each gui
    
    if(isOverallOverride){
        guiUniversal.loadFromFile("settings/" + curatorName + "_overall.xml");
    }
    else{
        guiUniversal.loadFromFile("settings/overall.xml");
    }
    
    if (isExcitedOverride){
        guiUniversalExcited.loadFromFile("settings/" + curatorName + "_excited.xml");
    }
    else {
        guiUniversalExcited.loadFromFile("settings/excited.xml");
    }
    
    isInside = ofContains(inside, curatorName);
    if (isThresholdOverride){
        guiSensorThresholding.loadFromFile("settings/" + curatorName + "_thesholds.xml");
    }
    else {
       
        if(isInside){
            ofLog()<< "Inside the Cabinet";
            guiSensorThresholding.loadFromFile("settings/thresholdsInside.xml");
        }
        else{
            guiSensorThresholding.loadFromFile("settings/thresholdsOutside.xml");
        }
    }
    
    
    isVertical  = ofContains(vertical, curatorName);
    isLeftScreen  = ofContains(left, curatorName);
    
    if(isOrientOverride){
        guiOrientation.loadFromFile("settings/" + curatorName +"_orientation.xml");
    }
    else{
        if(isVertical){
            ofLog()<< "Is Vertical";
            guiOrientation.loadFromFile("settings/orientationVertical.xml");
        }
        else if (isLeftScreen){
            guiOrientation.loadFromFile("settings/orientationHorizontalLeft.xml");
            ofLog()<< "Is Left";
        }else{
            guiOrientation.loadFromFile("settings/orientationHorizontalRight.xml");
            ofLog()<< "Is Right";
        }
    }
    
    

    
    globalPercentTrans = percentTrans;
    globalAmountOfNoise = amountOfNoise;
    globalAmontOfQuiver = amontOfQuiver;
    
    globalDurationOfTrans = durationOfTrans;
    colorOfBlob = color;
    colorOfBackground = color2;
    
    globalSlurpAlpha = slurpAlpha;
    globalSlurpNoise = slurpNoise;
    globalSlurpQuiver = slurpQuiver;
    globalAmontOfGaus = amontOfGaus;
    globalFilterThresh = filterThresh;
    
    globalBtMotionBlur = BtMotionBlur;
    globalTpMotionBlur = TpMotionBlur;
    globalfinalPassBlur = finalPassBlur;
    
    
    
    
}

void guiManager::draw(){
    
    //guiState is initialized in ofApp
    
    if(1 == guiState){
        guiUniversal.draw();
    }
    else if (2 == guiState){
        guiUniversalExcited.draw();
    }
    else if (3 == guiState){
        guiSensorThresholding.draw();
    }
    else if (4 == guiState){
        guiOrientation.draw();
    }
    
}

void guiManager::scaleExciteValues(bool isScalingToExcited, float mappedVal, bool isInTrigMode){
    
    int down = 0;
    int up =1;
    
    if(!isScalingToExcited){
        down = 1;
        up = 0;
    }
    
    globalPercentTrans = ofMap(mappedVal,down,up,percentTrans,percentTransExcit);
    globalAmountOfNoise = ofMap(mappedVal,down ,up ,amountOfNoise,amountOfNoiseExcit);
    globalAmontOfQuiver = ofMap(mappedVal,down,up,amontOfQuiver,amontOfQuiverExcit);;
    
    globalDurationOfTrans = ofMap(mappedVal,down,up,durationOfTrans,durationOfTransExcit);
   
    // do something here
    colorOfBlob = color.get().getLerped(colorExcit.get(), abs(down - mappedVal));
    colorOfBackground = color2.get().getLerped(color2Excit.get(), abs(down - mappedVal));
    
    if(isInTrigMode){
        globalSlurpAlpha = ofMap(mappedVal,down,up,slurpAlpha,slurpAlphaExcit);
    }
    
    globalSlurpNoise = ofMap(mappedVal,down,up,slurpNoise,slurpNoiseExcit);
    globalSlurpQuiver = ofMap(mappedVal,down,up,slurpQuiver,slurpQuiverExcit);
    globalAmontOfGaus = ofMap(mappedVal,down,up,amontOfGaus,amontOfGausExcit);
    globalFilterThresh = ofMap(mappedVal,down,up,filterThresh,filterThreshExcit);
    
    globalBtMotionBlur = ofMap(mappedVal,down,up,BtMotionBlur,BtMotionBlurExcit);
    globalTpMotionBlur = ofMap(mappedVal,down,up,TpMotionBlur,TpMotionBlurExcit);
    globalfinalPassBlur = ofMap(mappedVal,down,up,finalPassBlur,finalPassBlurExcit);
    
}


void guiManager::initGlobalMovements(bool isExcite, bool isInTrigMode){
    
    if(isExcite){
        globalPercentTrans = percentTransExcit;
        globalAmountOfNoise = amountOfNoiseExcit;
        globalAmontOfQuiver = amontOfQuiverExcit;
    
        globalDurationOfTrans = durationOfTransExcit;
        colorOfBlob = colorExcit;
        colorOfBackground = color2Excit;
    
        if(isInTrigMode){
            globalSlurpAlpha = slurpAlphaExcit;
        }
    
        globalSlurpNoise = slurpNoiseExcit;
        globalSlurpQuiver = slurpQuiverExcit;
        globalAmontOfGaus = amontOfGausExcit;
        globalFilterThresh = filterThreshExcit;
        
        globalBtMotionBlur = BtMotionBlurExcit;
        globalTpMotionBlur = TpMotionBlurExcit;
        globalfinalPassBlur = finalPassBlurExcit;
    }
    else {
        globalPercentTrans = percentTrans;
        globalAmountOfNoise = amountOfNoise;
        globalAmontOfQuiver = amontOfQuiver;
        
        globalDurationOfTrans = durationOfTrans;
        colorOfBlob = color;
        colorOfBackground = color2;
        if(isInTrigMode){
            globalSlurpAlpha = slurpAlpha;
        }
        globalSlurpNoise = slurpNoise;
        globalSlurpQuiver = slurpQuiver;
        globalAmontOfGaus = amontOfGaus;
        globalFilterThresh = filterThresh;
        
        globalBtMotionBlur = BtMotionBlur;
        globalTpMotionBlur = TpMotionBlur;
        globalfinalPassBlur = finalPassBlur;
    }
}



void guiManager::saveSettings(){
    
    ofLog()<< "Saving settings!";
    
    if(isOverallOverride){
        guiUniversal.saveToFile("settings/" + curatorName + "_overall.xml");
    }
    else{
        guiUniversal.saveToFile("settings/overall.xml");
    }
    
    if (isExcitedOverride){
        guiUniversalExcited.saveToFile("settings/" + curatorName + "_excited.xml");
    }
    else {
        guiUniversalExcited.saveToFile("settings/excited.xml");
    }
    
    if (isThresholdOverride){
        guiSensorThresholding.saveToFile("settings/" + curatorName + "_thesholds.xml");
    }
    else {
        
        if(isInside){
            guiSensorThresholding.saveToFile("settings/thresholdsInside.xml");
        }
        else{
            guiSensorThresholding.saveToFile("settings/thresholdsOutside.xml");
        }
    }

    
    if(isOrientOverride){
        guiOrientation.saveToFile("settings/" + curatorName +"_orientation.xml");
    }
    else{
        if(isVertical){
            ofLog()<< "Is Vertical";
            guiOrientation.saveToFile("settings/orientationVertical.xml");
        }
        else if (isLeftScreen){
            guiOrientation.saveToFile("settings/orientationHorizontalLeft.xml");
            ofLog()<< "Is Left";
        }else{
            guiOrientation.saveToFile("settings/orientationHorizontalRight.xml");
            ofLog()<< "Is Right";
        }
    }

    
    
}

void guiManager::overRide(){
    
    if(1 == guiState){
        guiUniversal.saveToFile("settings/" + curatorName + "_overall.xml");
    }
    else if (2 == guiState){
        guiUniversalExcited.saveToFile("settings/" + curatorName + "_excited.xml");
    }
    else if (3 == guiState){
        guiSensorThresholding.saveToFile("settings/" + curatorName + "_thresholds.xml");
    }
    else if (4 == guiState){
        guiOrientation.saveToFile("settings/" + curatorName + "_orientation.xml");
    }
}

