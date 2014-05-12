#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(40, 100, 40);
	sender.setup(HOST, PORT);

    for (int i = 0; i < 6; i++) {
        values.push_back(0.0f);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    int i = 0;
    ofxOscMessage a;
    a.setAddress("/amp");
    a.addFloatArg(sin(((i+1) * 0.02) * (float)ofGetFrameNum()));
    sender.sendMessage(a);
    
    i++;
    ofxOscMessage b;
    b.setAddress("/freq");
    b.addFloatArg(sin(((i+1) * 0.02) * (float)ofGetFrameNum()));
    sender.sendMessage(b);
    
    i++;
    ofxOscMessage c;
    c.setAddress("/specCentroid");
    c.addFloatArg(sin(((i+1) * 0.02) * (float)ofGetFrameNum()));
    sender.sendMessage(c);
    
    i++;
    ofxOscMessage d;
    d.setAddress("/specFlatness");
    d.addFloatArg(sin(((i+1) * 0.02) * (float)ofGetFrameNum()));
    sender.sendMessage(d);

    i++;
    ofxOscMessage n;
    n.setAddress("/loudness");
    n.addFloatArg(sin(((i+1) * 0.02) * (float)ofGetFrameNum()));
    sender.sendMessage(n);

    i++;
    ofxOscMessage e;
    e.setAddress("/Arduino/Hole1");
    e.addFloatArg(sin(((i+1) * 0.02) * (float)ofGetFrameNum()));
    sender.sendMessage(e);

    i++;
    ofxOscMessage f;
    f.setAddress("/Arduino/Hole2");
    f.addFloatArg(sin(((i+1) * 0.02) * (float)ofGetFrameNum()));
    sender.sendMessage(f);

    i++;
    ofxOscMessage g;
    g.setAddress("/Arduino/Hole3");
    g.addFloatArg(sin(((i+1) * 0.02) * (float)ofGetFrameNum()));
    sender.sendMessage(g);

    i++;
    ofxOscMessage h;
    h.setAddress("/Arduino/Hole4");
    h.addFloatArg(sin(((i+1) * 0.02) * (float)ofGetFrameNum()));
    sender.sendMessage(h);

    i++;
    ofxOscMessage j;
    j.setAddress("/Arduino/Hole5");
    j.addFloatArg(sin(((i+1) * 0.02) * (float)ofGetFrameNum()));
    sender.sendMessage(j);

    i++;
    ofxOscMessage k;
    k.setAddress("/Arduino/Hole6");
    k.addFloatArg(sin(((i+1) * 0.02) * (float)ofGetFrameNum()));
    sender.sendMessage(k);

    i++;
    ofxOscMessage l;
    l.setAddress("/Arduino/Hole7");
    l.addFloatArg(sin(((i+1) * 0.02) * (float)ofGetFrameNum()));
    sender.sendMessage(l);

    i++;
    ofxOscMessage m;
    m.setAddress("/Arduino/Pressure");
    m.addFloatArg(sin(((i+1) * 0.02) * (float)ofGetFrameNum()));
    sender.sendMessage(m);

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(40, 100, 40);
    ofSetColor(0);
    ofPushMatrix();
    for (int i = 0; i < values.size(); i++) {
        ofTranslate(0,40);
        ofRect(150, 0, values[i] * 100, 10);
    }
    ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
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
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

