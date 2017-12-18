//
//  morph.cpp
//  example
//
//  Created by Caroline Record on 8/1/17.
//
//

#include "morph.h"

void morph::setup( int x, int y){
    
    largestBlobDimensions = ofVec2f(0,0);
    
    ofxCsv name;
    name.load("CuratorName.csv");
    string curatorName = name.getRow(0).getString(0);
    string pathToImages = "collection_Images/" + curatorName;
    gManager.setup(curatorName);
    
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
        if(temp.isPainting){
            temp.paintingPoly = pngToPolyline(temp.imgForPainting);
        }
        items.push_back(temp);
    }
    

    transformFrom = 0;
    transformToo = 1;

    midTrans = items.at(transformFrom).poly;
    
  
 
    
    isExcite = false;
    isTransIntoExcite = false;
    isTransOutOfExcite = false;
    
    
     
    bHide = true;
    state = 1;
    startTimeOfState = ofGetElapsedTimeMillis();
    startMark = ofGetElapsedTimeMillis();
    isSetupState = false;
    alpha = 255;
    countOfTransforms =0;

    doText = false;
    
    //title.load("font/Klavika-Regular.otf", 20);
    //bool ofTrueTypeFont::load(const string &filename, int fontsize, bool _bAntiAliased=true, bool _bFullCharacterSet=true, bool makeContours=false, float simplifyAmt=0.3f, int dpi=0)
    
    
    
    //body.load("font/franklinGothic.otf", 20);
    
    fboWidth = ((largestBlobDimensions.x /1.5)+50) *gManager.renderScale;
    fboHeight = ((largestBlobDimensions.y /1.5)+50 ) *gManager.renderScale;
    fboXpos = (gManager.blobOffset->x + ofGetWidth()/2) - fboWidth/2;
    fboYPos = (gManager.blobOffset->y + ofGetHeight()/2) - fboHeight/2;
    
    
    alphaPainting =0;
    
    
    motionBlur.allocate(fboWidth*2,fboHeight*2, GL_RGB );
    motionBlur.begin();
    ofClear(255,255,255,0);
    ofSetColor(255);
    ofDrawRectangle(0, 0, motionBlur.getWidth(), motionBlur.getHeight());
    motionBlur.end();
    

    
    drawTrailing.allocate(fboWidth*2, fboHeight*2, GL_RGB );
    
    drawTrailing.begin();
    ofClear(0,0,0,255);
    ofSetColor(0);
    ofDrawRectangle(0, 0, drawTrailing.getWidth(), drawTrailing.getHeight());
    drawTrailing.end();
    
    ardTalk.setup();
    isTriggered = false;
    
    grayImg.allocate(ofGetWidth(), ofGetHeight());
    colorImg.allocate(ofGetWidth(), ofGetHeight() );
    
    

    
    leftOverFadeTime = 0;

    
    
    isSensor = true;
    
    
    ofFbo::Settings s;
    s.width = fboWidth;
    s.height = fboHeight;
    s.internalformat = GL_RGBA;
    s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    s.maxFilter = GL_LINEAR; GL_NEAREST;
    s.numSamples = 0;
    s.numColorbuffers = 1;
    s.useDepth = false;
    s.useStencil = false;

    
    //ofBackground(22);
    ofFbo::Settings t;
    t.width = fboWidth;
    t.height = fboHeight;
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
    
    //glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA | GLUT_MULTISAMPLE);
    //glutInitDisplayString( "rgba double samples>=4 ");
    falseImgPos = 0;
    
    isPaintingSquiggleTime = false;
    lastTransformed = 100;
    
}

void morph::resetValues(){
    startTimeOfState = ofGetElapsedTimeMillis();
    startMark = ofGetElapsedTimeMillis();
}


