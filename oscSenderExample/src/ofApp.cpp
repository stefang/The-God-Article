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
    
    string letters = "abcdef";
    
    for (int i = 0; i < 6; i++) {
        values[i] = sin(((i+1) * 0.02) * (float)ofGetFrameNum());
        ofxOscMessage m;
        string address = "/value/";
        address += letters[i];
        m.setAddress(address);
        m.addFloatArg(values[i]);
        sender.sendMessage(m);
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
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

