#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetFullscreen(true);
    
    //blob1.setup("shapeTest",ofGetWidth()/2 ,ofGetWidth()/2);
    blob1.setup(ofGetWidth()/2 ,ofGetWidth()/2);
    
    //imageTest.load("curators/Bridgett_Tamiah/painting/5199.png");
    slowCount =0;
    
    fakeSensor = true;
    fakeVal = 0;
}
    




//--------------------------------------------------------------
void ofApp::update(){
    
   // if(slowCount > 1000){
    
    blob1.ardTalk.update(fakeVal, true);
    blob1.update();
   // }
    

    if(slowCount < 5000){
        slowCount++;
        //ofLog()<<slowCount;
    }

    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    //ofBackground(255);
    
    
    //blob1.drawMorph(ofGetWidth()/2,ofGetHeight()/2);
    

    //if(slowCount > 2000){
        blob1.drawMorph(ofGetWidth()/2,ofGetHeight()/2);
   // }
    /*
    else{
        blob1.resetValues();
    }
     */
     
    
    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    // 49 to 52
    if((key >= 49) & (key <= 52)){
        blob1.gManager.guiState = ofToInt(ofToString(char(key)));
        blob1.bHide = false;
    }
    
    else if (key == ' '){
        blob1.triggerNext();
    }
    else if (key == 'h'){
        blob1.bHide = true;
    }
    else if (key =='s'){
        blob1.gManager.saveSettings();
    }
    else if ((blob1.bHide == false) & (key == 'o')){
        blob1.gManager.overRide();
        
    }
    else if (key == 'n'){
        if(fakeVal > 0){
            fakeVal -= 5;
        }
    }
    else if (key == 'm'){
        
          fakeVal += 5;
        
    }
    
}

void ofApp::exit(){
    blob1.gManager.saveSettings();
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
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
