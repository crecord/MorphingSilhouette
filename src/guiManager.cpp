//
//  guiManager.cpp
//  example
//
//  Created by Caroline Record on 12/13/17.
//
//

#include "guiManager.h"


void guiManager::setup(){
    
    ofxCsv blurb;
    
    guiUniversal.setup("panel");
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
    
    
    //durOfImgTrans
    guiUniversal.add(durOfImgTrans.set("speed ofImg",200,100,10000));
    guiUniversal.add(color.set("color",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    guiUniversal.add(color2.set("color 2",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    guiUniversal.add(color3.set("color 3",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    guiUniversal.add(textColor.set("text color",ofColor(0),ofColor(0,0),ofColor(255,255)));
    guiUniversal.add(underImgMargin.set("under img Margin",0,100,10));
    guiUniversal.add(underTitle.set("under title", 0,100,5));
    
    
    guiUniversal.add(upperMask.set("Upper Mask", 0, 0, 100));
    guiUniversal.add(lowerMask.set("Lower Mask", 0, 0, 100));
    guiUniversal.add(leftMask.set("Left Mask", 0, 0, 100));
    guiUniversal.add(rightMask.set("Right Mask", 0, 0, 100));
    
    
    guiUniversal.add(renderScale.set("scale",1,0,3));
    guiUniversal.add(rotation.set("rotate", 0, 0, 3));
    guiUniversal.add(blobOffset.set("offset", ofVec2f(0,0), ofVec2f(-ofGetWidth()/2,-ofGetHeight()/2), ofVec2f(ofGetWidth()/2,ofGetHeight()/2)));
    guiUniversal.add(flipVert.set("flip vertical", false));
    guiUniversal.add(flipHor.set("flip horizontal", false));
    
    guiUniversal.add(sensorThresh.set("sensor threshold", 100, 10, 500));
    guiUniversal.add(motionDifference.set("motion difference threshold", 10, 1, 500));
    
    
    guiUniversal.loadFromFile("settings/settings.xml");
    
    guiUniversalExcited.setup("excited Blob");
    guiUniversalExcited.add(excitedThresh.set("Excite Threshold", 30, 10, 500));
    
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
    
    //durOfImgTrans
    
    guiUniversalExcited.add(colorExcit.set("color",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    
    guiUniversalExcited.add(color2Excit.set("back col",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    guiUniversalExcited.loadFromFile("settings/settings2.xml");
    
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

void guiManager::draw(char identitifier){
    
    guiUniversalExcited.draw();
    guiUniversal.draw();
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
    guiUniversal.saveToFile("settings/settings.xml");
    guiUniversalExcited.saveToFile("settings/settings2.xml");
}

