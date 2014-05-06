#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0,0,0);
	recording=false;
	ofSoundStreamSetup(0,NUM_CHANNELS,this, SAMPLE_RATE,BUFFER_SIZE,4);
    playback.setMultiPlay(false);
    waveformDisplay.setup(ofGetWidth() - 30, 400);
}

//--------------------------------------------------------------
void ofApp::update(){
    if (playback.getIsPlaying()) {
        waveformDisplay.update(playback.getPosition());
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255,255, 255);
	ofSetColor(20,20,20);
	ofDrawBitmapString("PRESS a to start recording",20,20);
	ofDrawBitmapString("PRESS s to stop recording",20,40);
	ofDrawBitmapString("PRESS d to restart playback",20,60);
	if(recording){
		ofSetCircleResolution(50);
		ofDrawBitmapString("RECORDING = TRUE",20,80);
		ofSetColor(255,0,0);
		ofCircle(45,160,30);
	} else {
		ofDrawBitmapString("RECORDING = FALSE",20,80);
    }
    
    waveformDisplay.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	static int audioCount=0;
	if (key=='a' && !recording){
		cout<<"Start recording\n";
        playback.stop();
        playback.unloadSound();
		currentRecording=ofToString(audioCount,0)+".wav";
		cout << currentRecording<<"----\n";
		recorder.setup(currentRecording);
		recorder.setFormat(SF_FORMAT_WAV | SF_FORMAT_PCM_16);
		recording=true;
		audioCount++;
	}
	if (key=='s' && recording){
		cout<<"Stop recording\n";
		recording=false;
		recorder.finalize();
        recorder.close();
        waveformDisplay.load(currentRecording);
        playback.loadSound(currentRecording);
        playback.play();
	}
    if (key=='d' && !recording) {
        playback.setPosition(0);
        if (!playback.getIsPlaying()) {
            playback.play();
        }
    }
}

//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannels){
	if(recording)
		recorder.addSamples(input,bufferSize*nChannels);
}
