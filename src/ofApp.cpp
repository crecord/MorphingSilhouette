#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetFullscreen(true);
    ofSetWindowTitle("morph shape");
    //blob1.setup("shapeTest",ofGetWidth()/2 ,ofGetWidth()/2);
    blob1.setup(ofGetWidth()/2 ,ofGetWidth()/2);
    
    //imageTest.load("curators/Bridgett_Tamiah/painting/5199.png");
    slowCount =0;
    
    fakeSensor = true;
    fakeVal = 0;
}
    




//--------------------------------------------------------------
void ofApp::update(){
    
    
    if(slowCount < 5000){
        slowCount++;
    }
    
   if(slowCount > 1000){
    if(!blob1.isSensor){
        blob1.ardTalk.update(fakeVal, true);
    }
    blob1.update();
    }
   else{
    ofBackground(23, 9, 14);
   }
    

    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    //ofBackground(255);
    
    

   // blob1.drawMorph(ofGetWidth()/2,ofGetHeight()/2);

    //blob1.drawMorph(ofGetWidth()/2,ofGetHeight()/2);

    

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
    
    // 49 to 52
    if((key >= 49) & (key <= 53)){
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
    
    if(!blob1.isSensor){
     if (key == 'n'){
        if(fakeVal > 0){
            fakeVal -= 5;
        }
    }
     if (key == 'm'){
        
          fakeVal += 5;
        
    }
     if (key == 'l'){
        
        fakeVal = 0;
        
    }
     if (key == 'k'){
        
        fakeVal = 150;
        
    }
     if (key == 'j'){
        
        fakeVal = 50;
    }
    }
    
    
     if (key == 'b'){
        blob1.falseImgPos = blob1.nextSill(blob1.falseImgPos);
    }
     if (key == 'a'){
        blob1.isSensor = !blob1.isSensor; 
    }
    
}

void ofApp::exit(){
    blob1.saveAnalytics(); 
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
