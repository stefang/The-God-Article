#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	currentSlot = 0;
	waveStart = 0;
	waveLength = 0;
	meshDetail = 0;
    
    // loadSettings();
    
    isRecording=false;
    isPlaying=false;
    soundStream.listDevices();
    soundStream.setDeviceID(4);
    soundStream.setup(this, NUM_CHANNELS, NUM_CHANNELS, SAMPLE_RATE, STREAM_BUFFER_SIZE, 4);
    waveObject = new ofxWaveHandler(&soundStream, WAVEBUFFER_MINSEC, (ofGetWidth()-30) * 4, 400, ofGetWidth()-30, 75);

    receiver.setup(12345);
    oscObject = new oscHandler(&receiver, (ofGetWidth()-30) * 4, 400);
    
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
	ofDrawBitmapString("PRESS O to start/stop playback...",20,40);
	ofDrawBitmapString("PRESS SPACE to start/pause recording to the end of the current slot...",20,60);
	ofDrawBitmapString("PRESS S to save current slot as in a file...",20,80);
	ofDrawBitmapString("PRESS C to clear current slot...",20,100);
	ofDrawBitmapString("PRESS 0-9 to select new slot and load sample if there were any saved...",20,120);
	ofDrawBitmapString("SLOT: "+ofToString(currentSlot),20,140);
    
	waveObject->drawOverviewBuffer(15,655);
    
    float gw = (float)ofGetWidth();
    float gwDiv = (gw-30)/waveObject->getBufferLengthSmplsf();
    float pos = (playPosition * gwDiv) * 4;
    int offset = (int)(((gw * 0.5) - pos));
    
    waveObject->drawWaveBuffer(offset, 250);
    oscObject->drawOSCBuffer(offset, 250);

    if (isPlaying) {
        ofSetColor(100, 100);
        ofRect(ofGetWidth() * 0.5, 250, 5, 400);
        float playDiv = ((float)ofGetWidth()-30) / (float)waveObject->getBufferLengthSmpls();
        ofRect((playPosition * playDiv) + 15, 655, 3, 75);
    }
    
    ofSetColor(255,255,255,255);

	if(isRecording){
		ofSetCircleResolution(50);
		ofSetColor(255,0,0);
		ofCircle(ofGetWidth()/2,455,30);
	}

}

//void ofApp::loadSettings() {
//    ofxXmlSettings configXml;
//    
//    if ( configXml.loadFile("settings.xml") ) {
//        cout << "Loaded settings.xml" << endl;
//    } else {
//        cout << "Could not load settings.xml" << endl;
//    }
//    
//    config.audio_in = configXml.getValue( "AUDIO::IN", 0 );
//    config.osc_port = configXml.getValue( "OSC::PORT", 7000 );
//    
//}

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
            waveObject->updateWaveBuffer(0, waveObject->getBufferLengthSmpls());
            waveObject->updateOverviewBuffer();
            oscObject->updateOSCBuffer();
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
		oscObject->loadBuffer(dataNameToLoad);
        waveObject->updateWaveBuffer(0, waveObject->getBufferLengthSmpls());
        waveObject->updateOverviewBuffer();
        oscObject->updateOSCBuffer();
        playPosition = 0;
	}
	if (key=='c') {
		waveObject->clearBuffer();
        playPosition = 0;
	}
	if (key=='p') {
        if(!isRecording) {
            oscObject->updateOSCBuffer();
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

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    float mult = waveObject->getBufferLengthSmpls() / (ofGetWidth()-30);
    int pos = (int)((x-15) * mult);
    if (pos >= waveObject->getBufferLengthSmpls()-1) {
        pos = waveObject->getBufferLengthSmpls()-1;
    }
    playPosition = pos;

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
