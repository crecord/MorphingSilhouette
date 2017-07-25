#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetFullscreen(true);
    interpolateCoeff = 0;
    lastTime = 0;
    interpolateTime = 1.5;
    
    leap.open();
    
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_NORMALIZE);
    
    //generatePolyline();
    
    pMerge.setup();
    
    
     shade.load("shadersGL3/shader");
    
    dir.allowExt("png");
    dir.listDir("pngs");
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
    
    
    isContract = true;
    isExpand = true;
    
    isImgWaitTime = false;
    isImgTime = false;
    timeFrozen = 0;
    isImageFade = false;
    
    int camWidth = ofGetWidth();  // try to grab at this size.
    int camHeight = 720 * camWidth / 1080;
    
    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
    for(int i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(camWidth, camHeight);
    
    startMark = ofGetElapsedTimeMillis();
    durationOfTrans = 300;
    percentTrans = .6;
    
    
    easingType = ofxTween::easeInOut;

   
   
    gui.setup("panel");
    gui.add(percentTrans.set("amount trans",.6,0,1));
    gui.add(amountOfNoise.set("noise",10,0,30));
    gui.add(durationOfTrans.set("speed",300,100,10000));
    //durOfImgTrans
    gui.add(durOfImgTrans.set("speed ofImg",200,100,10000));
    gui.add(color.set("color",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
    gui.loadFromFile("settings.xml");
    
    bHide = false;
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
    /*
    if (num < siz-1){
        num++;
    }
    else{
        num =0;
    }
    return num;
     */
    
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
            ofTranslate((ofGetWidth()/4)+100, ofGetHeight()/2);
            ofRotateZ(90);
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
    
    
    vidGrabber.update();
    
    fingersFound.clear();
    
    simpleHands = leap.getSimpleHands();
    //ofLog() << "simple: " + ofToString( simpleHands.size());
    
    // leap interaction
    /*
    if(simpleHands.size() >0){
        if(!isImgWaitTime){
            isImgWaitTime = true;
            isImgTime = false;
        }
    }
    else{
        if((isImageFade == false) & isImgTime){
            isImageFade = true;
            startTimeImageFade = ofGetElapsedTimef();
        }
    }*/
    
    
    
    
    if(!isImgTime){
        
        if(isImgWaitTime){
            int timePassed = ofGetElapsedTimeMillis() - startTimeFrozen;
            
             if (timePassed > durationOfTrans-1){
                 // it is transformed the enitre way!
                 //isImgWaitTime = false;
                 isImgTime = true;
                 
             }
            interpolateCoeff = ofMap(timePassed, 0, durOfImgTrans, 0, 1);
        }
        else{
            int timePassed = ofGetElapsedTimeMillis() - startMark;
            if (timePassed > durationOfTrans-1){
                startMark = ofGetElapsedTimeMillis();
                transformFrom = nextSill(transformFrom);
                transformToo = nextSill(transformToo);
                timePassed = ofGetElapsedTimeMillis() - startMark;
                midTrans = pMerge.getPolyline();
            }
            interpolateCoeff = ofMap(timePassed, 0, durationOfTrans, 0, 1*percentTrans);
        }
       // ofxTween::map
        
        //interpolateCoeff = ofxTween::map(timePassed,0, durationOfTrans,  0, 1*percentTrans, clamp, easing, easingType);
        

    /*
    time = (ofGetElapsedTimef()- timeFrozen)-lastTime;
    if (time>interpolateTime*2.) {
        //ofLog()<< "switch!!";
        lastTime = ofGetElapsedTimef()- timeFrozen;
    }
        
    interpolateCoeff = time/interpolateTime;
    if(interpolateCoeff>1.){
        
        if(isContract){
            isExpand = true;
            isContract = false;
            // move forward
            //ofLog() << "ping";
            transformFrom = nextSill(transformFrom);
            if(isImgWaitTime){
                isImgTime = true;
                startTimeFrozen = ofGetElapsedTimef()- timeFrozen;
            }
        }

        interpolateCoeff = 2.-interpolateCoeff;
    }
    else{
        
        if(isExpand){
            isContract = true;
            isExpand = false;
           // ofLog() << "pong";
            transformToo = nextSill(transformToo);
            if(isImgWaitTime){
                isImgTime = true;
                startTimeFrozen = ofGetElapsedTimef()- timeFrozen;
            }
        }
        
    }
     */
    }
    else{
        // time to draw the image
    }


    //ofLog() << interpolateCoeff;
  //  ofLog() << interpolateCoeff;
    if (!isImgWaitTime | !isImgTime){
         pMerge.mergePolyline(midTrans, polys.at(transformToo), interpolateCoeff,amountOfNoise);
    }
    else{
        pMerge.mergePolyline(midTrans, polys.at(transformToo), interpolateCoeff,0);
    }
   
    
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
    //filled.simplify();
    
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0,0,0);
    
    
    ofPushMatrix();
      ofScale(-1, 1);
      ofTranslate(-ofGetWidth(), 0);
      vidGrabber.draw(0, 0);
    ofPopMatrix();
    
    
    if(isImgTime){
        
        
        
        ofPushMatrix();
        ofTranslate((ofGetWidth()/4)+100, ofGetHeight()/2);
        ofRotateZ(90);
        ofVec2f sz = getSz(images.at(transformFrom));
        images.at(transformToo).draw(0,0, sz.x/1.5,sz.y/1.5 );
        ofPopMatrix();
       
        /*
        if(isContract){
            images.at(transformFrom).setAnchorPercent(.5, .5);
            ofVec2f sz = getSz(images.at(transformFrom));
            images.at(transformFrom).draw(0,0, sz.x/1.5,sz.y/1.5 );
        }
        else{
            images.at(transformToo).setAnchorPercent(.5, .5);
            ofVec2f sz = getSz(images.at(transformToo));
            images.at(transformToo).draw(0,0, sz.x/1.5,sz.y/1.5 );
        }
         ofPopMatrix();
        
        
        if(!isImageFade){
            float toMap =ofGetElapsedTimef() - (startTimeFrozen+timeFrozen);
            float scaledAlpha =ofMap(toMap ,0., 1.,255,0 );
            alpha = int( ofClamp(scaledAlpha,0.,255.));
        }
        else{
            
            float toMap =ofGetElapsedTimef() -  startTimeImageFade;
            float scaledAlpha =ofMap(toMap ,0., 1.,0,255 );
            alpha = int( ofClamp(scaledAlpha,0.,255.));
            
            if(alpha >= 255){
                isImgTime = false;
                isImgWaitTime = false;
                timeFrozen = ofGetElapsedTimef() - startTimeFrozen;
                isImageFade = false;
            }
         
        }
         */
    }
    
    else{
        alpha = 255;
    }
    
    
    ofColor col = color;
    filled.setColor(ofColor(col.r , col.g,col.b,alpha));
    //filled.setFilled(false);
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
        if(!isImgWaitTime ){
            isImgWaitTime = true;
            isImgTime = false;
            startTimeFrozen = ofGetElapsedTimeMillis();
            midTrans = pMerge.getPolyline();
        }
    }
    else if(key =='h'){
    
        bHide = !bHide;
    }

    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == ' '){
        isImageFade = true;
        startTimeImageFade = ofGetElapsedTimef();
        
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
