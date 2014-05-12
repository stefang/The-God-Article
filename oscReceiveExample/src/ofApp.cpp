#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	receiver.setup(12345);

    for (int i = 0; i < 8; i++) {
        values.push_back(0.0f);
        adjustments.push_back(0.0f);
        ofPolyline cur;
        lines.push_back(cur);
    }
    
    pos = ofVec2f(50, 20);
    
    ofSetLineWidth(2);

}

//--------------------------------------------------------------
void ofApp::update(){

    if (playing) {
        if (position < record.size()-1) {
            position++;
            values = record[position];
            history.push_back(record[position]);
        }
    } else {
        // check for waiting messages
        while(receiver.hasWaitingMessages()){
            // get the next message
            ofxOscMessage m;
            receiver.getNextMessage(&m);
            
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
        
        history.push_back(values);
        
        if (recording) {
            record.push_back(values);
        }
        
    }

    if (history.size() > 300) {
        history.erase(history.begin());
    }

    for (int i = 0; i < values.size(); i++) {
        lines[i].clear();
        for (int h = 0; h < history.size(); h++) {
            lines[i].addVertex(ofPoint(h*4, history[h][i] * 40));
        }
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255,255,255);
    ofPushMatrix();
    ofTranslate(pos);
    if (playing) {
        for (int i = 0; i < values.size(); i++) {
            ofTranslate(0,100);
            ofRect(0, values[i] * 40, 5, 5);
            lines[i].draw();
        }
    } else {
        for (int i = 0; i < values.size(); i++) {
            ofTranslate(0,100);
            ofRect(0, values[i] * 40, 5, 5);
            lines[i].draw();
        }
    }
    ofPopMatrix();
	ofDrawBitmapString("PRESS a to start recording",20,20);
	ofDrawBitmapString("PRESS s to stop recording",20,40);
	ofDrawBitmapString("PRESS d to restart playback",20,60);
	ofDrawBitmapString("PRESS f to view live data",20,80);
	ofDrawBitmapString("PRESS g to load last data",20,100);
    if(recording){
		ofSetCircleResolution(50);
		ofDrawBitmapString("RECORDING = TRUE",20,120);
		ofSetColor(255,0,0);
		ofCircle(165,76,6);
	} else {
		ofDrawBitmapString("RECORDING = FALSE",20,120);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key=='a' && !recording){
		cout<<"Start recording\n";
        history.clear();
        record.clear();
		recording=true;
        playing=false;
	}
	if (key=='s' && recording){
		cout<<"Stop recording\n";
		recording=false;
        cout << record.size() << endl;
        cout<<"Saving recording file\n";
        filename = "data-";
        filename += ofToString(ofGetSystemTimeMicros());
        filename += ".txt";
        cout << "record::size " << record.size() << endl;
        ofFile newFile(ofToDataPath(filename), ofFile::WriteOnly);
        newFile.create();
        for (int h = 0; h < record.size(); h++) {
            for (int i = 0; i < values.size(); i++) {
                lines[i].clear();
                newFile << history[h][i];
                if (h < record.size()-1 ) {
                    newFile << ",";
                }
            }
            newFile << endl;
        }
        newFile.close();
		playing=true;
        position = 0;
        history.clear();
		cout<<"Playback\n";
	}
    if (key=='d' && !recording && playing) {
		cout<<"Reset Playback\n";
        position = 0;
        history.clear();
    }
    if (key=='f' && playing) {
        history.clear();
		recording=false;
		playing=false;
    }
    if (key=='g' && !recording) {
        history.clear();
//        record.clear();
//        string fileContents = ofBufferFromFile(ofToDataPath(filename));
//        vector<string> lines = ofSplitString(fileContents, "\n");
//        for (int l = 0; l < lines.size(); l++) {
//            vector<string> vals = ofSplitString(lines[l], ",");
//            vector<float> valsf;
//            for (int v = 0; v < vals.size(); v++) {
//                valsf.push_back(ofToFloat(vals[v]));
//            }
//            record.push_back(valsf);
//        }
        position = 0;
        recording=false;
        playing=true;
    }
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
