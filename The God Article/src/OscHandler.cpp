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
    
    oscDisplay.allocate(width, height);
    
    for (int i = 0; i < 8; i++) {
        values.push_back(0.0f);
        adjustments.push_back(0.0f);
        ofPolyline cur;
        lines.push_back(cur);
    }
}

void oscHandler::update() {
    while(receiver->hasWaitingMessages()){
        ofxOscMessage m;
        receiver->getNextMessage(&m);
        if(m.getAddress() == "/value/a"){
            values[0] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/value/b"){
            values[1] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/value/c"){
            values[2] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/value/d"){
            values[3] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/value/e"){
            values[4] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/value/f"){
            values[5] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/value/g"){
            values[6] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/value/h"){
            values[7] = m.getArgAsFloat(0);
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
    }
    
    buffer.push_back(values);
    
}

void oscHandler::updateOSCBuffer() {
    
    float step = width / values.size();

    for (int i = 0; i < values.size(); i++) {
        lines[i].clear();
        for (int h = 0; h < buffer.size(); h++) {
            lines[i].addVertex(ofPoint(h*step, buffer[h][i] * 30));
        }
    }
    
    oscDisplay.begin();
    ofBackground(255);
    ofSetColor(0);
    ofPushMatrix();
    for (int i = 0; i < values.size(); i++) {
        ofTranslate(0,20);
        lines[i].draw();
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

int oscHandler::loadBuffer(string fileName) {
    buffer.clear();
}

int oscHandler::saveBuffer(string fileName){

}
