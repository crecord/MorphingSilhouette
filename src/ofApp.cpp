#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetFullscreen(true);
    interpolateCoeff = 0;
    //ofHideCursor();

    
    pMerge.setup();
    
    
    shade.load("shadersGL3/shader");
    
    dir.allowExt("png");
    dir.listDir("melissa");
    dir.sort();
    
    //allocate the vector to have as many ofImages as files
    
    if( dir.size() ){
        images.assign(dir.size(), ofImage());
    }
    
    // you can now iterate through the files and load them into the ofImage vector
    for(int i = 0; i < (int)dir.size(); i++){
        images[i].load(dir.getPath(i));
        polys.push_back(pngToPolyline(images[i]));
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
    
    gui.loadFromFile("settings.xml");
    
    bHide = false;
    
    
    state = 0;
    startTimeOfState = ofGetElapsedTimeMillis();
    startMark = ofGetElapsedTimeMillis();
    isSetupState = false;
    
    alpha = 255;
    
    // gaus
    fboBlurOnePass.allocate(ofGetWidth(), ofGetHeight());
    fboBlurTwoPass.allocate(ofGetWidth(), ofGetHeight());
    shaderBlurX.load("shadersGL3/shaderBlurX");
    shaderBlurY.load("shadersGL3/shaderBlurY");
    
    countOfTransforms =0;
}

ofVec2f ofApp::getSz(ofImage img){
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

int ofApp::nextSill(int num){
    int siz = images.size();
    
    return  num = (int) ofRandom(0, siz);
    
}

ofPolyline ofApp::pngToPolyline(ofImage img){
    ofPolyline cur;
    cur.clear();
    
    
    colorImg.allocate(ofGetWidth(), ofGetHeight() );
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
        cur.addVertices(contourFinder.blobs[0].pts);
        cur.setClosed(true);
        cur = cur.getSmoothed(7.);
    }
    //ofLog()<< cur.getVertices().size();
    return cur;
    
}


//--------------------------------------------------------------
void ofApp::update(){
    
    
    colOfForm = color;
    
    
    if ( state == 0){
        // normal flow lure (no one is here)
        if(isSetupState){
            isSetupState = false;
        }
        colOfForm = color;
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
        quiv = amontOfQuiver/2;
    }
    else if( state == 1){
        //some one is here - instruct them
        if(isSetupState){
            isSetupState = false;
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
    }
    else if( state == 2){
        //morphing into object + fading noise
        if(isSetupState){
            isSetupState = false;
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
        if(timePassed > 499){
            state =4;
            isSetupState = true;
            startTimeOfState = ofGetElapsedTimeMillis();
            alpha = 0;
        }
    }
    else if( state == 4){
        //looking at the solid image (AUTOMATIC)
        if(isSetupState){
            isSetupState = false;
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
        if(timePassed > 499){
            state =6;
            startTimeOfState = ofGetElapsedTimeMillis();
            isSetupState = true;
            alpha = 255;
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

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(color2);
    
    if((state  >= 3) & (state  <= 5)){
        ofVec2f sz = getSz(images.at(transformToo));
        
        ofPushMatrix();
            int wid = ofGetWidth()/2;
            int hi = ofGetHeight()/2;
            ofTranslate(wid, hi);
            //ofRotateZ(90);
            images.at(transformToo).setAnchorPoint(.5, .5);
            images.at(transformToo).draw(-1*(sz.x/1.5)/2,-1*(sz.y/1.5)/2, sz.x/1.5,sz.y/1.5 );
        ofPopMatrix();
    }
    
    filled.setColor(ofColor(colOfForm.r , colOfForm.g,colOfForm.b, alpha));
    filled.draw();
    
    if( !bHide ){
        gui.draw();
    }
  
    


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'g'){
       // generatePolyline();
    }
    if(key == 'f'){
        pMerge.setNbPoints(pMerge.getNbPoints()+1);
    }
    else if (key == 'd'){
        if(pMerge.getNbPoints()>3){
            pMerge.setNbPoints(pMerge.getNbPoints()-1);
        }
    }
    else if (key == ' '){

    }
    else if(key =='h'){
    
        bHide = !bHide;
    }

    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == ' '){
        state++;
        startTimeOfState = ofGetElapsedTimeMillis();
        isSetupState = true;
    }
}


void ofApp::exit(){
    gui.saveToFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
