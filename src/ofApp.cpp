#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetFullscreen(true);
    
    //ofHideCursor();
    //blob1.setup("shapeTest",ofGetWidth()/2 ,ofGetWidth()/2);
    blob1.setup("Samuels_Diane",ofGetWidth()/2 ,ofGetWidth()/2);
    //blob2.setup("pngs",ofGetWidth()/4,ofGetHeight()/4);
    
    imageTest.load("IMG_0321.JPG");
    
}
    




//--------------------------------------------------------------
void ofApp::update(){
    blob1.update();
   //
    
    //blob2.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    //ofBackground(255);
    
  //  blob1.drawMorph(ofGetWidth()/2,ofGetHeight()/2);
    //blob2.drawMorph(ofGetWidth()/4,ofGetHeight()/4);
    //ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 300);
    
    //imageTest.draw(0,0);
    //ofSetColor(255,100);
    //imageTest.draw(0,0);
    //ofSetColor(255,255,0);
    //ofDrawEllipse(mouseX, mouseY, 50, 50);
    
    
    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    /*
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

    */
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == ' '){
        // trigger next.
        blob1.triggerNext();
    }
    else if (key == 'g'){
        blob2.triggerNext();
    }
    else if (key == 'h'){
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
