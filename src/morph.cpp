//
//  morph.cpp
//  example
//
//  Created by Caroline Record on 8/1/17.
//
//

#include "morph.h"

void morph::setup(string pathToImages, int x, int y){
    
    
    interpolateCoeff = 0;
    pMerge.setup();
    
    
    dir.allowExt("png");
    dir.listDir(pathToImages);
    dir.sort();
    
    csvDir.allowExt("csv");
    csvDir.listDir(pathToImages);
    csvDir.sort();
    
    // you can now iterate through the files and load them into the ofImage vector
    
    shade.load("shaders/silhouette/shader");
    threshShade.load("shaders/threshShade/threshShade");
    
    
    //string pathToImages, string pathToCsv, ofImage img, ofPolyline polys
    for(int i = 0; i < dir.size(); i++){
        item temp;
        ofImage imgTemp;
        imgTemp.load(dir.getPath(i));
        ofPolyline ply = pngToPolyline(imgTemp);
        temp.setup(pathToImages, dir.getPath(i), csvDir.getPath(i), imgTemp, ply);
        items.push_back(temp);
    }
    

    transformFrom = 0;
    transformToo = 1;

    midTrans = items.at(transformFrom).poly;
    
  
    gui.setup("panel");
    gui.add(percentTrans.set("amount trans",.6,0,1));
    gui.add(amountOfNoise.set("noise",10,0,30));
    gui.add(amontOfQuiver.set("quiver",10,1,50));
    
    gui.add(durationOfTrans.set("speed",300,100,10000));
    gui.add(slurpAlpha.set("slurp alpha", 5, 0,255));
    
    gui.add(slurpNoise.set("slurp noise",10,0,50));
    gui.add(slurpQuiver.set("slurp quiver",.5,0,3));
    
    gui.add(amontOfGaus.set("gaus",.5,0,1));
    gui.add(filterThresh.set("filter thresh",.2,0,1));
    
    
    gui.add(BtMotionBlur.set("BtMotionBlur",1,-5,5));
    gui.add(TpMotionBlur.set("TpMotionBlur",0,-5,5));
    gui.add(finalPassBlur.set("finalPassBlur",0.3,0,1));

    
    //durOfImgTrans
    gui.add(durOfImgTrans.set("speed ofImg",200,100,10000));
    gui.add(color.set("color",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    gui.add(color2.set("color 2",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    gui.add(color3.set("color 3",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    gui.add(textColor.set("text color",ofColor(0),ofColor(0,0),ofColor(255,255)));
    gui.add(underImgMargin.set("under img Margin",0,100,10));
    gui.add(underTitle.set("under title", 0,100,5));
    
    
    gui.add(upperMask.set("Upper Mask", 0, 0, 100));
    gui.add(lowerMask.set("Lower Mask", 0, 0, 100));
    gui.add(leftMask.set("Left Mask", 0, 0, 100));
    gui.add(rightMask.set("Right Mask", 0, 0, 100));
    
    
    gui.add(renderScale.set("scale",1,0,3));
    gui.add(rotation.set("rotate", 0, 0, 3));
    gui.add(blobOffset.set("offset", ofVec2f(0,0), ofVec2f(-ofGetWidth()/2,-ofGetHeight()/2), ofVec2f(ofGetWidth()/2,ofGetHeight()/2)));
    gui.add(flipVert.set("flip vertical", false));
    gui.add(flipHor.set("flip horizontal", false));
    
    gui.add(sensorThresh.set("sensor threshold", 100, 10, 500));
    gui.add(motionDifference.set("motion difference threshold", 10, 1, 500));
    
    
    gui.loadFromFile("settings/settings.xml");
    
    guiExcited.setup("excited Blob");
    guiExcited.add(excitedThresh.set("Excite Threshold", 30, 10, 500));
    
    guiExcited.add(durOfTransIntoExcite.set("dur trans to excite",200,100,10000));
    guiExcited.add(durOfTransOutExcite.set("dur trans out of excite",200,100,10000));
    
    guiExcited.add(percentTransExcit.set("amount trans",.6,0,1));
    guiExcited.add(amountOfNoiseExcit.set("noise",10,0,30));
    guiExcited.add(amontOfQuiverExcit.set("quiver",10,1,50));
    
    guiExcited.add(durationOfTransExcit.set("speed in",300,100,10000));
   
    
    guiExcited.add(slurpAlphaExcit.set("slurp alpha", 5, 0,255));
    
    guiExcited.add(slurpNoiseExcit.set("slurp noise",10,0,50));
    guiExcited.add(slurpQuiverExcit.set("slurp quiver",.5,0,3));
    
    guiExcited.add(amontOfGausExcit.set("gaus",.5,0,1));
    guiExcited.add(filterThreshExcit.set("filter thresh",0.2,0,1));
    
    
    guiExcited.add(BtMotionBlurExcit.set("BtMotionBlur",1,-5,5));
    guiExcited.add(TpMotionBlurExcit.set("TpMotionBlur",0,-5,5));
    guiExcited.add(finalPassBlurExcit.set("finalPassBlur",0.3,0,1));

    //durOfImgTrans
    
    guiExcited.add(colorExcit.set("color",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    
    guiExcited.add(color2Excit.set("back col",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    guiExcited.loadFromFile("settings/settings2.xml");
    
    isExcite = false;
    isTransIntoExcite = false;
    isTransOutOfExcite = false;
    
    
     
    bHide = false;
    state = 1;
    startTimeOfState = ofGetElapsedTimeMillis();
    startMark = ofGetElapsedTimeMillis();
    isSetupState = false;
    alpha = 255;
    countOfTransforms =0;

    doText = false;
    
    title.load("font/Klavika-Regular.otf", 20);
    body.load("font/franklinGothic.otf", 12);
    
    
    alphaPainting =0;
    
    
    motionBlur.allocate(ofGetWidth(), ofGetHeight(), GL_RGB );
    motionBlur.begin();
    ofClear(255,255,255,0);
    ofSetColor(255);
    ofDrawRectangle(0, 0, motionBlur.getWidth(), motionBlur.getHeight());
    motionBlur.end();
    

    
    drawTrailing.allocate(ofGetWidth(), ofGetHeight(), GL_RGB );
    
    drawTrailing.begin();
    ofClear(0,0,0,255);
    ofSetColor(0);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    drawTrailing.end();
    
    ardTalk.setup();
    isTriggered = false;
    
    grayImg.allocate(ofGetWidth(), ofGetHeight());
    colorImg.allocate(ofGetWidth(), ofGetHeight() );
    
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

    
    leftOverFadeTime = 0;

    
    
    isSensor = false;
    
    
    ofFbo::Settings s;
    s.width = ofGetWidth();
    s.height = ofGetHeight();
    s.internalformat = GL_RGBA;
    s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    s.maxFilter = GL_LINEAR; GL_NEAREST;
    s.numSamples = 0;
    s.numColorbuffers = 1;
    s.useDepth = false;
    s.useStencil = false;

    
    ofBackground(22);
    ofFbo::Settings t;
    t.width = ofGetWidth();
   t.height = ofGetHeight();
    t.internalformat = GL_RGBA;
    t.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    t.maxFilter = GL_LINEAR; GL_NEAREST;
    t.numSamples = 0;
    t.numColorbuffers = 1;
   t.useDepth = false;
    t.useStencil = false;
    
    gpuBlur1.setup(t, false);
    gpuBlur2.setup(s, false);
    
    
    // the target value to fade slurp out to
    fadeSlurpToo = 150;
    
}

void morph::resetValues(){
    startTimeOfState = ofGetElapsedTimeMillis();
    startMark = ofGetElapsedTimeMillis();
}


void morph::update(){
    
    gpuBlur1.blurOffset = 1.5;
    gpuBlur1.blurPasses = 1;

    //gpuBlur1.blurOffset = 130 * globalAmontOfGaus;
    //gpuBlur1.blurPasses = 10 * 1 - globalAmontOfGaus;
    gpuBlur1.numBlurOverlays = 1;
    gpuBlur1.blurOverlayGain = 255;
    gpuBlur2.blurOffset = 1.5;
    gpuBlur2.blurPasses = 1;

   // gpuBlur2.blurOffset = 130 * globalfinalPassBlur;
   // gpuBlur2.blurPasses = 10 * 1 - globalfinalPassBlur;
    gpuBlur2.numBlurOverlays = 1;
    gpuBlur2.blurOverlayGain = 255;
     
 
    ofEnableAlphaBlending();
    

    
    //ofLog()<< state;

    if(state == 1){
        //some one is here - instruct them
        if(isSetupState){
            isSetupState = false;
            startTimeUnused = ofGetElapsedTimeMillis();
        }
        
        //colOfForm = color2;
        int timePassed = ofGetElapsedTimeMillis() - startMark;
        if (timePassed > globalDurationOfTrans-1){
            startMark = ofGetElapsedTimeMillis();
            transformFrom = nextSill(transformFrom);
            transformToo = nextSill(transformToo);
            timePassed = ofGetElapsedTimeMillis() - startMark;
            midTrans = pMerge.getPolyline();
        }
        interpolateCoeff = ofMap(timePassed, 0, globalDurationOfTrans, 0, 1*globalPercentTrans);
        quantityOfNoise = globalAmountOfNoise;
        quiv = globalAmontOfQuiver;
        
        // overall time left unattended
        if ((ofGetElapsedTimeMillis() - startTimeUnused) > 90000 ){
            populateVector();
        }
    }
    else if( state == 2){
        //morphing into object + fading noise
        if(isSetupState){
            isSetupState = false;
            
            startMark = ofGetElapsedTimeMillis();
            transformFrom = nextSill(transformFrom);
            
            // deciding which one to visibly transform into.
            // TO DO: Let's try not to repeat
            int transformTooControlled = transformToo;
            if( interpolateCoeff < .7){
                transformTooControlled = transformFrom;
            }
            transformToo = transformTooControlled;
            
            
            // old code for deciding which index to transform into
            /*
            int transformTooControlled = transformToo;
            bool onList = false;
            int indexToErase = 0;
            for (int i=0; i < randomIndices.size(); i++){
                if (randomIndices.at(i) == transformToo){
                    onList = true;
                    indexToErase = i;
                }
            }
            if (!onList){
                if(randomIndices.size() > 0){
                    int num = ofRandom(randomIndices.size()-1);
                    transformTooControlled = randomIndices.at(num);
                    randomIndices.erase(randomIndices.begin() + num);
                }
                else{
                    populateVector();
                    int num = ofRandom(randomIndices.size()-1);
                    transformTooControlled = randomIndices.at(num);
                    randomIndices.erase(randomIndices.begin() + num);
                }
            }
            else{
                randomIndices.erase(randomIndices.begin() + indexToErase);
            }
            
            transformToo = transformTooControlled;
             */
            
            midTrans = pMerge.getPolyline();
            
            countOfTransforms ++;
            
        }
        
        int timePassed = ofGetElapsedTimeMillis() - startTimeOfState;
        
        
        interpolateCoeff = ofMap(timePassed, 0, durOfImgTrans, 0, 1);
        
        // fade out the noise and the slurp
        quantityOfNoise = ofMap(timePassed, 0, durOfImgTrans, globalAmountOfNoise, 0);
        quiv = ofMap(timePassed, 0, durOfImgTrans, globalAmontOfQuiver, 0);
        
        if(isExcite){
            globalSlurpAlpha = ofMap(timePassed, 0, durOfImgTrans, slurpAlphaExcit, fadeSlurpToo);
        }
        else{
            globalSlurpAlpha = ofMap(timePassed, 0, durOfImgTrans, slurpAlpha, fadeSlurpToo);
        }
        
        if (timePassed > durOfImgTrans-1){
            state = 3;
            isSetupState = true;
            startTimeOfState = ofGetElapsedTimeMillis();
        }
        
        
    }
    else if( state == 3){
        // fade in image (AUTOMATIC)
        if(isSetupState){
            isSetupState = false;
        }
        
        int timePassed = ofGetElapsedTimeMillis() - startTimeOfState;
        alpha = int( ofMap(timePassed, 0,500,255 ,0));
        alphaText = int( ofMap(timePassed, 0,500,0 ,255));
        
        if(timePassed > 499){
            state =4;
            isSetupState = true;
            startTimeOfState = ofGetElapsedTimeMillis();
            alpha = 0;
            alphaText = 255;
            alphaPainting =0;
        }
    }
    else if( state == 4){
        //looking at the solid image (AUTOMATIC)
        
        // if it is a painting item initiate the fade in
        if(isSetupState){
            isSetupState = false;
            startTimeOfState = ofGetElapsedTimeMillis();
            alphaPainting =0;
        }
        int timePassed = ofGetElapsedTimeMillis() - startTimeOfState;
        if((items.at(transformToo).isPainting)){
            alphaPainting = int(ofClamp(ofMap(timePassed, 0, 1000, 0, 255),0,255));
            
        }
        alpha = 0;
    }
    
    else if( state == 5){
        //fade out image
        if(isSetupState){
            isSetupState = false;
            alpha = 0;
            
        }
        
        int timePassed = ofGetElapsedTimeMillis() - startTimeOfState;
        alpha = int( ofMap(timePassed, 0,500,0,255));
        alphaText = int( ofMap(timePassed, 0,500,255,0));
        alphaPainting = int(ofMap(timePassed,0,500,255,0));
        
        if(timePassed > 499){
            state =6;
            startTimeOfState = ofGetElapsedTimeMillis();
            isSetupState = true;
            alpha = 255;
            alphaText = 0;
            alphaPainting =0;

        }
    }
    else if( state == 6){
        if(isSetupState){
            alpha = 255;
            isSetupState = false;
            midTrans = pMerge.getPolyline();
        }
        //return to normal image fade back in noise (AUTOMATIC)
        //colOfForm = color2;
        int timePassed = ofGetElapsedTimeMillis() - startMark;
        if (timePassed > globalDurationOfTrans-1){
            startMark = ofGetElapsedTimeMillis();
            transformFrom = nextSill(transformFrom);
            transformToo = nextSill(transformToo);
            timePassed = ofGetElapsedTimeMillis() - startMark;
            midTrans = pMerge.getPolyline();
        }
        
        int timePassed2 = ofGetElapsedTimeMillis() - startTimeOfState;
        interpolateCoeff = ofMap(timePassed, 0, globalDurationOfTrans, 0, 1*globalPercentTrans);
        quantityOfNoise = ofMap(timePassed2, 0, durOfImgTrans, 0, globalAmountOfNoise);
        quiv = globalAmontOfQuiver;
        
        // reIntro the slurp
        if(isExcite){
            globalSlurpAlpha = ofMap(timePassed, 0, durOfImgTrans, fadeSlurpToo, slurpAlphaExcit.get());
        }
        else{
            globalSlurpAlpha = ofMap(timePassed, 0, durOfImgTrans,  fadeSlurpToo, slurpAlpha.get());
        }
        
        if(timePassed2 > durOfImgTrans-1){
            state =1;
            startTimeOfState = ofGetElapsedTimeMillis();
            isSetupState = true;
        }
    }
    
    if(state != 4){
        pMerge.mergePolyline(midTrans, items.at(transformToo).poly, interpolateCoeff,quantityOfNoise, quiv);
    }
    
    if(isSensor){
    
        ardTalk.update();
        
        if((ardTalk.averagedOut > sensorThresh) & !isTriggered){
            
            // Let's show an image!
            state = 2;
            isTriggered = true;
            if(!isExcite){
                isTransIntoExcite = true;
            }
            isTransOutOfExcite = false;
            startTimeOfState = ofGetElapsedTimeMillis();
            isSetupState = true;
        }
        // transform back into the blob if the person moves out of the threshold and is moving face
        else if((ardTalk.averagedOut < sensorThresh) & isTriggered & (ardTalk.averagedOutDiff > motionDifference )){
            state = 5;
            isTriggered = false;
            startTimeOfState = ofGetElapsedTimeMillis();
            isSetupState = true;
        
            
            // clear out draw trailing
            // might not be necessary anymore
            /*
            drawTrailing.begin();
                ofClear(0,0,0,255);
                ofSetColor(0);
                ofDrawRectangle(0, 0, drawTrailing.getWidth(), drawTrailing.getHeight());
            drawTrailing.end();
            */
        }
        
        // go into excited mode
        else if((ardTalk.averagedOut > excitedThresh) & !isExcite & !isTransIntoExcite & !isTriggered & (ardTalk.averagedOut < sensorThresh) ){
            isTransIntoExcite = true;
            isTransOutOfExcite = false;
            startTimeOfExciteFade =  ofGetElapsedTimeMillis();

        }
       
        else if(ardTalk.averagedOut < excitedThresh ){
            if(isTriggered){
                //triggerNext();
                state = 5;
                isTriggered = false;
                startTimeOfState = ofGetElapsedTimeMillis();
                isSetupState = true;

                drawTrailing.begin();
                    ofClear(0,0,0,255);
                    ofSetColor(0);
                    ofDrawRectangle(0, 0, drawTrailing.getWidth(), drawTrailing.getHeight());
                drawTrailing.end();
            }
            if(!isTransOutOfExcite & isExcite){
                isTransOutOfExcite = true;
                isTransIntoExcite = false;
                startTimeOfExciteFade =  ofGetElapsedTimeMillis();
            }
        }
    
    }
    
    if(isTransIntoExcite){
        float timeElapsed = ofGetElapsedTimeMillis() - startTimeOfExciteFade;
        //float mappedVal = ofxTween::map(timeElapsed, 0., durOfTransIntoExcite, 0, 1, true, easingQuart, easingType);
        float mappedVal =  ofxeasing::map_clamp(timeElapsed+ 0.f, 0.f, durOfTransIntoExcite+ 0.f, 0.f, 1.f, &ofxeasing::quart::easeIn);
        
        globalPercentTrans = ofMap(mappedVal,0,1,percentTrans,percentTransExcit);
        globalAmountOfNoise = ofMap(mappedVal,0,1,amountOfNoise,amountOfNoiseExcit);
        globalAmontOfQuiver = ofMap(mappedVal,0,1,amontOfQuiver,amontOfQuiverExcit);;
        
        globalDurationOfTrans = ofMap(mappedVal,0,1,durationOfTrans,durationOfTransExcit);
        colorOfBlob = color.get().getLerped(colorExcit.get(), mappedVal);
        colorOfBackground = color2.get().getLerped(color2Excit.get(), mappedVal);
        
        if((state  < 2) | (state  > 6)){
            globalSlurpAlpha = ofMap(mappedVal,0,1,slurpAlpha,slurpAlphaExcit);
        }
        
        globalSlurpNoise = ofMap(mappedVal,0,1,slurpNoise,slurpNoiseExcit);
        globalSlurpQuiver = ofMap(mappedVal,0,1,slurpQuiver,slurpQuiverExcit);
        globalAmontOfGaus = ofMap(mappedVal,0,1,amontOfGaus,amontOfGausExcit);
        globalFilterThresh = ofMap(mappedVal,0,1,filterThresh,filterThreshExcit);
        
        globalBtMotionBlur = ofMap(mappedVal,0,1,BtMotionBlur,BtMotionBlurExcit);
        globalTpMotionBlur = ofMap(mappedVal,0,1,TpMotionBlur,TpMotionBlurExcit);
        globalfinalPassBlur = ofMap(mappedVal,0,1,finalPassBlur,finalPassBlurExcit);
        
        
        if(timeElapsed >= durOfTransIntoExcite){
            isTransIntoExcite = false;
            isExcite = true;
            leftOverFadeTime =0;
        }
        leftOverFadeTime = timeElapsed;
    }
    
    
    else if (isTransOutOfExcite){
        float timeElapsed = ofGetElapsedTimeMillis() - startTimeOfExciteFade;
        //float mappedVal = ofxTween::map(timeElapsed, 0., durOfTransOutExcite, 0, 1, true, easingQuart, easingType);
        
        float mappedVal =  ofxeasing::map_clamp(timeElapsed+ 0.f, 0.f, durOfTransOutExcite+ 0.f, 0.f, 1.f, &ofxeasing::quart::easeIn);
        
        globalPercentTrans = ofMap(mappedVal,0,1,percentTransExcit,percentTrans);
        globalAmountOfNoise = ofMap(mappedVal,0,1,amountOfNoiseExcit,amountOfNoise);
        globalAmontOfQuiver = ofMap(mappedVal,0,1,amontOfQuiverExcit,amontOfQuiver);;
        
        globalDurationOfTrans = ofMap(mappedVal,0,1,durationOfTransExcit,durationOfTrans);
        colorOfBlob = colorExcit.get().getLerped(color.get(), mappedVal);
        colorOfBackground = color2Excit.get().getLerped(color2.get(), mappedVal);
        
        if((state  < 2) | (state  > 6)){
            globalSlurpAlpha = ofMap(mappedVal,0,1,slurpAlphaExcit,slurpAlpha);
        }
        
        globalSlurpNoise = ofMap(mappedVal,0,1,slurpNoiseExcit,slurpNoise);
        globalSlurpQuiver = ofMap(mappedVal,0,1,slurpQuiverExcit,slurpQuiver);
        globalAmontOfGaus = ofMap(mappedVal,0,1,amontOfGausExcit,amontOfGaus);
        globalFilterThresh = ofMap(mappedVal,0,1,filterThreshExcit,filterThresh);
        
        globalBtMotionBlur = ofMap(mappedVal,0,1,BtMotionBlurExcit,BtMotionBlur);
        globalTpMotionBlur = ofMap(mappedVal,0,1,TpMotionBlurExcit,TpMotionBlur);
        globalfinalPassBlur = ofMap(mappedVal,0,1,finalPassBlurExcit,finalPassBlur);
        
        if(timeElapsed >= durOfTransOutExcite){
            isTransOutOfExcite = false;
            isExcite = false;
            leftOverFadeTime =0;
        }
        leftOverFadeTime = timeElapsed;
        
    }
    else if(isExcite & !isTriggered){
        globalPercentTrans = percentTransExcit;
        globalAmountOfNoise = amountOfNoiseExcit;
        globalAmontOfQuiver = amontOfQuiverExcit;
        
        globalDurationOfTrans = durationOfTransExcit;
        colorOfBlob = colorExcit;
        colorOfBackground = color2Excit;
        
        if((state  < 2) | (state  > 6)){
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
    else if(!isTriggered){
        
        globalPercentTrans = percentTrans;
        globalAmountOfNoise = amountOfNoise;
        globalAmontOfQuiver = amontOfQuiver;
        
        globalDurationOfTrans = durationOfTrans;
        colorOfBlob = color;
        colorOfBackground = color2;
        if((state  < 2) | (state  > 6)){
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




void morph::populateVector(){
    randomIndices.clear();
    for(int i = 0; i < (int)dir.size(); i++){
        randomIndices.push_back(i);
    }
}

void morph::saveGuiSettings(){
    gui.saveToFile("settings/settings.xml");
    guiExcited.saveToFile("settings/settings2.xml");
}
void morph::drawGui(int x,int y){
    
   
}

void morph::drawMorph(int x,int y){
    
    
    ofSetColor(color3);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    ofPushMatrix();
    
        int rightOffset = ofGetWidth()*(rightMask/100.0);
        int leftOffset = ofGetWidth()*(leftMask/100.0);
        int upperOffset = ofGetHeight()*(upperMask/100.0);
        int lowerOffset = ofGetHeight()*(lowerMask/100.0);
    
        ofTranslate(blobOffset->x + leftOffset + (ofGetWidth() - (leftOffset + rightOffset))/2, blobOffset->y + upperOffset + (ofGetHeight()- (upperOffset + lowerOffset))/2);

        int flipx = 1;
        int flipy = 1;
        if(flipVert){
            flipy = -1;
        }
        if(flipHor){
            flipx= -1;
        }
    
        ofScale(renderScale* flipx, renderScale* flipy);
    
        if(rotation.get() ==0){
            ofRotateZ(0);
        }
        else{
            ofRotateZ(360/(rotation+1));
        }

        // draw images if fadeing in, displaying or fading out image
        if((state  >= 3) & (state  <= 5)){
            ofVec2f sz = getSz(items.at(transformToo).img);
        
            if(items.at(transformToo).isPainting){
                ofSetColor(255,alphaPainting);
                items.at(transformToo).imgForPainting.draw(-1*(sz.x/1.5)/2,-1*(sz.y/1.5)/2, sz.x/1.5,sz.y/1.5 );
                ofSetColor(255);
            }
        
            ofSetColor(255);
            items.at(transformToo).img.draw(-1*(sz.x/1.5)/2,-1*(sz.y/1.5)/2, sz.x/1.5,sz.y/1.5 );
            // draw text
            string titleText = items.at(transformToo).blurb.getRow(0).getString(0);
            string museumName = items.at(transformToo).blurb.getRow(0).getString(1) +"\n" + items.at(transformToo).blurb.getRow(0).getString(2);
            ofSetColor(textColor.get().r,textColor.get().g,textColor.get().b,alphaText);
            title.drawString(titleText, -1*(sz.x/1.5)/2, ((sz.y/1.5)/2)+ title.stringHeight(titleText)+ underImgMargin);
            body.drawString(museumName, -1* (sz.x/1.5)/2 , ((sz.y/1.5)/2)+ title.stringHeight(titleText) + underTitle + underImgMargin);
            ofSetColor(255);
        }
    
    
        ofSetColor(255,255);
        // no need to morph when there is a filepath
        if(state != 4){
            pathToPath();
        }
    
    ofPopMatrix();
    
    // draw squares to box off areas with the screen.
    ofSetColor(0);
    ofDrawRectangle(0, 0, ofGetWidth(), upperOffset);
    ofDrawRectangle(0, ofGetHeight()-lowerOffset, ofGetWidth(), lowerOffset);
    ofDrawRectangle(0,0,leftOffset, ofGetHeight());
    ofDrawRectangle(ofGetWidth()- rightOffset,0,rightOffset, ofGetHeight());
    
    
    if( !bHide ){
        gui.draw();
        guiExcited.draw();
        ofSetColor(200);
        ofDrawBitmapString(ofToString(ardTalk.averagedOut), ofGetWidth()/2, 100);
        ofDrawBitmapString(ofToString(ardTalk.averagedOutDiff), ofGetWidth()/2, 50);
        ofDrawBitmapString(ofToString(ofGetFrameRate()), ofGetWidth()/2, 20);

    }
    
    
}

void morph::triggerNext(){
        if (state == 4 ){
            state = 5;
            //isTriggered = !isTriggered;
        }
        else{
            state = 2;
        }
        startTimeOfState = ofGetElapsedTimeMillis();
        isSetupState = true;
}




int morph::nextSill(int num){
    int siz = items.size();
    return  num = (int) ofRandom(0, siz);
    
}


void morph::drawWithGL(ofPolyline pols, int res){
    ofSetPolyMode(OF_POLY_WINDING_ODD);
    if(pols.getVertices().size()> 1){
        ofBeginShape();
        for(int i=0; i < pols.getVertices().size(); i+=res){
            ofCurveVertex(pols.getVertices().at(i).x,pols.getVertices().at(i).y);
        }
        ofEndShape();
    }
}



void morph::pathToPath(){
   
    // adjust the amount of fade...
    
    // this is how fast the motion trail is faded away.
    int alphVal;
    if((state  < 2) | (state  > 6)){
       alphVal = int(ofMap(ofNoise(ofGetElapsedTimef()*globalSlurpQuiver) ,0,1,globalSlurpAlpha + globalSlurpNoise *-1,globalSlurpAlpha + globalSlurpNoise));
    }
    else{
        alphVal = globalSlurpAlpha;
    }
    
    // draw the previous trail and blur it
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    gpuBlur1.beginDrawScene();
        ofClear(0, 0, 0, 0);
        ofSetColor(255);
        drawTrailing.draw(0,0, ofGetWidth(), ofGetHeight());
    gpuBlur1.endDrawScene();
    
    gpuBlur1.performBlur();
    
   
    
    
    drawTrailing.begin();
        ofPushMatrix();
            ofSetColor(255);
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); //pre-multiplied alpha
            ofTranslate(0,0 );
            ofScale(2,2);
            gpuBlur1.drawBlurFbo();
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofPopMatrix();
    
        // draw the background to fade away the slurp
        ofSetColor(0,0,0, alphVal);
        ofDrawRectangle(0,0,drawTrailing.getWidth(),drawTrailing.getHeight());
    
        // draw the blob at twice the scale
        // will scale down later to increase resolution
        ofTranslate(drawTrailing.getWidth()/2,drawTrailing.getHeight()/2);
        ofScale(2, 2);
        ofSetColor(255);
        drawWithGL(pMerge.getPolyline(), 1);
    
    drawTrailing.end();
    
    
    ofSetColor(255);
    motionBlur.begin();
        ofClear(255,255,255,0);
        threshShade.begin();
            threshShade.setUniform4f("blobCol", colorOfBlob.r/255.f, colorOfBlob.g/255.f, colorOfBlob.b/255.f, colorOfBlob.a/255.f);
            threshShade.setUniform4f("backCol", colorOfBackground.r/255.f, colorOfBackground.g/255.f, colorOfBackground.b/255.f, colorOfBackground.a/255.f);
            threshShade.setUniform1f("thresholdPnt",globalFilterThresh);
            threshShade.setUniform1f("motionBlrBt",globalBtMotionBlur);
            threshShade.setUniform1f("motionBlrTp",globalTpMotionBlur);
            threshShade.setUniform1f("alphaVal",alpha/255.f);
            drawTrailing.draw(0,0);
        threshShade.end();
    motionBlur.end();
    
    //motionBlur.draw(-ofGetWidth()/2 ,-ofGetHeight()/2,ofGetWidth(), ofGetHeight());


    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    gpuBlur2.beginDrawScene();
        ofClear(0, 0, 0, 0);
        ofSetColor(255);
        motionBlur.draw(0,0,ofGetWidth(),ofGetHeight());
    gpuBlur2.endDrawScene();
    
    gpuBlur2.performBlur();
    
    
    ofPushMatrix();
        ofSetColor(255);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); //pre-multiplied alpha
        ofTranslate(-ofGetWidth()/2 ,-ofGetHeight()/2 );
        gpuBlur2.drawBlurFbo();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofPopMatrix();

    ofSetColor(255);

}




ofPolyline morph::pngToPolyline(ofImage img){
    
    
    ofPolyline cur;
    cur.clear();
    ofLog()<< "image width: " << img.getWidth();
    ofLog()<< "image width: " << img.getHeight();
    colorImg.allocate(img.getWidth(), img.getHeight());
    grayImg.allocate(img.getWidth(), img.getHeight());
    
    drawNoColor.allocate(img.getWidth(), img.getHeight(),GL_RGB);
    ofPixels pixels;
    pixels.allocate(img.getWidth(), img.getHeight() ,GL_RGBA);
    
    drawNoColor.begin();
    ofClear(0,0,0);
    shade.begin();
    
    
    img.setAnchorPercent(.5, .5);
    //int newHeight = img.getHeight()*(ofGetWidth()/1.5)/img.getWidth();
    ofVec2f sz = getSz(img);
    
    ofPushMatrix();
    int wid = img.getWidth()/2;
    int hi = img.getHeight()/2;
    ofLog()<< "halfwidth: " <<wid;
    ofLog()<< "halfHi: " <<hi;
    ofTranslate(wid, hi);
    //ofRotateZ(90);
    
    img.draw(0,0, sz.x/1.5,sz.y/1.5 );
    ofPopMatrix();
    
    shade.end();
    
    
    drawNoColor.end();
    
    
    drawNoColor.readToPixels(pixels);
    colorImg.setFromPixels(pixels);
    grayImg = colorImg;
    //grayImg.threshold(20);
    contourFinder.findContours(grayImg, 20, ofGetWidth()*ofGetHeight(), 10, true);
    //ofLog()<< contourFinder.nBlobs;
    if(contourFinder.nBlobs > 0){
        // add all the current vertices to cur polyline
        //cur.addVertices(contourFinder.blobs[0].pts);
        for( int i=0; i< contourFinder.blobs[0].pts.size(); i++){
            ofVec2f pos= contourFinder.blobs[0].pts.at(i);
            //pos = ofVec2f(pos.x - wid, pos.y - hi);
            cur.addVertex(pos.x - wid, pos.y - hi,0);
        }
        cur.setClosed(true);
        //cur = cur.getSmoothed(7.);
    }
    //ofLog()<< cur.getVertices().size();
    return cur;
}


ofVec2f morph::getSz(ofImage img){
    float scale = 1.5;
    int wid;
    int height;
    if(img.getWidth() >= img.getHeight() ){
        height = img.getHeight()*(ofGetWidth()/scale)/img.getWidth();
        wid =  ofGetWidth()/scale;
    }
    else{
        wid = img.getWidth()*(ofGetHeight()/scale)/img.getHeight();
        height =  ofGetHeight()/scale;
    }
    return ofVec2f(wid, height);
    
}

