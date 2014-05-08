#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0,0,0);
	recording=false;
	actSlot = 0;

	soundStream.listDevices();
	
	// you can change the deviceID to 0 if you only have one soundcard... 
	// I have to use 1 because 0 is the integrated soundboard and 1 is my USB card... 
	// Check the console for the deviceIDs...
	soundStream.setDeviceID(1);


	soundStream.setup(this, 0, NUM_CHANNELS, SAMPLE_RATE, BUFFER_SIZE, 4);

	waveObject = new ofxWaveHandler(&soundStream, ofGetWidth()-30, 400);
}

void testApp::exit(){
	soundStream.close();
	delete waveObject;
}

//--------------------------------------------------------------
void testApp::update(){
	waveObject->updateWaveBuffer();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(255,255, 255);
	ofSetColor(20,20,20);
	ofDrawBitmapString("PRESS SPACE to start/pause recording to the end of the actual slot...",20,20);
	ofDrawBitmapString("PRESS S to save actual slot as in a file...",20,40);
	ofDrawBitmapString("PRESS C to clear actual slot...",20,60);
	ofDrawBitmapString("PRESS 0-9 to select new slot and load sample if there were any saved...",20,80);

	waveObject->drawWaveBuffer(15,255);

	ofDrawBitmapString("SLOT: "+ofToString(actSlot),25,275);
	ofDrawBitmapString("Current buffer size in samples... "+ofToString(waveObject->getBufferLengthSmpls()),25,620);
	ofDrawBitmapString("Current buffer size in seconds... "+ofToString(waveObject->getBufferLengthSec(),2),25,640);

	ofSetColor(20,20,20);
	if(recording){
		ofSetCircleResolution(50);
		ofDrawBitmapString("RECORDING = TRUE",20,100);
		ofSetColor(255,0,0);
		ofCircle(45,180,30);
	} else {
		ofDrawBitmapString("RECORDING = FALSE",20,100);
	}	
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	if (key=='s') {
		string currentRecording=ofToString(actSlot,2,'0')+".wav";
		cout<<"Saving: " << currentRecording << "\n";
		/* FORMAT EXAMPLES:  
		SF_FORMAT_AIFF | SF_FORMAT_PCM_16;
		SF_FORMAT_RAW | SF_FORMAT_PCM_16;
		SF_FORMAT_AU | SF_FORMAT_FLOAT;
		(see sndfile.h for more information) */
		waveObject->saveBuffer(currentRecording, SF_FORMAT_WAV | SF_FORMAT_PCM_16, 0, 0);
	}
	if (key==' '){
		if(recording) {
			cout<<"Stop recording...\n";
			recording = false;
		}
		else {
			cout<<"Start recording...\n";
			recording = true;
		}

	}
	if (key>='0' && key <= '9') {
		actSlot = key-'0';
		string fileNameToLoad="0"+ofToString(actSlot)+string(".wav");
		waveObject->loadBuffer(fileNameToLoad);
	}
	if (key=='c') {
		waveObject->clearBuffer();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void testApp::audioIn(float* input, int bufferSize, int nChannels){
	if(recording) waveObject->addSamples(input,bufferSize*nChannels);
}
