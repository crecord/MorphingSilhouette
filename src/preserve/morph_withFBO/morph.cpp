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
    gui.add(filterThresh.set("filter thresh",100,1,255));
    
    //durOfImgTrans
    gui.add(durOfImgTrans.set("speed ofImg",200,100,10000));
    gui.add(color.set("color",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    gui.add(color2.set("color 2",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
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
    guiExcited.add(filterThreshExcit.set("filter thresh",100,1,255));
    
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
    isIntersect = false;
    
    
    motionBlur.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA );
    motionBlur.begin();
    ofClear(255,255,255,255);
    ofSetColor(255);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    motionBlur.end();
    
    int slurpRes =2;
    
    drawTrailingBlurY.allocate(ofGetWidth()/slurpRes, ofGetHeight()/slurpRes );

    
    drawTrailingBlurX.allocate(ofGetWidth()/slurpRes, ofGetHeight()/slurpRes);
    
    drawTrailing.allocate(ofGetWidth()/slurpRes, ofGetHeight()/slurpRes, GL_RGB );
    
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
    
    leftOverFadeTime = 0;
    // to ease from excited to non excited state. 
    easingType = ofxTween::easeIn;
    
    
    colorImgSlurp.allocate(ofGetWidth()/slurpRes, ofGetHeight()/slurpRes);
    grayImgSlurp.allocate(ofGetWidth()/slurpRes, ofGetHeight()/slurpRes);
    pixelsSlurp.allocate(ofGetWidth()/slurpRes, ofGetHeight()/slurpRes,GL_RGBA);
    
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
            //instead get the blob including the slurp
            //midTrans = pMerge.getPolyline();
            midTrans = cur;
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
            ofFbo drawSnapshot;
            drawSnapshot.allocate(ofGetWidth(), ofGetHeight());
            drawSnapshot.begin();
            
                ofPushMatrix();
                ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
                ofPushMatrix();
                ofScale(2,2);
                ofSetColor(colorOfBlob.r , colorOfBlob.g, colorOfBlob.b, alpha);
                drawWithGL(cur.getVertices(),1);
                ofPopMatrix();
                drawWithGL(mergedPoints,1);
                ofPopMatrix();
            
            drawSnapshot.end();
            
            
            colorImg.allocate(ofGetWidth(), ofGetHeight());
            grayImg.allocate(ofGetWidth(), ofGetHeight());
            
            ofPixels pixels;
            pixels.allocate(ofGetWidth(), ofGetHeight() ,GL_RGBA);
            
    
            drawSnapshot.readToPixels(pixels);
            colorImg.setFromPixels(pixels);
            grayImg = colorImg;

            
            contourFinder.findContours(grayImg, 20, ofGetWidth() * ofGetHeight(), 10, false);
            midTrans.clear();
            //slurpedPoints = contourFinder.blobs[0].pts;
            for(int i =0; i > contourFinder.blobs[0].pts.size(); i++){
                midTrans.addVertex( contourFinder.blobs[0].pts.at(i).x - ofGetWidth()/2,contourFinder.blobs[0].pts.at(i).y - ofGetHeight()/2 );
            }
            
            //midTrans.addVertices(contourFinder.blobs[0].pts);
            
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
        if(timePassed2 > durOfImgTrans-1){
            state =1;
            startTimeOfState = ofGetElapsedTimeMillis();
            isSetupState = true;
        }
    }
    
    
    if(state != 4){
        pMerge.mergePolyline(midTrans, items.at(transformToo).poly, interpolateCoeff,quantityOfNoise, quiv);
    
        //checkIntersection(pMerge.getPolyline());
        mergedPoints = pMerge.getPolyline().getVertices();
    }
    
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
    
    
    
    if(isTransIntoExcite){
        float timeElapsed = ofGetElapsedTimeMillis() - startTimeOfExciteFade;
        float mappedVal = ofxTween::map(timeElapsed, 0., durOfTransIntoExcite, 0, 1, true, easingQuart, easingType);
        
        
        globalPercentTrans = ofMap(mappedVal,0,1,percentTrans,percentTransExcit);
        globalAmountOfNoise = ofMap(mappedVal,0,1,amountOfNoise,amountOfNoiseExcit);
        globalAmontOfQuiver = ofMap(mappedVal,0,1,amontOfQuiver,amontOfQuiverExcit);;
        
        globalDurationOfTrans = ofMap(mappedVal,0,1,durationOfTrans,durationOfTransExcit);
        colorOfBlob = color.get().getLerped(colorExcit.get(), mappedVal);
        colorOfBackground = color2.get().getLerped(color2Excit.get(), mappedVal);
        
        globalSlurpAlpha = ofMap(mappedVal,0,1,slurpAlpha,slurpAlphaExcit);
        globalSlurpNoise = ofMap(mappedVal,0,1,slurpNoise,slurpNoiseExcit);
        globalSlurpQuiver = ofMap(mappedVal,0,1,slurpQuiver,slurpQuiverExcit);
        globalAmontOfGaus = ofMap(mappedVal,0,1,amontOfGaus,amontOfGausExcit);
        globalFilterThresh = ofMap(mappedVal,0,1,filterThresh,filterThreshExcit);
        if(timeElapsed >= durOfTransIntoExcite){
            isTransIntoExcite = false;
            isExcite = true;
            leftOverFadeTime =0;
        }
        leftOverFadeTime = timeElapsed;
    }
    
    
    else if (isTransOutOfExcite){
        float timeElapsed = ofGetElapsedTimeMillis() - startTimeOfExciteFade;
        float mappedVal = ofxTween::map(timeElapsed, 0., durOfTransOutExcite, 0, 1, true, easingQuart, easingType);
        
        globalPercentTrans = ofMap(mappedVal,0,1,percentTransExcit,percentTrans);
        globalAmountOfNoise = ofMap(mappedVal,0,1,amountOfNoiseExcit,amountOfNoise);
        globalAmontOfQuiver = ofMap(mappedVal,0,1,amontOfQuiverExcit,amontOfQuiver);;
        
        globalDurationOfTrans = ofMap(mappedVal,0,1,durationOfTransExcit,durationOfTrans);
        colorOfBlob = colorExcit.get().getLerped(color.get(), mappedVal);
        colorOfBackground = color2Excit.get().getLerped(color2.get(), mappedVal);
        
        globalSlurpAlpha = ofMap(mappedVal,0,1,slurpAlphaExcit,slurpAlpha);
        globalSlurpNoise = ofMap(mappedVal,0,1,slurpNoiseExcit,slurpNoise);
        globalSlurpQuiver = ofMap(mappedVal,0,1,slurpQuiverExcit,slurpQuiver);
        globalAmontOfGaus = ofMap(mappedVal,0,1,amontOfGausExcit,amontOfGaus);
        globalFilterThresh = ofMap(mappedVal,0,1,filterThreshExcit,filterThresh);
        
        
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
        
        globalSlurpAlpha = slurpAlphaExcit;
        globalSlurpNoise = slurpNoiseExcit;
        globalSlurpQuiver = slurpQuiverExcit;
        globalAmontOfGaus = amontOfGausExcit;
        globalFilterThresh = filterThreshExcit;
    }
    else if(!isTriggered){
        
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
    }

    
}



//http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
int morph::orientation(ofVec2f p, ofVec2f q, ofVec2f r)
{
    // See http://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    float val = (q.y - p.y) * (r.x - q.x) -
    (q.x - p.x) * (r.y - q.y);
    
    if (val == 0) return 0;  // colinear
    
    return (val > 0)? 1: 2; // clock or counterclock wise
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool morph::doIntersect(ofVec2f p1, ofVec2f q1, ofVec2f p2, ofVec2f q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
    
    // General case
    if (o1 != o2 && o3 != o4){
        return true;
    }
    
    return false; // Doesn't fall in any of the above cases
}

void morph::lookForInter(){
    bool toCont = true;
    for(int i=0; i < mergedPoints.size(); i+=2){
        if(toCont & (i < mergedPoints.size()-1 )){
        ofVec2f p1 = mergedPoints.at(i);
        ofVec2f p2 = mergedPoints.at(i+1);
        for(int j=0; j < mergedPoints.size(); j+=2){
            
            if(j < mergedPoints.size()-1){
                ofVec2f p3 = mergedPoints.at(j);
                ofVec2f p4 = mergedPoints.at(j+1);
                bool isSame = (p3 == p1) & (p2 == p4);
                if(!isSame){
                    bool isInter = doIntersect(p1, p2, p3, p4);
                    if(isInter){
                        mergedPoints.erase(mergedPoints.begin()+i, mergedPoints.begin()+j+1);
                        isIntersect = true;
                        //break;
                        toCont =false;
                        lookForInter();
                    }
                }
            }
        }
        }

    }
}

void morph::checkIntersection(ofPolyline ply){
    isIntersect = false;
    //method one- seeing if the lines intersect
    mergedPoints = ply.getVertices();
    lookForInter();
    
    // method two- adding up all the angles
    //https://www.khanacademy.org/math/geometry/hs-geo-foundations/hs-geo-polygons/v/sum-of-the-exterior-angles-of-convex-polygon
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
    
    
    ofSetColor(colorOfBackground);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
    
    ofClear(colorOfBackground);
    
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
    
    
    if((state  >= 3) & (state  <= 5)){
        ofPushMatrix();
        ofVec2f sz = getSz(items.at(transformToo).img);
        ofTranslate(-1*(sz.x/1.5)/2,-1*(sz.y/1.5)/2);
        
        
        //ofPushMatrix();
        //ofTranslate(x + upperMask, y);
        //ofRotateZ(90);
        //images.at(transformToo).setAnchorPoint(.5, .5);
        
        if(items.at(transformToo).isPainting){
            //ofLog()<< alphaPainting;
            ofSetColor(255,alphaPainting);
            items.at(transformToo).imgForPainting.draw(0,0, sz.x/1.5,sz.y/1.5 );
            ofSetColor(255);
        }
        
            ofSetColor(255);
            items.at(transformToo).img.draw(0,0, sz.x/1.5,sz.y/1.5 );
            // draw text
            string titleText = items.at(transformToo).blurb.getRow(0).getString(0);
            string museumName = items.at(transformToo).blurb.getRow(0).getString(1) +"\n" + items.at(transformToo).blurb.getRow(0).getString(2);
            ofSetColor(textColor.get().r,textColor.get().g,textColor.get().b,alphaText);
            title.drawString(titleText, 0, title.stringHeight(titleText)+ underImgMargin);
            body.drawString(museumName,0 , title.stringHeight(titleText) + underTitle + underImgMargin);
        
            // calibration circle
            //ofDrawEllipse(-1*(sz.x/1.5)/2, (sz.y/1.5)/2, 10, 10);
            ofSetColor(255);
        
            ofPopMatrix();
        
        
    }
 
    ofEnableAlphaBlending();
    

  
    if(state != 4){
        if(!isTriggered){
            ofPushMatrix();
            pathToPath();
            ofPopMatrix();
        } else{
        
            ofSetColor(colorOfBlob.r , colorOfBlob.g, colorOfBlob.b, alpha);
            drawWithGL(mergedPoints,1);
        }
    }
    
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

void morph::drawWithGL(vector<ofPoint> pntsToDraw, int resolution){
    ofSetPolyMode(OF_POLY_WINDING_ODD);
    ofBeginShape();
    for(int i=0; i < pntsToDraw.size(); i+=resolution){
        ofCurveVertex(pntsToDraw.at(i));
    }
    ofCurveVertex(pntsToDraw.at(0));
    ofEndShape();
}


void morph::pathToPath(){
   
    //pixelsSlurp.clear();
    //colorImgSlurp.clear();
    
    
    // adjust the amount of fade...
    int alphVal = int(ofMap(ofNoise(ofGetElapsedTimef()*globalSlurpQuiver) ,0,1,globalSlurpAlpha + globalSlurpNoise *-1,globalSlurpAlpha + globalSlurpNoise));
    //int alphVal = slurpAlpha.get();
    // add a little blur to it to avoid sharp trails
    
    //drawTrailing.set
    
    // another one 
    
   
    drawTrailingBlurX.begin();
       // clea
        blurX.begin();
        blurX.setUniform1f("blurAmnt",globalAmontOfGaus);
        ofSetColor(255);
        drawTrailing.draw(0,0);
        blurX.end();
    drawTrailingBlurX.end();
    
    drawTrailingBlurY.begin();
        blurY.begin();
        blurY.setUniform1f("blurAmnt", globalAmontOfGaus);
        ofSetColor(255);

        drawTrailingBlurX.draw(0,0);
        blurY.end();
    drawTrailingBlurY.end();
   
    
    drawTrailing.begin();
   
    //colorImg.draw(0,0);
    //ofClear(0,0,0,20);
    ofSetColor(255);
    drawTrailingBlurY.draw(0,0);
    ofSetColor(0,0,0, alphVal);
    ofDrawRectangle(0,0,drawTrailingBlurY.getWidth(),drawTrailingBlurY.getHeight());

    ofTranslate(drawTrailingBlurY.getWidth()/2,drawTrailingBlurY.getHeight()/2);
    //pth.setColor(ofColor(255));
    //pth.draw();
        ofSetColor(255);
        ofPushMatrix();
        ofScale(.5, .5);
        drawWithGL(mergedPoints,3);
        ofPopMatrix();
    
    drawTrailing.end();
    
   
    //ofSetColor(255);
    //drawTrailing.draw(-ofGetWidth()/2 ,-ofGetHeight()/2);
    //drawTrailingBlurY.draw(0,0);
  
   
    drawTrailing.readToPixels(pixelsSlurp);

    colorImgSlurp.setFromPixels(pixelsSlurp);
    //colorImg.blurGaussian();
    grayImgSlurp = colorImgSlurp;
    //grayImg.blur();
    grayImgSlurp.threshold(globalFilterThresh);
    
    //grayImg.draw(0,0);
    
 
    
    contourFinder.findContours(grayImgSlurp, 20, drawTrailingBlurY.getHeight()*drawTrailingBlurY.getWidth(), 10, false);
    
    //slurpedPoints = contourFinder.blobs[0].pts;
    
    cur.clear();
    cur.addVertices(contourFinder.blobs[0].pts);
    cur = cur.getSmoothed(2);
    
    ofPushMatrix();
    ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
    ofScale(2,2);
    ofSetColor(colorOfBlob.r , colorOfBlob.g, colorOfBlob.b, alpha);
    drawWithGL(cur.getVertices(),1);
    ofPopMatrix();
    
    // draw ontop to give it definition!
    ofPushMatrix();
    //ofScale(1.2, 1.2);
    drawWithGL(mergedPoints,1);
    ofPopMatrix();
    
    /*
    if(contourFinder.nBlobs > 0){
        for( int i=0; i< contourFinder.blobs[0].pts.size(); i+=2){
            slurpedPoints.push_back(ofPoint(contourFinder.blobs[0].pts.at(i).x-ofGetWidth()/2,contourFinder.blobs[0].pts.at(i).y-ofGetHeight()/2));
        }
    }*/
   
     
    /*
    ofPushView();
    ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
    ofSetColor(colorOfBlob.r , colorOfBlob.g, colorOfBlob.b, alpha);
    drawWithGL(contourFinder.blobs[0].pts);
    ofPopView();
    //ofLog()<< contourFinder.nBlobs;
    */
    
    /*
    ofPath cur;
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
        //cur.simplify();
        //cur.simplify();
        //cur
        //cur.setCurveResolution(5);
        
    }*/
    
    /*
    ofPushView();
    cur.setFillColor(ofColor(colorOfBlob.r , colorOfBlob.g, colorOfBlob.b, alpha));
    cur.draw();
    ofPopView();
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
            pos = ofVec2f(pos.x - wid, pos.y - hi);
            cur.addVertex(pos);
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

