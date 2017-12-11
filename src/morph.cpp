//
//  morph.cpp
//  example
//
//  Created by Caroline Record on 8/1/17.
//
//

#include "morph.h"

void morph::setup(string pathToImages, int x, int y){
    
    
#ifdef TARGET_OPENGLES
    blurX.load("gaus/shadersES2/shaderBlurX");
    blurY.load("gaus/shadersES2/shaderBlurY");
#else
    if(ofIsGLProgrammableRenderer()){
        blurX.load("gaus/shadersGL3/shaderBlurX");
        blurY.load("gaus/shadersGL3/shaderBlurY");
        ofLog()<< "3";
    }
#endif
    
    interpolateCoeff = 0;
    pMerge.setup();
    
    
    dir.allowExt("png");
    dir.listDir(pathToImages);
    dir.sort();
    
    csvDir.allowExt("csv");
    csvDir.listDir(pathToImages);
    csvDir.sort();
    
    // you can now iterate through the files and load them into the ofImage vector
    
    shade.load("shadersGL3/shader");
    threshShade.load("threshShade/threshShade");
    
  //   blurX.load("shadersGL3/shaderBlurX");
    //blurY.load("shadersGL3/shaderBlurY");
    
    //string pathToImages, string pathToCsv, ofImage img, ofPolyline polys
    for(int i = 0; i < dir.size(); i++){
        item temp;
        ofImage imgTemp;
        imgTemp.load(dir.getPath(i));
        ofPolyline ply = pngToPolyline(imgTemp);
        temp.setup(pathToImages, dir.getPath(i), csvDir.getPath(i), imgTemp, ply);
        items.push_back(temp);
    }
    
    // Old images
    /*
    for(int i = 0; i < (int)dir.size(); i++){
        images[i].load(dir.getPath(i));
        polys.push_back(pngToPolyline(images[i]));
        ofxCsv temp;
        temp.load(csvDir.getPath(i));
        blurbs.push_back(temp);
        randomIndices.push_back(i);
    }
     */
    
    transformFrom = 0;
    transformToo = 1;
    //midTrans = polys.at(transformFrom);
    midTrans = items.at(transformFrom).poly;
    
  
    gui.setup("panel");
    gui.add(percentTrans.set("amount trans",.6,0,1));
    gui.add(amountOfNoise.set("noise",10,0,30));
    gui.add(amontOfQuiver.set("quiver",10,1,50));
    
    gui.add(durationOfTrans.set("speed",300,100,10000));
    gui.add(slurpAlpha.set("slurp alpha", 5, 0,255));
    
    gui.add(slurpNoise.set("slurp noise",10,0,50));
    gui.add(slurpQuiver.set("slurp quiver",.5,0,3));
    
    gui.add(amontOfGaus.set("gaus",1,0,20));
    gui.add(filterThresh.set("filter thresh",.2,0,1));
    
    
    gui.add(BtMotionBlur.set("BtMotionBlur",1,-5,5));
    gui.add(TpMotionBlur.set("TpMotionBlur",0,-5,5));
    gui.add(finalPassBlur.set("finalPassBlur",0.3,0,3));

    
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
    
    
    gui.loadFromFile("settings.xml");
    
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
    
    guiExcited.add(amontOfGausExcit.set("gaus",1,0,20));
    guiExcited.add(filterThreshExcit.set("filter thresh",0.2,0,1));
    
    
    guiExcited.add(BtMotionBlurExcit.set("BtMotionBlur",1,-5,5));
    guiExcited.add(TpMotionBlurExcit.set("TpMotionBlur",0,-5,5));
    guiExcited.add(finalPassBlurExcit.set("finalPassBlur",0.3,0,3));

    //durOfImgTrans
    
    guiExcited.add(colorExcit.set("color",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    
    guiExcited.add(color2Excit.set("back col",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    guiExcited.loadFromFile("settings2.xml");
    
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
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    motionBlur.end();
    
    
    
    drawTrailingBlurY.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA );

    
    drawTrailingBlurX.allocate(ofGetWidth(), ofGetHeight());
    
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
    // to ease from excited to non excited state. 
    //easingType = ofxTween::easeIn;
    
    
    colorImgSlurp.allocate(ofGetWidth(), ofGetHeight());
    grayImgSlurp.allocate(ofGetWidth(), ofGetHeight());
    pixelsSlurp.allocate(ofGetWidth(), ofGetHeight(),GL_RGBA);
    
    isSensor = false;
    
}

void morph::resetValues(){
    startTimeOfState = ofGetElapsedTimeMillis();
    startMark = ofGetElapsedTimeMillis();
}


void morph::update(){
    
    

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
            int transformTooControlled = transformToo;
            if( interpolateCoeff < .7){
                transformTooControlled = transformFrom;
            }
            transformToo = transformTooControlled;
            
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
            //instead get the blob including the slurp
            midTrans = pMerge.getPolyline();
            //midTrans = cur;
            /*
            filled.tessellate();
            bool hasOut = filled.hasOutline();
            //vector <ofPolyline> polys = filled.getOutline();
            
            if(filled.getOutline().size()> 0){
                midTrans = filled.getOutline().at(0);
            }
            else{
                midTrans = pMerge.getPolyline();
            }
             */
            //midTrans = filled.getOutline().at(0);
            
            countOfTransforms ++;
            
        }
        
        int timePassed = ofGetElapsedTimeMillis() - startTimeOfState;
        
        if (timePassed > durOfImgTrans-1){
            state = 3;
            isSetupState = true;
            startTimeOfState = ofGetElapsedTimeMillis();
        }
        interpolateCoeff = ofMap(timePassed, 0, durOfImgTrans, 0, 1);
        quantityOfNoise = ofMap(timePassed, 0, durOfImgTrans, globalAmountOfNoise, 0);
        quiv = ofMap(timePassed, 0, durOfImgTrans, globalAmontOfQuiver, globalAmontOfQuiver*2);
        
        if(isExcite){
            globalSlurpAlpha = ofMap(timePassed, 0, durOfImgTrans, slurpAlphaExcit, 150);
        }
        else{
            globalSlurpAlpha = ofMap(timePassed, 0, durOfImgTrans, slurpAlpha, 150);
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
            globalSlurpAlpha = ofMap(timePassed, 0, durOfImgTrans, 103, slurpAlphaExcit.get());
        }
        else{
            globalSlurpAlpha = ofMap(timePassed, 0, durOfImgTrans,  103, slurpAlpha.get());
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
    //checkIntersection(pMerge.getPolyline());
    //mergedPoints = pMerge.getPolyline().getVertices();
    
    
    /*
    filled.clear();
    for( int i = 0; i < mergedPoints.size(); i++) {
        if(i == 0) {
            filled.newSubPath();
            filled.moveTo(mergedPoints.at(i));
        } else {
            filled.curveTo(mergedPoints.at(i));
        }
    }
    filled.close();
     */
    
    //filled.setPolyWindingMode(OF_POLY_WINDING_POSITIVE);
    
    //filled.simplify(.7);
    
    //filled.getOutline();
    //filled = pathToPath(filled);
    
    if(isSensor){
    
    ardTalk.update();
    
    
    

    if((ardTalk.averagedOut > sensorThresh) & !isTriggered){
        //triggerNext();
        state = 2;
        isTriggered = true;
        if(!isExcite){
            isTransIntoExcite = true;
        }
        isTransOutOfExcite = false;
        startTimeOfState = ofGetElapsedTimeMillis();
        isSetupState = true;
        //isExcite = true;
    }
    else if((ardTalk.averagedOut < sensorThresh) & isTriggered & (ardTalk.averagedOutDiff > motionDifference )){

        state = 5;
        isTriggered = false;
        startTimeOfState = ofGetElapsedTimeMillis();
        isSetupState = true;
        
        
        drawTrailing.begin();
        ofClear(0,0,0,255);
        ofSetColor(0);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        drawTrailing.end();
    }
    else if((ardTalk.averagedOut > excitedThresh) & !isExcite & !isTransIntoExcite & (ardTalk.averagedOut < sensorThresh) ){
        
        isTransIntoExcite = true;
        isTransOutOfExcite = false;
        startTimeOfExciteFade =  ofGetElapsedTimeMillis();
        //isExcite = true;
        
        if(isTriggered){
            //triggerNext();
            state = 5;
            isTriggered = false;
            startTimeOfState = ofGetElapsedTimeMillis();
            isSetupState = true;
            
           
            drawTrailing.begin();
            ofClear(0,0,0,255);
            ofSetColor(0);
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
            drawTrailing.end();
            
        }
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
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
            drawTrailing.end();
         
            
            
        }
        if(!isTransOutOfExcite & isExcite){
            isTransOutOfExcite = true;
            isTransIntoExcite = false;
            startTimeOfExciteFade =  ofGetElapsedTimeMillis();
        }
        //isExcite = false;
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
    gui.saveToFile("settings.xml");
    guiExcited.saveToFile("settings2.xml");
}
void morph::drawGui(int x,int y){
    
   
}

void morph::drawMorph(int x,int y){
    
    
    ofSetColor(color3);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
    
    //ofClear(colorOfBackground);
    
   // ofBackground(<#int r#>, <#int g#>, <#int b#>)
    
    

    ofPushMatrix();
    
    int rightOffset = ofGetWidth()*(rightMask/100.0);
    int leftOffset = ofGetWidth()*(leftMask/100.0);
    int upperOffset = ofGetHeight()*(upperMask/100.0);
    int lowerOffset = ofGetHeight()*(lowerMask/100.0);
    
    
    /*
    ofParameter<float> scale;
    ofParameter<int> rotation;
    ofParameter<ofVec2f> blobOffset;
    ofxButton flipVert;
    ofxButton flipHor;
     */
    
    ofSetColor(0);

    
    
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
    
    
    
    /*
    if(!isTriggered){
        ofPushMatrix();
        pathToPath();
        ofPopMatrix();
    } else{
        
        ofSetColor(colorOfBlob.r , colorOfBlob.g, colorOfBlob.b, alpha);
        drawWithGL(pMerge.getPolyline(), 1);
    }*/
    
    
    
    
    
    if((state  >= 3) & (state  <= 5)){
        ofVec2f sz = getSz(items.at(transformToo).img);
        
        //ofPushMatrix();
        //ofTranslate(x + upperMask, y);
        //ofRotateZ(90);
        //images.at(transformToo).setAnchorPoint(.5, .5);
        
        if(items.at(transformToo).isPainting){
            //ofLog()<< alphaPainting;
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
        
            // calibration circle
            //ofDrawEllipse(-1*(sz.x/1.5)/2, (sz.y/1.5)/2, 10, 10);
            ofSetColor(255);

        //ofPopMatrix();
        
        
    }
    
    
    // no need to morph when there is a filepath
    if(state != 4){
        pathToPath();
    }
    
    
    /*
    motionBlur.begin();
    
    ofPushMatrix();
    ofSetColor(color2);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
    
        ofTranslate(x, y);
        //filled.draw();
        filled = pathToPath(filled);
    
        //check intersections
        if(isIntersect){
            filled.setColor(ofColor(0 , 255, 0));
        }
        else{
            filled.setColor(ofColor(colOfForm.r , colOfForm.g, colOfForm.b, alpha));
        }
    
    
   // filled.draw();
    
    ofPopMatrix();
    //ofColor col = color2;

    motionBlur.end();
    
    
    
    motionBlur.draw(0, 0);
    
    */
    
   // ofSetColor(colorOfBlob.r , colorOfBlob.g, colorOfBlob.b, alpha);
    //drawWithGL(mergedPoints);
  
    
    
    
    /*
    //filled.draw();
    if(!isTriggered){
        ofPushMatrix();
        filled = pathToPath(filled);
        ofPopMatrix();
    }
    //check intersections
    
   
    filled.setColor(ofColor(colorOfBlob.r , colorOfBlob.g, colorOfBlob.b, alpha));

        filled.draw();
     */
        
    ofPopMatrix();
    
    
    
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
   
    //pixelsSlurp.clear();
    //colorImgSlurp.clear();
    
    
    // adjust the amount of fade...
    
    int alphVal;
    if((state  < 2) | (state  > 6)){
       alphVal = int(ofMap(ofNoise(ofGetElapsedTimef()*globalSlurpQuiver) ,0,1,globalSlurpAlpha + globalSlurpNoise *-1,globalSlurpAlpha + globalSlurpNoise));
    }
    else{
        alphVal = globalSlurpAlpha;
    }
    //int alphVal = slurpAlpha.get();
    // add a little blur to it to avoid sharp trails
    
    //drawTrailing.set
    
    // another one 
    
   
    drawTrailingBlurX.begin();
        // clea
        //blurY.begin();
        //blurY.setUniform1f("blurAmnt", globalAmontOfGaus);
        ofSetColor(0,0,0, 255);
        ofDrawRectangle(0,0,drawTrailingBlurY.getWidth(),drawTrailingBlurY.getHeight());
        blurX.begin();
        blurX.setUniform1f("blurAmnt",globalAmontOfGaus);
        ofSetColor(255);
        drawTrailing.draw(0,0);
        blurX.end();
    
    drawTrailingBlurX.end();
    
    drawTrailingBlurY.begin();
        ofSetColor(0,0,0, 255);
        ofDrawRectangle(0,0,drawTrailingBlurY.getWidth(),drawTrailingBlurY.getHeight());

        blurY.begin();
        blurY.setUniform1f("blurAmnt", globalAmontOfGaus);
        blurY.setUniform1f("alphaValue", 1);
        ofSetColor(255);
        drawTrailingBlurX.draw(0,0);
        blurY.end();
    
    drawTrailingBlurY.end();
   
    
    drawTrailing.begin();
   
    //colorImg.draw(0,0);
    //ofClear(0,0,0,20);
   // ofSetColor(255);
   // drawTrailingBlurY.draw(0,0);
    ofSetColor(0,0,0, alphVal);
    ofDrawRectangle(0,0,drawTrailing.getWidth(),drawTrailing.getHeight());
    ofTranslate(drawTrailing.getWidth()/2,drawTrailing.getHeight()/2);
    
    //pth.setColor(ofColor(255));
    //pth.draw();
     //   ofScale(2, 2);
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
        //threshShade.setUniform1f("motionBlrBt",globalBtMotionBlur);
        //threshShade.setUniform1f("motionBlrTp",globalTpMotionBlur);
        threshShade.setUniform1f("motionBlrBt",globalBtMotionBlur);
        threshShade.setUniform1f("motionBlrTp",globalTpMotionBlur);
        threshShade.setUniform1f("alphaVal",alpha/255.f);
        //threshShade.setUniform1f("alphaVal",1.f);
    
    
        drawTrailing.draw(0,0);
        threshShade.end();
    
    
    motionBlur.end();
    
    
    drawTrailingBlurX.begin();
    // clea
    //blurY.begin();
    //blurY.setUniform1f("blurAmnt", globalAmontOfGaus);
    
    blurX.begin();
        blurX.setUniform1f("blurAmnt",globalfinalPassBlur);
        ofSetColor(255,0,0);
        motionBlur.draw(0,0);
    blurX.end();
    
    drawTrailingBlurX.end();
    
    
    //drawTrailingBlurY.clear();
    
    drawTrailingBlurY.begin();
    ofClear(255,255,255,0);
    //ofSetColor(255,0,0);
    //ofDrawRectangle(0,0,500,500);
    // I am making an arbitrary change
    
    blurY.begin();
    //blurY.setUniform1f("blurAmnt", globalfinalPassBlur);
    blurY.setUniform1f("alphaValue", alpha/255.f);
    //ofSetColor(255);
    drawTrailingBlurX.draw(0,0);
    blurY.end();
    
    drawTrailingBlurY.end();
    
    
   
    //ofSetColor(255);
    //drawTrailing.draw(-ofGetWidth()/2 ,-ofGetHeight()/2);
    //drawTrailingBlurY.draw(0,0);
  
    ofSetColor(255);

    //drawTrailingBlurY.draw(-ofGetWidth()/2 ,-ofGetHeight()/2, ofGetWidth(), ofGetHeight());
    motionBlur.draw(-ofGetWidth()/2 ,-ofGetHeight()/2, ofGetWidth(), ofGetHeight());
    //ofSetColor(0,255,0);
    //ofDrawRectangle(0,0,400,400);
    
    /*
    
    drawTrailing.readToPixels(pixelsSlurp);
    colorImgSlurp.setFromPixels(pixelsSlurp);
    //colorImg.blurGaussian();
    grayImgSlurp = colorImgSlurp;
    //grayImg.blur();
    grayImgSlurp.threshold(globalFilterThresh);
 .
    //grayImgSlurp.draw(-ofGetWidth()/2 ,-ofGetHeight()/2);
    
    ofSetColor(255);
    grayImgSlurp.draw(-ofGetWidth()/2 ,-ofGetHeight()/2);
    */
    
    
    
   
    //slurpedPoints = contourFinder.blobs[0].pts;
    
    /*
    if(contourFinder.nBlobs > 0){
        for( int i=0; i< contourFinder.blobs[0].pts.size(); i+=2){
            slurpedPoints.push_back(ofPoint(contourFinder.blobs[0].pts.at(i).x-ofGetWidth()/2,contourFinder.blobs[0].pts.at(i).y-ofGetHeight()/2));
        }
    }*/
   
    
    /*
    
   contourFinder.findContours(grayImgSlurp, 20, ofGetWidth()*ofGetHeight(), 10, false);
    
    ofPushView();
    ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
    ofSetColor(colorOfBlob.r , colorOfBlob.g, colorOfBlob.b, alpha);
    drawWithGL(contourFinder.blobs[0].pts);
    ofPopView();
    
   
    
     
     */
    
    
    //ofLog()<< contourFinder.nBlobs;
    /*
    if(contourFinder.nBlobs > 0){
        // add all the current vertices to cur polyline
        //cur.addVertices(contourFinder.blobs[0].pts);
        for( int i=0; i< contourFinder.blobs[0].pts.size(); i++){
            ofVec2f pos= contourFinder.blobs[0].pts.at(i);
            if(i == 0) {
                cur.newSubPath();
                cur.moveTo(ofVec2f(pos.x-ofGetWidth()/2,pos.y-ofGetHeight()/2));
            } else {
                cur.curveTo(ofVec2f(pos.x-ofGetWidth()/2,pos.y-ofGetHeight()/2));
            }
        }
        
        //cur.setClosed(true);
        //cur = cur.getSmoothed(7.);
    }
     */
    
    
    
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

