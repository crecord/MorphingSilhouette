//
//  item.cpp
//  example
//
//  Created by Caroline Record on 9/25/17.
//
//

#include "item.h"





void item::setup(string baseFilname, string pathToImages, string pathToCsv, ofImage img2, ofPolyline polys){
    isPaintingReady = false; 
    isPainting = false;
    img = img2;
    poly = polys;
    ofLog()<<pathToImages;
    
    vector<string> slicesForCsv = ofSplitString(pathToImages, ".");
    pathToCsv = slicesForCsv.at(0)+".csv";
    
    vector<string> slices = ofSplitString(pathToImages, "/");
    vector<string> relvantSlice = ofSplitString(slices.at(slices.size()-1), "_");
    if(relvantSlice.size() > 0){
        if(relvantSlice.at(0) == "P"){
            isPainting = true;
            isPaintingReady = true;
            // get the correct filepath to load the painting.
            string fileName = baseFilname + "/painting/" + relvantSlice.at(1);
            vector <string> filenameForCsv = ofSplitString(relvantSlice.at(1), ".");
            pathToCsv = baseFilname + "/" + filenameForCsv.at(0) + ".csv";
            ofLog()<<"filePath: "<<fileName;
            imgForPainting.load(fileName);
        }
    }
    
    blurb.load(pathToCsv);
    
}

