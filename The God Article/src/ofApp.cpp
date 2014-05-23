#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	currentSlot = 0;
	waveStart = 0;
	waveLength = 0;
	meshDetail = 0;
    
    bWaveform = true;
    bFreq = true;
    bAmp = true;
    bBreath = true;
    bFingers = true;
    
    loadSettings();
    
    isRecording=false;
    isPlaying=false;
    isLive=true;
    soundStream.listDevices();
    
    meshScale = 1;
    
    soundStream.setDeviceID(config.audio_in);
    soundStream.setup(this, NUM_CHANNELS, NUM_CHANNELS, SAMPLE_RATE, STREAM_BUFFER_SIZE, 4);
    
    waveObject = new ofxWaveHandler(&soundStream, WAVEBUFFER_MINSEC, (ofGetWidth()-30) * 32, 100, ofGetWidth()-30, 75);

    receiver.setup(config.osc_port);
    oscObject = new oscHandler(&receiver, (ofGetWidth()-30) * 32, ofGetHeight() - 330);
    
    ofEnableAlphaBlending();
    
    font.loadFont("OpenSans-Light.ttf", 10);
    
    playPosition = 0;
    updateVisCount();
    
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
    ofBackground(31,33,43);

    // Draw OSC Visualisers
    
    if (view == 0) {
        float gw = (float)ofGetWidth();
        float gwDiv = oscObject->width / waveObject->getBufferLengthSmplsf();
        float pos;
        pos = (playPosition * gwDiv);
        if(isRecording){
            pos = (waveObject->getBufferLengthSmplsf() * gwDiv);
        }
        
        int offset = (int)(((gw * 0.5) - pos));
        ofPushMatrix();
        ofTranslate(offset, 0);
        ofTranslate(0, 25);
        if (bWaveform) {
            if (!isRecording) {
                waveObject->drawWaveMesh();
            }
            ofTranslate(0, singleHeight + 10);
        }
        if (bFreq) {
            ofSetColor(255,255,255,255);
            if (!isRecording) {
                oscObject->spectrogram.draw(0,0);
            }
            ofSetLineWidth(2);
            ofSetColor(255,255,255,150);
            oscObject->freq.draw();
            ofTranslate(0, singleHeight + 10);
        }
        if (bBreath) {
            oscObject->breath.draw();
            ofTranslate(0, singleHeight + 10);
        }
        if (bFingers) {
            oscObject->drawFingers();
        }
        ofPopMatrix();

        if (isLive) {
            ofPushMatrix();
            ofTranslate(ofGetWidth()*0.5, 25);
            if (bWaveform) {
                ofTranslate(0, singleHeight + 10);
            }
            if (bFreq) {
                oscObject->drawFreqLive();
                ofTranslate(0, singleHeight + 10);
            }
            if (bBreath) {
                oscObject->drawBreathLive();
                ofTranslate(0, singleHeight + 10);
            }
            if (bFingers) {
                oscObject->drawFingersLive();
            }
            ofPopMatrix();
        }
        
        ofSetColor(222,222,235, 60);
        ofRect(ofGetWidth() * 0.5, 15, 1, ofGetHeight() - 240);
    }
    
    if (view == 1) {
//        int pos;
//        if(isRecording){
//            pos = oscObject->buffer.size()-1;
//        } else {
//            float div = oscObject->buffer.size() / waveObject->getBufferLengthSmplsf();
//            pos = (playPosition * div);
//        }
//        oscObject->drawCircularBuffer(0, 95, pos);
//        if (isLive) {
//            oscObject->drawCircularLive(0, 95);
//        }
    }

    
	if(isRecording){
		ofSetCircleResolution(50);
		ofSetColor(255,0,0);
		ofCircle(ofGetWidth()/2, singleHeight*0.5 ,12);
        font.drawString(ofToString(waveObject->getBufferLengthSec()),(ofGetWidth()/2) + 20, (singleHeight*0.5) + 5);
	}
    
    // Overlay edges
    ofSetColor(31,33,43);
    ofRect(0, 0, 15, ofGetHeight());
    ofRect(ofGetWidth()-15, 0, 15, ofGetHeight());
    
    // Waveform overview
    ofPushMatrix();
    ofSetLineWidth(1);
    ofTranslate(0, ofGetHeight()-210);
    ofSetColor(0,0,0, 50);
    ofRect(0, 0, ofGetWidth(), 105);
	ofSetColor(222,222,235, 50);
    ofLine(0, 0, ofGetWidth(), 0);
	waveObject->drawOverviewBuffer(15,15);
    
    // Draw waveform overview playhead
    
    ofSetColor(132,132,145, 200);
    float playDiv = ((float)ofGetWidth()-30) / (float)waveObject->getBufferLengthSmpls();
    ofRect((playPosition * playDiv) + 15, 15, 3, 75);
    
    ofPopMatrix();

    // Legend footer
    ofPushMatrix();
    ofSetLineWidth(1);
    ofTranslate(0, ofGetHeight()-105);
    ofSetColor(0,0,0, 50);
    ofRect(0, 0, ofGetWidth(), 110);
	ofSetColor(222,222,235, 50);
    ofLine(0, 0, ofGetWidth(), 0);
	ofSetColor(222,222,235);
    ofTranslate(0, 5);
	font.drawString("P to start/stop playback",20,20);
	font.drawString("SPACE to start/pause recording to the end of the current slot",20,40);
	font.drawString("S to save SLOT: "+ofToString(currentSlot)+" as in a file",20,60);
	font.drawString("C to clear current slot",20,80);
    int x = abs(ofGetWidth()*0.5);
	font.drawString("0-9 to select new slot and load sample if there were any saved",x,20);
    font.drawString("ERTY to switch visualisers on and off",x,40);
	font.drawString("Q-W to select linear or alternative visualisations",x,60);
	font.drawString("L to toggle live data visualisers",x,80);
    ofPopMatrix();

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
    
    bWaveform = configXml.getValue( "VIS::WAVEFORM", 1 );
    bFreq = configXml.getValue( "VIS::FREQ", 1 );
    bBreath = configXml.getValue( "VIS::BREATH", 1 );
    bFingers = configXml.getValue( "VIS::FINGERS", 1 );

    
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
            oscObject->updateMeshes();
            oscObject->updateFFT();
            waveObject->waveFormWidth = oscObject->width;
            waveObject->updateOverviewBuffer();
            waveObject->updateWaveMesh();
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

        updateVisCount();
        playPosition = 0;
	}
	if (key=='c') {
		waveObject->clearBuffer();
        oscObject->clearBuffer();
        playPosition = 0;
	}
	if (key=='p') {
        if(!isRecording) {
            if(isPlaying) {
                isPlaying = false;
            } else {
                isPlaying = true;
            }
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
    if (key==356) { // left arrow
        if (meshScale > 0.1) {
            meshScale-=0.1;
            oscObject->meshScale = meshScale;
            waveObject->meshScale = meshScale;
        }
        updateVisCount();
    }

    if (key==358) { // right arrow
        meshScale+=0.1;
        oscObject->meshScale = meshScale;
        waveObject->meshScale = meshScale;
        updateVisCount();
    }
    
    if (key=='e') {
        bWaveform = !bWaveform;
        updateVisCount();
    }

    if (key=='r') {
        bFreq = !bFreq;
        updateVisCount();
    }

    if (key=='t') {
        bBreath = !bBreath;
        updateVisCount();
    }

    if (key=='y') {
        bFingers = !bFingers;
        updateVisCount();
    }

}

void ofApp::updateVisCount() {
    int totalHeight = ofGetHeight()-240;
    int visCount = 0;
    
    if ( bWaveform ) visCount += 1;
    if ( bFreq ) visCount += 1;
    if ( bBreath ) visCount += 1;
    if ( bFingers ) visCount += 1;
    
    if (visCount > 0) {
        singleHeight = totalHeight / visCount;
        singleHeight -= 10 + ( 10 / visCount);
    } else {
        singleHeight = totalHeight;
    }
    
    
    oscObject->singleHeight = singleHeight;
    waveObject->waveFormHeight = singleHeight;

    oscObject->updateMeshes();
    oscObject->updateFFT();
    waveObject->waveFormWidth = oscObject->width;
    waveObject->updateOverviewBuffer();
    waveObject->updateWaveMesh();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    if (y >= ofGetHeight()-210 && y <= ofGetHeight()-105 && x >= 15 && x <= ofGetWidth()-15) {
        float mult = waveObject->getBufferLengthSmpls() / (ofGetWidth()-30);
        int pos = (int)((x-15) * mult);
        if (pos >= waveObject->getBufferLengthSmpls()-1) {
            pos = waveObject->getBufferLengthSmpls()-1;
        }
        playPosition = pos;
    }

    if (y >= ofGetHeight()-210 && y <= ofGetHeight()-105 && x >= 0 && x <= 15) {
        playPosition = 0;
    }
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if (y >= ofGetHeight()-210 && y <= ofGetHeight()-105 && x >= 15 && x <= ofGetWidth()-15) {
        float mult = waveObject->getBufferLengthSmpls() / (ofGetWidth()-30);
        int pos = (int)((x-15) * mult);
        if (pos >= waveObject->getBufferLengthSmpls()-1) {
            pos = waveObject->getBufferLengthSmpls()-1;
        }
        playPosition = pos;
    }
    
    if (y >= ofGetHeight()-210 && y <= ofGetHeight()-105 && x >= 0 && x <= 15) {
        playPosition = 0;
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    updateVisCount();
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
