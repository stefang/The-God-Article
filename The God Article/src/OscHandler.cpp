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
    for (int i = 0; i < 13; i++) {
        values.push_back(0.0f);
        adjustments.push_back(1.0f);
    }
    
    for (int i = 0; i < 512; i++) {
        fft.push_back(0.0f);
    }

    adjustments.push_back(0.25f);
    meshScale = 1;
    ofSetCircleResolution(40);
    
    singleHeight = 50;
    
    spectrogram.allocate(w, h, OF_IMAGE_COLOR);
}

oscHandler::~oscHandler() {
    values.clear();
    adjustments.clear();
}

void oscHandler::update(bool record) {
    while(receiver->hasWaitingMessages()){
        ofxOscMessage m;
        receiver->getNextMessage(&m);
        // cout << m.getAddress() << endl;
        if(m.getAddress() == "/amp"){
            values[0] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/freq"){
            values[1] = m.getArgAsFloat(0) / 20000;
        }
        if(m.getAddress() == "/specCentroid"){
            values[2] = m.getArgAsFloat(0) / 20000;
        }
        if(m.getAddress() == "/specFlatness"){
            values[3] = m.getArgAsFloat(0);
        }
        if(m.getAddress() == "/loudness"){
            values[4] = m.getArgAsFloat(0) / 64;
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
        if(m.getAddress() == "/fftData"){
            fft.clear();
            for (int i = 0; i < m.getNumArgs(); i++) {
                fft.push_back(m.getArgAsFloat(i));
            }
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
        if(m.getAddress() == "/scale/14"){ // Sets waveform and mesh width...
            adjustments[13] = m.getArgAsFloat(0);
        }
    }
    if (record) {
        buffer.push_back(values);
        fftbuffer.push_back(fft);
    }
    updateMeshes();
}

void oscHandler::drawOSCLive(int x, int y) {
//    float wHeight = height * 0.1;
//    ofPushMatrix();
//    ofTranslate(x, y);
//    ofPushMatrix();
//    ofTranslate(0, height * 0.3);
//    ofSetColor(254, 100, 100, 100);
//    ofRect(-7, wHeight * values[1] , 17, values[12] * wHeight);
//    ofPopMatrix();
//    ofPushMatrix();
//    ofTranslate(0, (height * 0.75)+15);
//    ofRect(-7, -(values[4] * (wHeight*2)) , 17, values[4] * (wHeight * 4));
//    ofPopMatrix();
//    ofPushMatrix();
//    ofTranslate(0, (height * 0.55));
//    for (int i = 0; i < 7; i++) {
//        ofSetColor(0,0,157,(values[5+i] * 200));
//        ofRect(-7, 0, 17, 30);
//        if (i == 0 || i == 3) {
//            ofTranslate(0, 60);
//        } else {
//            ofTranslate(0, 40);
//        }
//    }
//    ofPopMatrix();
//    ofPopMatrix();
};

void oscHandler::drawOSCBuffer(int x, int y) {
//    if (buffer.size() > 0) {
//        ofPushMatrix();
//        ofTranslate(x, y);
//        ofSetColor(0, 0, 0, 50);
////        ofRect(0, 0, width, height);
//        ofSetColor(249,249,249,255);
////        ofRect(0, height*0.5 + 15, width, height*0.5 - 15);
//        
//        ofSetColor(0,0,0,50);
//        
//        ofPushMatrix();
//        ofTranslate(0, height * 0.3);
//        amp.draw();
//        freq.draw();
//        ofPopMatrix();
//        ofPushMatrix();
//        ofTranslate(0, (height * 0.75)+15);
//        breath.draw();
//        ofPopMatrix();
//        
//        ofPushMatrix();
//        ofTranslate(0, (height * 0.55)+15);
//        for (int i = 0; i < 7; i++) {
//            fingers[i].draw();
//            if (i == 0 || i == 3) {
//                ofTranslate(0, 60);
//            } else {
//                ofTranslate(0, 40);
//            }
//        }
//        ofPopMatrix();
//        ofPopMatrix();
//    }
}

void oscHandler::drawFingers() {
    ofPushMatrix();
    ofSetColor(0, 0, 0, 100);
    ofRect(0, 0, width, singleHeight);
    float step = (singleHeight / fingers.size());
    for (int i = 0; i < fingers.size(); i++) {
        fingers[i].draw();
        if (i == 0 || i == 3) {
            ofTranslate(0, step);
        } else {
            ofTranslate(0, step);
        }
    }
    ofPopMatrix();
}

void oscHandler::drawCircularBuffer(int x, int y, int pos) {
    ofPushMatrix();
    ofTranslate(x, y);
    if (buffer.size() > 0 && pos < buffer.size()-1) {
        ofSetColor(234,234,205);
        ofRect(0, 0, width, height);
        ofSetColor(255, 255, 255, 200);
        vector<float> vals = buffer[pos];
        ofCircle(ofGetWidth()*0.5, height*0.5, (height*0.4) * vals[1]);
        ofSetColor(126,137,145, 125);
        ofCircle(ofGetWidth()*0.5, height*0.5, (height*0.4) * vals[12]);
        ofPushStyle();
        ofPolyline noise;
        float r = height*0.44;
        for (int d = 0; d < 360; d++) {
            float x, y, rn;
            rn = vals[3]*5;
            x = (r + (rn*sin(d*600))) * cos(ofDegToRad(d));
            y = (r + (rn*sin(d*600))) * sin(ofDegToRad(d));
            noise.addVertex(ofPoint((ofGetWidth()*0.5)+x,(height*0.5)+y));
        }
        noise.close();
        ofSetColor(255,255,255,vals[3]*255);
        ofSetLineWidth(2);
        ofNoFill();
        noise.draw();
        ofPopStyle();
        
    }
    ofPopMatrix();
}

void oscHandler::drawCircularLive(int x, int y) {
    ofPushMatrix();
    ofTranslate(x, y);
    ofSetColor(0, 0, 0, 50);
    ofCircle(ofGetWidth()*0.5, height*0.5, (height*0.4) * values[0]);
    ofSetColor(255,137,145, 50);
    ofCircle(ofGetWidth()*0.5, height*0.5, (height*0.4) * values[12]);
    ofPushStyle();
    ofPolyline noise;
    float r = height*0.46;
    for (int d = 0; d < 360; d++) {
        float x, y, rn;
        rn = values[3]*5;
        x = (r + (rn*sin(d*600))) * cos(ofDegToRad(d));
        y = (r + (rn*sin(d*600))) * sin(ofDegToRad(d));
        noise.addVertex(ofPoint((ofGetWidth()*0.5)+x,(height*0.5)+y));
        // ofRect((ofGetWidth()*0.5)+x,(height*0.5)+y,1,1);
    }
    noise.close();
    ofSetColor(0,0,0,values[3]*255);
    ofSetLineWidth(1);
    ofNoFill();
    noise.draw();
    ofPopStyle();
    ofPopMatrix();
};

void oscHandler::updateFFT() {
    if (fftbuffer.size() > 0) {
        spectrogram.resize(fftbuffer.size(), 512);
        for (int h = 0; h < fftbuffer.size(); h++) {
            for (int o = 0; o < 512; o++) {
                ofColor c;
                float v = fftbuffer[h][o];
                
                c = ofColor(fftbuffer[h][o]*155, fftbuffer[h][o]*155, fftbuffer[h][o]*155);
                
                spectrogram.setColor(h, o, c);
            }
        }
        spectrogram.resize(width, singleHeight);
        spectrogram.update();
    }

}

void oscHandler::updateMeshes() {
    
    if (buffer.size() > 0) {
        width = buffer.size() * meshScale;
        float step = (float)width / (float)buffer.size();
        
//        amp.clear();
//        amp.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
//        amp.setupIndicesAuto();
//        
//        for (int h = 0; h < buffer.size()-1; h++) {
//            amp.addColor(ofColor(131,163,68,255));
//            amp.addVertex(ofPoint(ofPoint(h*step, singleHeight * ((buffer[h][12] * adjustments[12])), 0)));
//            amp.addColor(ofColor(131,163,68,255));
//            amp.addVertex(ofPoint(ofPoint(h*step, singleHeight * (-(buffer[h][12] * adjustments[12])), 0)));
//            amp.addColor(ofColor(131,163,68,255));
//            amp.addVertex(ofPoint(ofPoint((h+1)*step, singleHeight * ((buffer[h+1][12] * adjustments[12])), 0)));
//            amp.addColor(ofColor(131,163,68,255));
//            amp.addVertex(ofPoint(ofPoint((h)*step, singleHeight * (-(buffer[h][12] * adjustments[12])), 0)));
//        }
        
        freq.clear();
        
        for (int h = 0; h < buffer.size()-1; h++) {
            float cval = abs(buffer[h][1]);
            cval = cval * adjustments[1];
            if (cval > 1) {
                cval = 1;
            }
            freq.addVertex(ofPoint(ofPoint(h*step, singleHeight * ((cval)), 0)));
        }
        
        breath.clear();
        breath.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        breath.setupIndicesAuto();
        
        for (int h = 0; h < buffer.size()-1; h++) {
            
            float cval = abs(buffer[h][4]);
            float nval = abs(buffer[h+1][4]);
            ofColor c = ofColor(240,224,171,255);
            
            cval = cval * adjustments[4];
            nval = nval * adjustments[4];
            
            if (cval > 1) {
                cval = 1;
//                c = ofColor(255,0,0,200);
            }
            if (nval > 1) {
                nval = 1;
//                c = ofColor(255,0,0,200);
            };
            
            breath.addColor(c);
            breath.addVertex(ofPoint(ofPoint(h*step, (singleHeight * 0.5) + ((singleHeight * 0.5) * (-cval)), 0)));
            breath.addColor(c);
            breath.addVertex(ofPoint(ofPoint(h*step, (singleHeight * 0.5) + ((singleHeight * 0.5) * (cval)), 0)));
            breath.addColor(c);
            breath.addVertex(ofPoint(ofPoint((h+1)*step, (singleHeight * 0.5) + ((singleHeight * 0.5) * (nval)), 0)));
            breath.addColor(c);
            breath.addVertex(ofPoint(ofPoint((h)*step, (singleHeight * 0.5) + ((singleHeight * 0.5) * (-cval)), 0)));
        }
        
        fingers.clear();
        for (int i = 0; i < 7; i++) {
            fingers.push_back( ofMesh() );
        }
        float barWidth = (singleHeight / 7);
        for (int i = 0; i < 7; i++) {
            fingers[i].clear();
            fingers[i].setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
            fingers[i].setupIndicesAuto();
            for (int h = 0; h < buffer.size()-1; h++) {
                fingers[i].addColor(ofColor(61,86,157,(buffer[h][5+i] * 200)));
                fingers[i].addVertex(ofPoint(ofPoint(h*step, 0, 0)));
                fingers[i].addColor(ofColor(61,86,157,(buffer[h][5+i] * 200)));
                fingers[i].addVertex(ofPoint(ofPoint(h*step, barWidth, 0)));
                fingers[i].addColor(ofColor(61,86,157,(buffer[h+1][5+i] * 200)));
                fingers[i].addVertex(ofPoint(ofPoint((h+1)*step, barWidth, 0)));
                fingers[i].addColor(ofColor(61,86,157,(buffer[h][5+i] * 200)));
                fingers[i].addVertex(ofPoint(ofPoint((h)*step, 0, 0)));
            }
        }
        
    }
    
}

void oscHandler::setSlot(int slot) {
    currentSlot = slot;
}

void oscHandler::clearBuffer() {
    buffer.clear();
    fftbuffer.clear();
    updateMeshes();
}

int oscHandler::loadBuffer(string fileName) {
    buffer.clear();
    fftbuffer.clear();
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
    
    ofStringReplace(fileName, ".txt", "-fft.txt");
    
    fileContents = ofBufferFromFile(ofToDataPath(fileName));
    lines = ofSplitString(fileContents, "\n");
    for (int l = 0; l < lines.size(); l++) {
        vector<string> vals = ofSplitString(lines[l], ",");
        vector<float> valsf;
        for (int v = 0; v < vals.size(); v++) {
            valsf.push_back(ofToFloat(vals[v]));
        }
        fftbuffer.push_back(valsf);
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
    
    ofStringReplace(fileName, ".txt", "-fft.txt");
    
    ofFile fftFile(ofToDataPath(fileName), ofFile::WriteOnly);
    fftFile.create();
    for (int h = 0; h < fftbuffer.size(); h++) {
        for (int i = 0; i < fft.size(); i++) {
            fftFile << fftbuffer[h][i];
            if (i < fft.size()-1 ) {
                fftFile << ",";
            }
        }
        fftFile << endl;
    }
    fftFile.close();
    
}

