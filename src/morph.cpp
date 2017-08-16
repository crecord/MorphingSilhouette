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
    
    shade.load("shadersGL3/shader");
    
    dir.allowExt("png");
    dir.listDir(pathToImages);
    dir.sort();
    
    csvDir.allowExt("csv");
    csvDir.listDir(pathToImages);
    csvDir.sort();
    
    //allocate the vector to have as many ofImages as files
    
    if( dir.size() ){
        images.assign(dir.size(), ofImage());
    }
    
    // you can now iterate through the files and load them into the ofImage vector
    for(int i = 0; i < (int)dir.size(); i++){
        images[i].load(dir.getPath(i));
        polys.push_back(pngToPolyline(images[i]));
        ofxCsv temp;
        temp.load(csvDir.getPath(i));
        blurbs.push_back(temp);
        randomIndices.push_back(i);
    }
    
    transformFrom = 0;
    transformToo = 1;
    midTrans = polys.at(transformFrom);
    easingType = ofxTween::easeInOut;
    
  
    gui.setup("panel");
    gui.add(percentTrans.set("amount trans",.6,0,1));
    gui.add(amountOfNoise.set("noise",10,0,30));
    gui.add(amontOfQuiver.set("quiver",10,1,50));
    gui.add(amontOfGaus.set("gaus",1,0,10));
    gui.add(durationOfTrans.set("speed",300,100,10000));
    //durOfImgTrans
    gui.add(durOfImgTrans.set("speed ofImg",200,100,10000));
    gui.add(color.set("color",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    gui.add(color2.set("color 2",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    gui.add(textColor.set("text color",ofColor(0),ofColor(0,0),ofColor(255,255)));
    gui.add(underImgMargin.set("under img Margin",0,100,10));
    gui.add(underTitle.set("under title", 0,100,5));
    gui.loadFromFile("settings.xml");
 
     
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
}


void morph::update(){
    
    
    colOfForm = color;
    ofLog()<< state;

    if( state == 1){
        //some one is here - instruct them
        if(isSetupState){
            isSetupState = false;
            startTimeUnused = ofGetElapsedTimeMillis();
        }
        
        //colOfForm = color2;
        int timePassed = ofGetElapsedTimeMillis() - startMark;
        if (timePassed > durationOfTrans-1){
            startMark = ofGetElapsedTimeMillis();
            transformFrom = nextSill(transformFrom);
            transformToo = nextSill(transformToo);
            timePassed = ofGetElapsedTimeMillis() - startMark;
            midTrans = pMerge.getPolyline();
        }
        interpolateCoeff = ofMap(timePassed, 0, durationOfTrans, 0, 1*percentTrans);
        quantityOfNoise = amountOfNoise;
        quiv = amontOfQuiver;
        
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
            midTrans = pMerge.getPolyline();
            
            countOfTransforms ++;
            
        }
        
        
        
        
        
        int timePassed = ofGetElapsedTimeMillis() - startTimeOfState;
        
        if (timePassed > durOfImgTrans-1){
            state = 3;
            isSetupState = true;
            startTimeOfState = ofGetElapsedTimeMillis();
        }
        interpolateCoeff = ofMap(timePassed, 0, durOfImgTrans, 0, 1);
        quantityOfNoise = ofMap(timePassed, 0, durOfImgTrans, amountOfNoise, 0);
        quiv = ofMap(timePassed, 0, durOfImgTrans, amontOfQuiver, amontOfQuiver*2);
        
        
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
        }
    }
    else if( state == 4){
        //looking at the solid image (AUTOMATIC)
        if(isSetupState){
            isSetupState = false;
        }
        alpha = 0;
        // draw text??
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
        if(timePassed > 499){
            state =6;
            startTimeOfState = ofGetElapsedTimeMillis();
            isSetupState = true;
            alpha = 255;
            alphaText = 0;
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
        if (timePassed > durationOfTrans-1){
            startMark = ofGetElapsedTimeMillis();
            transformFrom = nextSill(transformFrom);
            transformToo = nextSill(transformToo);
            timePassed = ofGetElapsedTimeMillis() - startMark;
            midTrans = pMerge.getPolyline();
        }
        
        int timePassed2 = ofGetElapsedTimeMillis() - startTimeOfState;
        interpolateCoeff = ofMap(timePassed, 0, durationOfTrans, 0, 1*percentTrans);
        quantityOfNoise = ofMap(timePassed2, 0, durOfImgTrans, 0, amountOfNoise);
        quiv = amontOfQuiver;
        if(timePassed2 > durOfImgTrans-1){
            state =1;
            startTimeOfState = ofGetElapsedTimeMillis();
            isSetupState = true;
        }
    }
    
    pMerge.mergePolyline(midTrans, polys.at(transformToo), interpolateCoeff,quantityOfNoise, quiv);
    
    
    
    filled.clear();
    for( int i = 0; i < pMerge.getPolyline().size(); i++) {
        if(i == 0) {
            filled.newSubPath();
            filled.moveTo(pMerge.getPolyline().getVertices()[i] );
        } else {
            filled.curveTo( pMerge.getPolyline().getVertices()[i] );
        }
    }
    
    filled.close();

}


void morph::populateVector(){
    randomIndices.clear();
    for(int i = 0; i < (int)dir.size(); i++){
        randomIndices.push_back(i);
    }
}

void morph::saveGuiSettings(){
    gui.saveToFile("settings.xml");
}
void morph::drawGui(int x,int y){
    
   
}

void morph::drawMorph(int x,int y){
    
    ofClear(color2);
    if((state  >= 3) & (state  <= 5)){
        ofVec2f sz = getSz(images.at(transformToo));
        
        ofPushMatrix();
        ofTranslate(x, y);
        //ofRotateZ(90);
        //images.at(transformToo).setAnchorPoint(.5, .5);
        images.at(transformToo).draw(-1*(sz.x/1.5)/2,-1*(sz.y/1.5)/2, sz.x/1.5,sz.y/1.5 );
        
            // draw text
            string titleText = blurbs.at(transformToo).getRow(0).getString(0);
            string museumName = blurbs.at(transformToo).getRow(0).getString(1) +"\n" + blurbs.at(transformToo).getRow(0).getString(2);
            ofSetColor(textColor.get().r,textColor.get().g,textColor.get().b,alphaText);
            title.drawString(titleText, -1*(sz.x/1.5)/2, ((sz.y/1.5)/2)+ title.stringHeight(titleText)+ underImgMargin);
            body.drawString(museumName, -1* (sz.x/1.5)/2 , ((sz.y/1.5)/2)+ title.stringHeight(titleText) + underTitle + underImgMargin);
        
            // calibration circle
            //ofDrawEllipse(-1*(sz.x/1.5)/2, (sz.y/1.5)/2, 10, 10);
            ofSetColor(255);

        ofPopMatrix();
        
        
    }

    ofPushMatrix();
        ofTranslate(x, y);
        filled.setColor(ofColor(colOfForm.r , colOfForm.g, colOfForm.b, alpha));
        filled.draw();
    ofPopMatrix();
    
    if( !bHide ){
       gui.draw();
    }
    
}

void morph::triggerNext(){
        if (state == 4 ){
            state = 5;
        }
        else{
            state = 2;
        }
        startTimeOfState = ofGetElapsedTimeMillis();
        isSetupState = true;

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

int morph::nextSill(int num){
    int siz = images.size();
    return  num = (int) ofRandom(0, siz);
    
}

ofPolyline morph::pngToPolyline(ofImage img){
    ofPolyline cur;
    cur.clear();
    
    colorImg.allocate(ofGetWidth(), ofGetHeight());
    grayImg.allocate(ofGetWidth(), ofGetHeight());
    
    drawNoColor.allocate(ofGetWidth(), ofGetHeight(),GL_RGB);
    ofPixels pixels;
    pixels.allocate(ofGetWidth(), ofGetHeight() ,GL_RGBA);
    
    drawNoColor.begin();
    ofClear(0,0,0);
    shade.begin();
    
    
    img.setAnchorPercent(.5, .5);
    //int newHeight = img.getHeight()*(ofGetWidth()/1.5)/img.getWidth();
    ofVec2f sz = getSz(img);
    
    ofPushMatrix();
    int wid = ofGetWidth()/2;
    int hi = ofGetHeight()/2;
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
        cur = cur.getSmoothed(7.);
    }
    //ofLog()<< cur.getVertices().size();
    return cur;
    
}