#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetFullscreen(true);
    
    //blob1.setup("shapeTest",ofGetWidth()/2 ,ofGetWidth()/2);
    blob1.setup("collection_Images/Rogers_Rob",ofGetWidth()/2 ,ofGetWidth()/2);
    
    //imageTest.load("curators/Bridgett_Tamiah/painting/5199.png");
    slowCount =0;
}
    




//--------------------------------------------------------------
void ofApp::update(){
    
    if(slowCount > 1000){
    blob1.update();
    }
    

    if(slowCount < 5000){
        slowCount++;
        //ofLog()<<slowCount;
    }

    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    //ofBackground(255);
    
    
    blob1.drawMorph(ofGetWidth()/2,ofGetHeight()/2);
    

    if(slowCount > 2000){
        blob1.drawMorph(ofGetWidth()/2,ofGetHeight()/2);
    }
    else{
        blob1.resetValues();
    }
     
    
    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    if (key == ' '){
        blob1.triggerNext();
    }
    if (key == 'h'){
        blob1.bHide = !blob1.bHide;
       // blob2.bHide = !blob2.bHide;
    }
    else if (key =='s'){
        blob1.saveGuiSettings();
    }
}


void ofApp::exit(){
    blob1.saveGuiSettings();
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
