//
//  item.hpp
//  example
//
//  Created by Caroline Record on 9/25/17.
//
//

#include "ofMain.h"



class handleSerial{
    
public:
    
    
    
    
    void setup();
    void update(int fakVal, bool isFake);
    int sensorVal;
    float averagedOut;
    float averagedOutDiff;
    
private:
    
    string ofxGetSerialString(ofSerial &serial, char until);
    string ofxTrimStringRight(string str);
    string ofxTrimStringLeft(string str);
    string ofxTrimString(string str);
    
    ofTrueTypeFont  font;
    float           readTime;           // when did we last read?
    ofSerial        serial;
    string          serialString;
    int lastValue; 
    
    bool isTrigger;
    deque<int> diffList;
    
    deque<int> differenceList;
    float averageOfList(deque<int> list);
    
    

};