//
//  item.cpp
//  example
//
//  Created by Caroline Record on 9/25/17.
//
//

#include "handleSerial.h"





void handleSerial::setup(){
    serial.listDevices();
    int baud = 9600;
    // Tells the computer which port to be listening to
    serial.setup(0, baud); //open the first device
    
    readTime = 0;
    serialString = "";
    sensorVal = 0;
    
    diffList.assign(3, 0);
    differenceList.assign(3, 0);
    averagedOut = 0;
    averagedOutDiff = 0;
    lastValue = 0;
}

void handleSerial::update(){
    //@add 2015/10/20 ########################################
    serialString = "";
    serialString = ofxGetSerialString(serial,'\n'); //read until end of line
    if(serialString.length()>0){
        sensorVal = ofToInt(serialString);
        
        //int absDiff = abs(result - lastSensorValue);
        diffList.push_front(sensorVal);
        diffList.pop_back();
        averagedOut = averageOfList(diffList);
        
        int diff = abs(lastValue - sensorVal);
        //int absDiff = abs(result - lastSensorValue);
        differenceList.push_front(diff);
        differenceList.pop_back();
        averagedOutDiff = averageOfList(differenceList);
        
        
        lastValue = sensorVal;
    }
    
    ofLogVerbose() << "serialString = " << ofToString(sensorVal) << "\n";
    
}


string handleSerial::ofxGetSerialString(ofSerial &serial, char until) {
    static string str;
    stringstream ss;
    char ch;
    int ttl=1000;
    while ((ch=serial.readByte())>0 && ttl-->0 && ch!=until) {
        ss << ch;
    }
    str+=ss.str();
    if (ch==until) {
        string tmp=str;
        str="";
        return ofxTrimString(tmp);
    } else {
        return "";
    }
}


//@add 2015/10/20 ########################################
// trim right trailing spaces
string handleSerial::ofxTrimStringRight(string str) {
    size_t endpos = str.find_last_not_of(" \t\r\n");
    return (string::npos != endpos) ? str.substr( 0, endpos+1) : str;
}


//@add 2015/10/20 ########################################
// trim left trailing spaces
string handleSerial::ofxTrimStringLeft(string str) {
    size_t startpos = str.find_first_not_of(" \t\r\n");
    return (string::npos != startpos) ? str.substr(startpos) : str;
}


//@add 2015/10/20 ########################################
// trim trailing spaces
string handleSerial::ofxTrimString(string str) {
    return ofxTrimStringLeft(ofxTrimStringRight(str));;
}


float handleSerial::averageOfList(deque<int> list){
    int sum = 0;
    
    for(int i=0; i < list.size(); i++){
        sum += list.at(i);
    }
    float average = sum / list.size();
    return average;
}