void morph::update(){
    
    fboXpos = (gManager.blobOffset->x + ofGetWidth()/2) - fboWidth/2;
    fboYPos = (gManager.blobOffset->y + ofGetHeight()/2) - fboHeight/2;
    
    
    gpuBlur1.blurOffset = gManager.globalBlurOneOffset;
    gpuBlur1.blurPasses = gManager.globalBlurOnePasses;

    gpuBlur1.numBlurOverlays = 1;
    gpuBlur1.blurOverlayGain = 255;
    
    gpuBlur2.blurOffset = gManager.globalBlurTwoOffset;
    gpuBlur2.blurPasses = gManager.globalBlurTwoPasses;

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
        if (timePassed > gManager.globalDurationOfTrans-1){
            startMark = ofGetElapsedTimeMillis();
            transformFrom = nextSill(transformFrom);
            transformToo = nextSill(transformToo);
            timePassed = ofGetElapsedTimeMillis() - startMark;
            midTrans = pMerge.getPolyline();
        }
        interpolateCoeff = ofMap(timePassed, 0, gManager.globalDurationOfTrans, 0, 1*gManager.globalPercentTrans);
        quantityOfNoise = gManager.globalAmountOfNoise;
        quiv = gManager.globalAmontOfQuiver;
        
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
            //transformFrom = nextSill(transformFrom);
            
            // deciding which one to visibly transform into.
            // don't repeat the last one that appeared
            int transformTooControlled = transformToo;
            if( interpolateCoeff < .6){
                transformTooControlled = transformFrom;
                if (transformTooControlled == lastTransformed){
                    lastTransformed = transformToo;
                }
            }
            
            
            
            transformToo = transformTooControlled;
            lastTransformed = transformToo;
            
            
            
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
        
        
        interpolateCoeff = ofMap(timePassed, 0, gManager.durOfImgTrans, 0, 1);
        
        // fade out the noise and the slurp
        //quantityOfNoise = ofMap(timePassed, 0, gManager.durOfImgTrans, gManager.globalAmountOfNoise, 0);
        //quiv = ofMap(timePassed, 0, gManager.durOfImgTrans, gManager.globalAmontOfQuiver, 0);
        /*
        if(isExcite){
            gManager.globalSlurpAlpha = ofMap(timePassed, 0, gManager.durOfImgTrans, gManager.slurpAlphaExcit, fadeSlurpToo);
        }
        else{
            gManager.globalSlurpAlpha = ofMap(timePassed, 0, gManager.durOfImgTrans, gManager.slurpAlpha, fadeSlurpToo);
        }
         */
        
        if (timePassed > gManager.durOfImgTrans-1){
            state = 3;
            isSetupState = true;
            startTimeOfState = ofGetElapsedTimeMillis();
            falseImgPos = transformToo;
        }
        
        
    }
    else if( state == 3){
        // fade in image (AUTOMATIC)
        if(isSetupState){
            isSetupState = false;
            gManager.createSnapShot();
        }
        
        int timePass = ofGetElapsedTimeMillis() - startTimeOfState;
        
        
        float scaleVal = ofMap(timePass, 0,500, 0, 1);
        //gManager.colorOfBlob = gManager.colorExcit->getLerped(gManager.colorOfHighlight, scaleVal);
        gManager.scaleIntoImageValue(scaleVal);
        alpha = int( ofMap(timePass, 0,500,0,255));
        alphaText = int( ofMap(timePass, 0,500,0 ,255));
        
        if(timePass > 499){
            state =4;
            isSetupState = true;
            startTimeOfState = ofGetElapsedTimeMillis();
            alpha = 255;
            alphaText = 255;
            alphaPainting =0;
            //gManager.colorOfBlob = gManager.colorOfHighlight;
        }
    }
    else if( state == 4){
        //looking at the solid image (AUTOMATIC)
        gManager.colorOfBlob = gManager.colorOfHighlight;
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
        if(alphaPainting >= 255){
            isPaintingSquiggleTime = true;
        }
        alpha = 255;
    }
    
    else if( state == 5){
        //fade out image
        if(isSetupState){
            isSetupState = false;
            alpha = 255;
            isPaintingSquiggleTime=false;
            gManager.createSnapShot();
            
        }
        
        int timePassed = ofGetElapsedTimeMillis() - startTimeOfState;
        
        float scaleVal = ofMap(timePassed, 0,500, 1,0);
        //gManager.colorOfBlob = gManager.colorExcit->getLerped(gManager.colorOfHighlight, scaleVal);
        
        gManager.scaleIntoExcited(1-scaleVal);
        alpha = int( ofMap(timePassed, 0,500,255,0));
        alphaText = int( ofMap(timePassed, 0,500,255,0));
        alphaPainting = int(ofMap(timePassed,0,500,255,0));
        
        if(timePassed > 500){
            isTransIntoExcite = false;
            isTransOutOfExcite = false;
            state =6;
            startTimeOfState = ofGetElapsedTimeMillis();
            isSetupState = true;
            alpha = 0;
            alphaText = 0;
            alphaPainting =0;
            isExcite = true;

        }
    }
    else if( state == 6){
        if(isSetupState){
            alpha = 0;
            isSetupState = false;
            midTrans = pMerge.getPolyline();
        }
        //return to normal image fade back in noise (AUTOMATIC)
        //colOfForm = color2;
        int timePassed = ofGetElapsedTimeMillis() - startMark;
        if (timePassed > gManager.globalDurationOfTrans-1){
            startMark = ofGetElapsedTimeMillis();
            transformFrom = nextSill(transformFrom);
            transformToo = nextSill(transformToo);
            timePassed = ofGetElapsedTimeMillis() - startMark;
            midTrans = pMerge.getPolyline();
        }
        
        int timePassed2 = ofGetElapsedTimeMillis() - startTimeOfState;
        interpolateCoeff = ofMap(timePassed, 0, gManager.globalDurationOfTrans, 0, 1*gManager.globalPercentTrans);
       // quantityOfNoise = ofMap(timePassed2, 0, gManager.durOfImgTrans, 0, gManager.globalAmountOfNoise);
        //quiv = gManager.globalAmontOfQuiver;
        
        /*
        // reIntro the slurp
        if(isExcite){
            gManager.globalSlurpAlpha = ofMap(timePassed, 0, gManager.durOfImgTrans, fadeSlurpToo, gManager.slurpAlphaExcit.get());
        }
        else{
            gManager.globalSlurpAlpha = ofMap(timePassed, 0, gManager.durOfImgTrans,  fadeSlurpToo, gManager.slurpAlpha.get());
        }
         */
        
        if(timePassed2 > gManager.durOfImgTrans-1){
            state =1;
            startTimeOfState = ofGetElapsedTimeMillis();
            isSetupState = true;
            falseImgPos =0;
        }
    }
    
    if(state != 4){
        pMerge.mergePolyline(midTrans, items.at(transformToo).poly, interpolateCoeff,gManager.globalAmountOfNoise, gManager.globalAmontOfQuiver, gManager.globalDensity);
    }else{
        //bool isP =items.at(transformToo).isPainting;
        //ofLog()<< ofToString(isPaintingSquiggleTime);
        if(isPaintingSquiggleTime == true){
            pMerge.mergePolyline(midTrans, items.at(transformToo).paintingPoly, interpolateCoeff,gManager.globalAmountOfNoise, gManager.globalAmontOfQuiver, gManager.globalDensity);
        }
        else{
             pMerge.mergePolyline(midTrans, items.at(transformToo).poly, interpolateCoeff,gManager.globalAmountOfNoise, gManager.globalAmontOfQuiver, gManager.globalDensity, false);
        }
      
    }
    
    if(isSensor){
    
        ardTalk.update(0,false);
    }
        if((ardTalk.averagedOut > gManager.sensorThresh) & !isTriggered){
            
            // Let's show an image!
            state = 2;
            isTriggered = true;
            if(!isExcite){
                isTransIntoExcite = true;
                gManager.createSnapShot();
            }
            isExcite = true;
            isTransOutOfExcite = false;
            startTimeOfState = ofGetElapsedTimeMillis();
            isSetupState = true;
        }
        // transform back into the blob if the person moves out of the threshold and is moving fast
    
        else if((ardTalk.averagedOut < gManager.sensorThresh) & isTriggered & (ardTalk.averagedOutDiff > gManager.motionDifference )){
            state = 5;
            isTriggered = false;
            startTimeOfState = ofGetElapsedTimeMillis();
            isSetupState = true;
            

        }
        // also transorm into excited if it is less than excited
        else if(ardTalk.averagedOut < gManager.excitedThresh ){
            if(isTriggered){
                //triggerNext();
                state = 5;
                isTriggered = false;
                startTimeOfState = ofGetElapsedTimeMillis();
                isSetupState = true;
                
                /*
                 drawTrailing.begin();
                 ofClear(0,0,0,255);
                 ofSetColor(0);
                 ofDrawRectangle(0, 0, drawTrailing.getWidth(), drawTrailing.getHeight());
                 drawTrailing.end();
                 */
            }
            else{
                if(!isTransOutOfExcite & isExcite){
                    isTransOutOfExcite = true;
                    gManager.createSnapShot();
                    isTransIntoExcite = false;
                    startTimeOfExciteFade =  ofGetElapsedTimeMillis();
                }
            }
        }
        
        // go into excited mode if the sensor is between the two thresholds and isn't any states already
        else if((ardTalk.averagedOut > gManager.excitedThresh.get()) & !isExcite & !isTransIntoExcite & !isTriggered & (ardTalk.averagedOut < gManager.sensorThresh.get())){
            isTransIntoExcite = true;
            gManager.createSnapShot();
            
            isTransOutOfExcite = false;
            startTimeOfExciteFade =  ofGetElapsedTimeMillis();
        }
    
    
    if(isTransIntoExcite & ((state  < 2) | (state  > 6))){
        float timeElapsed = ofGetElapsedTimeMillis() - startTimeOfExciteFade;
        //float mappedVal = ofxTween::map(timeElapsed, 0., durOfTransIntoExcite, 0, 1, true, easingQuart, easingType);
        float mappedVal =  ofxeasing::map_clamp(timeElapsed+ 0.f, 0.f, gManager.durOfTransIntoExcite+ 0.f, 0.f, 1.f, &ofxeasing::quart::easeIn);
        //gManager.scaleExciteValues(true, mappedVal,((state  < 2) | (state  > 6)));
        gManager.scaleIntoExcited(mappedVal);
        
        if(timeElapsed >= gManager.durOfTransIntoExcite){
            isTransIntoExcite = false;
            isExcite = true;
            leftOverFadeTime =0;
        }
        leftOverFadeTime = timeElapsed;
    }
    
    
    else if (isTransOutOfExcite  & ((state  < 2) | (state  > 6))){
        float timeElapsed = ofGetElapsedTimeMillis() - startTimeOfExciteFade;
        //float mappedVal = ofxTween::map(timeElapsed, 0., durOfTransOutExcite, 0, 1, true, easingQuart, easingType);
        float mappedVal =  ofxeasing::map_clamp(timeElapsed+ 0.f, 0.f, gManager.durOfTransOutExcite+ 0.f, 0.f, 1.f, &ofxeasing::quart::easeIn);
        //gManager.scaleExciteValues(false, mappedVal,((state  < 2) | (state  > 6)));
        gManager.scaleIntoNormal(mappedVal);
        
        if(timeElapsed >= gManager.durOfTransOutExcite){
            isTransOutOfExcite = false;
            isExcite = false;
            leftOverFadeTime =0;
        }
        leftOverFadeTime = timeElapsed;
        
    }
    
    if(!bHide & !isTransOutOfExcite & !isTransIntoExcite & state != 3 & state != 5){
        
        if(state == 4 ){
            gManager.initGlobalMovements(3);
        }
        else if (isExcite){
             gManager.initGlobalMovements(2);
        }
        else {
            gManager.initGlobalMovements(1);
        }

    }
    
    /*
    else if(isExcite & !isTriggered){
        
        gManager.initGlobalMovements(true, ((state  < 2) | (state  > 6)));
        
        
    }
    else if(!isTriggered){
        
        gManager.initGlobalMovements(false, ((state  < 2) | (state  > 6)));

    }*/
    
    
    //gManager.initGlobalMovements(isExcite, isTriggered);
    
    

    
}




