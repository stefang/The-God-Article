#pragma once

#include "ofMain.h"
#include "ofxLibsndFileRecorder.h"
#include "Waveform.h"

#define NUM_CHANNELS 1
#define SAMPLE_RATE 44100
#define BUFFER_SIZE 1024

class ofApp : public ofBaseApp{

	public:
    void setup();
    void update();
    void draw();
    void keyPressed  (int key);
    void audioReceived(float * input, int bufferSize, int nChannels);
    bool recording;
    
    string currentRecording, lastRecording;

    ofxLibsndFileRecorder recorder;
    
    ofSoundPlayer  playback;

    Waveform waveformDisplay;
    
    float *data;
    
    ofMutex mutex;

};
