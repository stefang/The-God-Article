//
//  oschandler.cpp
//  The God Article
//
//  Created by Stefan Goodchild on 12/05/2014.
//
//

#include "oscHandler.h"

oscHandler::oscHandler(ofxOscReceiver* rec, int w, int h) {
    receiver = rec;
    
    width = w;
    height = h;
    
    oscDisplay.allocate(width, height, GL_RGBA);
    oscDisplay.begin();
    ofClear(255,255,255,255);
    oscDisplay.end();
    
    for (int i = 0; i < 13; i++) {
        values.push_back(0.0f);
        adjustments.push_back(1.0f);
    }
}

void oscHandler::update(bool record) {
    while(receiver->hasWaitingMessages()){
        ofxOscMessage m;
        receiver->getNextMessage(&m);
        if(m.getAddress() == "/amp"){
            values[0] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/freq"){
            values[1] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/specCentroid"){
            values[2] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/specFlatness"){
            values[3] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/loudness"){
            values[4] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/Arduino/Hole1"){
            values[5] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/Arduino/Hole2"){
            values[6] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/Arduino/Hole3"){
            values[7] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/Arduino/Hole4"){
            values[8] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/Arduino/Hole5"){
            values[9] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/Arduino/Hole6"){
            values[10] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/Arduino/Hole7"){
            values[11] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/Arduino/Pressure"){
            values[12] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/scale/1"){
            adjustments[0] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/scale/2"){
            adjustments[1] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/scale/3"){
            adjustments[2] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/scale/4"){
            adjustments[3] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/scale/5"){
            adjustments[4] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/scale/6"){
            adjustments[5] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/scale/7"){
            adjustments[6] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/scale/8"){
            adjustments[7] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/scale/9"){
            adjustments[8] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/scale/10"){
            adjustments[9] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/scale/11"){
            adjustments[10] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/scale/12"){
            adjustments[11] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/scale/13"){
            adjustments[12] = m.getArgAsFloat(0);
        }
    }
    
    if (record) buffer.push_back(values);
    
}

void oscHandler::updateOSCBuffer() {
    
    float step = (float)width / (float)buffer.size();
    float vstep = (float)height / (float)values.size()+1;

    oscDisplay.begin();
    ofClear(255,255,255, 0);
    ofSetColor(0,0,0,150);
    ofFill();
    ofPushMatrix();
    for (int i = 0; i < values.size(); i++) {
        ofTranslate(0, vstep);
        for (int h = 0; h < buffer.size(); h++) {
            cout << adjustments[i] << endl;
            ofRect(h*step, 0, step, (buffer[h][i]*adjustments[i])*(vstep*0.5));
        }
    }
    ofPopMatrix();
    oscDisplay.end();
}

void oscHandler::drawOSCBuffer(int x, int y) {
    oscDisplay.draw(x, y);
}

void oscHandler::setSlot(int slot) {
    currentSlot = slot;
}

void oscHandler::clearBuffer() {
    buffer.clear();
}

int oscHandler::loadBuffer(string fileName) {
    buffer.clear();
    string fileContents = ofBufferFromFile(ofToDataPath(fileName));
    vector<string> lines = ofSplitString(fileContents, "\n");
    for (int l = 0; l < lines.size(); l++) {
        vector<string> vals = ofSplitString(lines[l], ",");
        vector<float> valsf;
         for (int v = 0; v < vals.size(); v++) {
             valsf.push_back(ofToFloat(vals[v]));
         }
         buffer.push_back(valsf);
     }
}

int oscHandler::saveBuffer(string fileName){
    ofFile newFile(ofToDataPath(fileName), ofFile::WriteOnly);
    newFile.create();
    for (int h = 0; h < buffer.size(); h++) {
        for (int i = 0; i < values.size(); i++) {
            newFile << buffer[h][i];
            if (i < values.size()-1 ) {
                newFile << ",";
            }
        }
        newFile << endl;
    }
    newFile.close();
}