void morph::populateVector(){
    randomIndices.clear();
    for(int i = 0; i < (int)dir.size(); i++){
        randomIndices.push_back(i);
    }
}

void morph::saveGuiSettings(){
    
}
void morph::drawGui(int x,int y){
    
}

// void applyScale(int wid, int height, int scale, bool isNoOffset=false)

void morph::applyScale(int wid, int height, int scale,  bool isNoOffset){
    
    if(!isNoOffset){
        ofTranslate(gManager.blobOffset->x * scale + leftOffset * scale + (wid - (leftOffset* scale + rightOffset* scale))/2, gManager.blobOffset->y* scale + upperOffset* scale + (height- (upperOffset* scale + lowerOffset* scale))/2);
    }
    else{
        ofTranslate( leftOffset * scale + (wid - (leftOffset* scale + rightOffset* scale))/2, upperOffset* scale + (height- (upperOffset* scale + lowerOffset* scale))/2);
    }
    
    int flipx = 1;
    int flipy = 1;
    if(gManager.flipVert){
        flipy = -1;
    }
    if(gManager.flipHor){
        flipx= -1;
    }
    
    ofScale(gManager.renderScale* flipx* scale, gManager.renderScale* flipy* scale);
    
    if(gManager.rotation.get() ==0){
        ofRotateZ(0);
    }
    else{
        ofRotateZ(360/(gManager.rotation+1));
    }
    //ofDrawRectangle(gManager.blobOffset->x, gManager.blobOffset->y, <#float w#>, <#float h#>)
}


