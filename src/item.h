//
//  item.hpp
//  example
//
//  Created by Caroline Record on 9/25/17.
//
//

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCsv.h"



class item{
    
public:
    
    
    
    
    void setup(string baseFilname, string pathToImages, string pathToCsv, ofImage img, ofPolyline polys);
   
    ofVec2f getSz(ofImage img);
    
    ofImage img;
    ofImage imgForPainting;
    ofPolyline poly;
    ofPolyline paintingPoly;
    ofxCsv blurb;
    bool isPainting;
    bool isPaintingReady; 
    
private:
    
    
    void generatePolyline();
    
    // find the contours of a png that is loaded in.
    
    
};