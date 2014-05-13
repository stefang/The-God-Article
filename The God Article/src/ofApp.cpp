#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	currentSlot = 0;
	waveStart = 0;
	waveLength = 0;
	meshDetail = 0;
    
    loadSettings();
    
    isRecording=false;
    isPlaying=false;
    soundStream.listDevices();
    
    soundStream.setDeviceID(config.audio_in);
    soundStream.setup(this, NUM_CHANNELS, NUM_CHANNELS, SAMPLE_RATE, STREAM_BUFFER_SIZE, 4);
    waveObject = new ofxWaveHandler(&soundStream, WAVEBUFFER_MINSEC, (ofGetWidth()-30) * 4, 695, ofGetWidth()-30, 75);

    receiver.setup(config.osc_port);
    oscObject = new oscHandler(&receiver, (ofGetWidth()-30) * 32, 750);
    
    ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::update(){
    if (isRecording) {
        waveObject->updateOverviewBuffer();
    }
    oscObject->update(isRecording);
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255,255, 255);
	ofSetColor(20,20,20, 255);
	ofDrawBitmapString("PRESS P to play back the current slot from 0...",20,20);
    ofDrawBitmapString("PRESS O to start/stop playback...",620,20);
	ofDrawBitmapString("PRESS SPACE to start/pause recording to the end of the current slot...",20,40);
	ofDrawBitmapString("PRESS S to save current slot as in a file...",620,40);
	ofDrawBitmapString("PRESS C to clear current slot...",20,60);
	ofDrawBitmapString("PRESS 0-9 to select new slot and load sample if there were any saved...",620,60);
	ofDrawBitmapString("SLOT: "+ofToString(currentSlot),20,80);
	ofDrawBitmapString("PRESS Q-W to select linear or circular visualisations...",620,80);
    
	waveObject->drawOverviewBuffer(15,860);
    
    if (view == 0) {
        float gw = (float)ofGetWidth();
        float gwDiv = oscObject->width / waveObject->getBufferLengthSmplsf();
        float pos;
        if(isRecording){
            pos = (waveObject->getBufferLengthSmplsf() * gwDiv);
        }
        if(isPlaying){
            pos = (playPosition * gwDiv);
        }
        int offset = (int)(((gw * 0.5) - pos));
        oscObject->drawOSCBuffer(offset, 95);
        ofSetColor(100, 90);
        ofRect(ofGetWidth() * 0.5, 95, 3, 750);
    }
    
    if (view == 1) {
        int pos;
        if(isRecording){
            pos = oscObject->buffer.size()-1;
        }
        if(isPlaying){
            float div = oscObject->buffer.size() / waveObject->getBufferLengthSmplsf();
            pos = (playPosition * div);
        }
        oscObject->drawCircularBuffer(0, 95, pos);
    }

    ofSetColor(100, 100);
    float playDiv = ((float)ofGetWidth()-30) / (float)waveObject->getBufferLengthSmpls();
    ofRect((playPosition * playDiv) + 15, 860, 3, 75);
    
	if(isRecording){
		ofSetCircleResolution(50);
		ofSetColor(255,0,0);
		ofCircle(ofGetWidth()/2,455,30);
        ofDrawBitmapString(ofToString(waveObject->getBufferLengthSec()),ofGetWidth() * 0.5, 515);
	}
    
    ofSetColor(255,255,255,255);

    ofRect(0, 0, 15, ofGetHeight());
    ofRect(ofGetWidth()-15, 0, 15, ofGetHeight());
}

void ofApp::loadSettings() {
    ofxXmlSettings configXml;
    
    if ( configXml.loadFile("appConfig.xml") ) {
        cout << "Loaded appConfig.xml" << endl;
    } else {
        cout << "Could not load appConfig.xml" << endl;
    }
    
    config.audio_in = configXml.getValue( "AUDIO::IN", 0 );
    config.osc_port = configXml.getValue( "OSC::PORT", 7000 );
    
}

void ofApp::audioIn(float* input, int bufferSize, int nChannels){
	if(isRecording) {
        waveObject->addSamples(input, bufferSize*nChannels);
    }
}

//--------------------------------------------------------------
void ofApp::audioOut (float* output, int bufferSize, int nChannels){
    if (isPlaying) {
        for (int i = 0; i < bufferSize; i++){
            output[i*nChannels    ] = waveObject->getSample(playPosition);
            playPosition++;
            if (playPosition > waveObject->getBufferLengthSmpls()) {
                playPosition = 0;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key=='s') {
		string currentRecording=ofToString(currentSlot,2,'0')+".wav";
		string currentDatafile=ofToString(currentSlot,2,'0')+".txt";
		cout<<"Saving: " << currentRecording << "\n";
		waveObject->saveBuffer(currentRecording, SF_FORMAT_WAV | SF_FORMAT_PCM_16, 0, 0);
		oscObject->saveBuffer(currentDatafile);
	}
	if (key==' '){
		if(isRecording) {
			cout<<"Stop recording...\n";
			isRecording = false;
            waveObject->updateOverviewBuffer();
            oscObject->updateMeshes();
		}
		else {
			cout<<"Start recording...\n";
			isRecording = true;
            isPlaying = false;
		}
	}
	if (key>='0' && key <= '9') {
        isRecording = false;
        isPlaying = false;
		currentSlot = key-'0';
		string fileNameToLoad="0"+ofToString(currentSlot)+string(".wav");
		string dataNameToLoad="0"+ofToString(currentSlot)+string(".txt");
		waveObject->loadBuffer(fileNameToLoad);
        waveObject->updateOverviewBuffer();
		oscObject->loadBuffer(dataNameToLoad);
        oscObject->updateMeshes();
        playPosition = 0;
	}
	if (key=='c') {
		waveObject->clearBuffer();
        oscObject->clearBuffer();
        playPosition = 0;
	}
	if (key=='p') {
        if(!isRecording) {
            oscObject->updateMeshes();
            playPosition = 0;
            isPlaying = true;
        }
	}
	if (key=='o') {
        if(isPlaying) {
            isPlaying = false;
        } else {
            isPlaying = true;
        }
	}
    if (key=='q') {
        view = 0;
    }
    if (key=='w') {
        view = 1;
    }
    if (key=='l') {
        isLive = !isLive;
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if (y >= 860 && x >= 15 && x <= ofGetWidth()-15) {
        float mult = waveObject->getBufferLengthSmpls() / (ofGetWidth()-30);
        int pos = (int)((x-15) * mult);
        if (pos >= waveObject->getBufferLengthSmpls()-1) {
            pos = waveObject->getBufferLengthSmpls()-1;
        }
        playPosition = pos;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if (y >= 860 && x >= 15 && x <= ofGetWidth()-15) {
        float mult = waveObject->getBufferLengthSmpls() / (ofGetWidth()-30);
        int pos = (int)((x-15) * mult);
        if (pos >= waveObject->getBufferLengthSmpls()-1) {
            pos = waveObject->getBufferLengthSmpls()-1;
        }
        playPosition = pos;
    }
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

//--------------------------------------------------------------
void ofApp::exit(){
    soundStream.close();
	delete waveObject;
}