void morph::drawMorph(int x,int y){
    
    
    ofSetColor(gManager.colorOfBackground);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    //ofPushMatrix();
    
         rightOffset = ofGetWidth()*(gManager.rightMask/100.0);
         leftOffset = ofGetWidth()*(gManager.leftMask/100.0);
         upperOffset = ofGetHeight()*(gManager.upperMask/100.0);
         lowerOffset = ofGetHeight()*(gManager.lowerMask/100.0);
    
    
    
        ofSetColor(255,255);
        // no need to morph when there is a filepath
        //if(state != 4){
            pathToPath();
        //}
    
    
    // draw images if fadeing in, displaying or fading out image
        if((state  >= 3) & (state  <= 5)){
            ofVec2f sz = getSz(items.at(falseImgPos).img);
            
            ofPushMatrix();
                applyScale(ofGetWidth(), ofGetHeight(), 1);
                if(items.at(falseImgPos).isPainting){
                    ofSetColor(255,alphaPainting);
                    items.at(falseImgPos).imgForPainting.draw(-1*(sz.x/1.5)/2,-1*(sz.y/1.5)/2, sz.x/1.5,sz.y/1.5 );
                    ofSetColor(255);
                }
                ofSetColor(255,alpha);
                items.at(falseImgPos).img.draw(-1*(sz.x/1.5)/2,-1*(sz.y/1.5)/2, sz.x/1.5,sz.y/1.5 );
            ofPopMatrix();
            
            // draw text
            string allTheText;
            
            for(int i =0; i < items.at(falseImgPos).blurb.getRow(1).getNumCols(); i++){
                string temp =  items.at(falseImgPos).blurb.getRow(1).getString(i);
                if (temp.size() > 0){
                    allTheText += temp + "\n";
                }
            }
            
            ofStringReplace(allTheText, "*", "©");
            /*
            guiOrientation.add(textScale.set("text scale", 1,-1,4));
            guiOrientation.add(textFlipVert.set("text flip vertical", false));
            guiOrientation.add(textFlipHor.set("text flip horizontal", false));
            guiOrientation.add(isImageAnchor.set("is image anchor", true));
            guiOrientation.add(textPos.set("text Pos if no anchor", ofVec2f(50,ofGetHeight()/2), ofVec2f(0,0), ofVec2f(ofGetWidth(),ofGetHeight())));
            */
            
            ofPushMatrix();
            
                //string titleText = items.at(transformToo).blurb.getRow(0).getString(0);
                //string museumName = items.at(transformToo).blurb.getRow(0).getString(1) +"\n" + items.at(transformToo).blurb.getRow(0).getString(2);
                ofSetColor(gManager.textColor.get().r,gManager.textColor.get().g,gManager.textColor.get().b,alphaText);
                if(gManager.isImageAnchor){
                    ofPushMatrix();
                    applyScale(ofGetWidth(), ofGetHeight(), 1);
                }
                else{
                    
                    
                    ofTranslate(gManager.textPos->x, gManager.textPos->y );
                    
                    if(gManager.rotationText.get() ==0){
                        ofRotateZ(0);
                    }
                    else{
                        ofRotateZ(360/(gManager.rotationText+1));
                    }
                    
                }
                //title.drawString(titleText, -1*(sz.x/1.5)/2, ((sz.y/1.5)/2)+ title.stringHeight(titleText)+ gManager.underImgMargin);
                // body.drawString(allTheText, -1* (sz.x/1.5)/2 , ((sz.y/1.5)/2)+ title.stringHeight(titleText) + gManager.underTitle + gManager.underImgMargin);
            
                int flipx = 1;
                int flipy = 1;
                if(gManager.textFlipVert){
                    flipy = -1;
                }
                if(gManager.textFlipHor){
                    flipx= -1;
                }
            
            int num =int(gManager.textScale.get() * 20);
                //body.setGlobalDpi(300);
                //ofScale(gManager.textScale* flipx, gManager.textScale* flipy);
            //" ©"
            
            
                if(gManager.isImageAnchor){
                
                    gManager.body.drawStringAsShapes(allTheText , -1* (sz.x/1.5)/2 , ((sz.y/1.5)/2) + gManager.underTitle + gManager.underImgMargin);
                    ofPopMatrix();
                }else{
                    
                    ofEnableAlphaBlending();
                    ofFill();
                    //ofSetColor(0,0,0);
                    ofRectangle box = gManager.body.getStringBoundingBox(allTheText, 0, 0);
                    ofDrawEllipse(0,0,20,20);
                   // body.drawStringAsShapes(allTheText + " ©", 0,-1*box.getHeight());
                    gManager.body.drawStringAsShapes(allTheText, 0,-1*box.getHeight());
                    gManager.body.drawString(allTheText, 0,-1*box.getHeight());
                }
                ofPopMatrix();
            
                ofSetColor(255);
            
        }
    
    //ofPopMatrix();
    
    // draw squares to box off areas with the screen.
    ofSetColor(0);
    ofDrawRectangle(0, 0, ofGetWidth(), upperOffset);
    ofDrawRectangle(0, ofGetHeight()-lowerOffset, ofGetWidth(), lowerOffset);
    ofDrawRectangle(0,0,leftOffset, ofGetHeight());
    ofDrawRectangle(ofGetWidth()- rightOffset,0,rightOffset, ofGetHeight());
    
    if( !bHide ){
        gManager.draw();
        ofSetColor(100);
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
    int  transformT;

    if(randomIndices.size() > 0){
        int num = ofRandom(randomIndices.size()-1);
        transformT = randomIndices.at(num);
        randomIndices.erase(randomIndices.begin() + num);
    }
    else{
        populateVector();
        int num = ofRandom(randomIndices.size()-1);
        transformT = randomIndices.at(num);
        randomIndices.erase(randomIndices.begin() + num);
    }
    return transformT;
    
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
       alphVal = int(ofMap(ofNoise(ofGetElapsedTimef()*gManager.globalSlurpQuiver) ,0,1,gManager.globalSlurpAlpha + gManager.globalSlurpNoise *-1,gManager.globalSlurpAlpha + gManager.globalSlurpNoise));
    }
    else{
        alphVal = gManager.globalSlurpAlpha;
    }
    
    // draw the previous trail and blur it
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    gpuBlur1.beginDrawScene();
        ofClear(0, 0, 0, 0);
        ofSetColor(255);
        drawTrailing.draw(0,0, fboWidth, fboHeight);
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
    
        ofPushMatrix();
    
            ofSetColor(0,0,0, alphVal);
            ofDrawRectangle(0,0,drawTrailing.getWidth(),drawTrailing.getHeight());
            applyScale(drawTrailing.getWidth(),drawTrailing.getHeight(),2, true);
    
            // draw the background to fade away the slurp
    
            // draw the blob at twice the scale
            // will scale down later to increase resolution
            //ofTranslate(drawTrailing.getWidth()/2,drawTrailing.getHeight()/2);
            //ofScale(2, 2);
            ofSetColor(255);
            drawWithGL(pMerge.getPolyline(), 1);
        ofPopMatrix();
    drawTrailing.end();
    
    
    ofSetColor(255);
    motionBlur.begin();
        ofClear(255,255,255,0);
        threshShade.begin();
            threshShade.setUniform4f("blobCol", gManager.colorOfBlob.r/255.f, gManager.colorOfBlob.g/255.f, gManager.colorOfBlob.b/255.f, gManager.colorOfBlob.a/255.f);
            threshShade.setUniform4f("backCol", gManager.colorOfBackground.r/255.f, gManager.colorOfBackground.g/255.f, gManager.colorOfBackground.b/255.f, gManager.colorOfBackground.a/255.f);
            threshShade.setUniform4f("highCol", gManager.colorOfHighlight.r/255.f, gManager.colorOfHighlight.g/255.f, gManager.colorOfHighlight.b/255.f, gManager.colorOfHighlight.a/255.f);
            threshShade.setUniform1f("thresholdPnt",gManager.globalFilterThresh);
            threshShade.setUniform1f("motionBlrBt",gManager.globalBtMotionBlur);
            threshShade.setUniform1f("motionBlrTp",gManager.globalTpMotionBlur);
            threshShade.setUniform1f("alphaVal",alpha/255.f);
            drawTrailing.draw(0,0);
        threshShade.end();
    motionBlur.end();
    
    //motionBlur.draw(-ofGetWidth()/2 ,-ofGetHeight()/2,ofGetWidth(), ofGetHeight());


    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    gpuBlur2.beginDrawScene();
        ofClear(0, 0, 0, 0);
        ofSetColor(255);
        motionBlur.draw(0,0,fboWidth, fboHeight);
    gpuBlur2.endDrawScene();
    
    gpuBlur2.performBlur();
    
    
    ofPushMatrix();
    ofTranslate(fboXpos, fboYPos);
        ofSetColor(255);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); //pre-multiplied alpha
        //ofTranslate(-ofGetWidth()/2 ,-ofGetHeight()/2 );
        gpuBlur2.drawBlurFbo();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofPopMatrix();

    // try drawing
    
   // ofSetColor(50, 200, 55, 100);
   // ofDrawRectangle(gManager.,0,);
    // ofTranslate(gManager.blobOffset->x * scale + leftOffset * scale + (wid - (leftOffset* scale + rightOffset* scale))/2, gManager.blobOffset->y* scale + upperOffset* scale + (height- (upperOffset* scale + lowerOffset* scale))/2);
    

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
    
    if(sz.x > largestBlobDimensions.x ){
        largestBlobDimensions.x = sz.x;
    }
    
    if(sz.y > largestBlobDimensions.y ){
        largestBlobDimensions.y = sz.y;
    }
    
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

