#include "ofxWaveHandler.h"

ofxWaveHandler::ofxWaveHandler(ofSoundStream* stream, int width, int height) {

	soundStream = stream;
	recBuffer=NULL;
	recPointer = 0;
	isBlocked = false;

	if (width == 0 || height == 0) {
		waveFormWidth = ofGetWidth();
		waveFormHeight = ofGetHeight();
	}
	else {
		waveFormWidth = width;
		waveFormHeight = height;
	}
	waveForm.allocate(waveFormWidth, waveFormHeight);
}
int ofxWaveHandler::loadBuffer(string fileName, unsigned int startSmpl) {
	if (isBlocked) {
		cout << "!> The buffer is still blocked (loadBuffer)!\n";
		return -1;
	}
	isBlocked = true;
	SndfileHandle	*oFile;
	oFile=new SndfileHandle(ofToDataPath(fileName,true));
	cout << "Opened file: " << fileName << "\n";
	printf ("- Sample rate : %d\n", oFile->samplerate()) ;
	printf ("- Channels    : %d\n", oFile->channels()) ;
	printf ("- Error       : %s\n", oFile->strError());
	printf ("- Frames      : %d\n", int(oFile->frames()));

	// if startSmpl <> 0 then concatenate the file from startSmpl
	recPointer = (startSmpl+oFile->frames())*oFile->channels();
	recBuffer=(float*)realloc(recBuffer, recPointer*sizeof(float));
	oFile->readf(recBuffer+startSmpl, oFile->frames());
	sf_close(oFile->takeOwnership());
	delete oFile;
	isBlocked = false;
	return 0;
}

int ofxWaveHandler::saveBuffer(string fileName, int audioFormat, unsigned int startSmpl, unsigned int endSmpl) {

	if (isBlocked) {
		cout << "!> The buffer is still blocked (saveBuffer)!\n";
		return -1;
	}
	isBlocked = true;

	SndfileHandle	*oFile;
	oFile=new SndfileHandle(ofToDataPath(fileName,true),SFM_WRITE, audioFormat, soundStream->getNumInputChannels() , soundStream->getSampleRate());
	if (!oFile) {
		isBlocked = false;
		cout << "!> SndFileHandl couldn't creat the output file!\n";
		return -1;
	}
	else {
		// calculate and constraint the start and end point of the buffer to write...
		if ((endSmpl*= soundStream->getNumInputChannels())== 0) endSmpl=recPointer;
		else	endSmpl =min((unsigned int)recPointer, endSmpl);
		if ((startSmpl*= soundStream->getNumInputChannels()) >= endSmpl) startSmpl=max((unsigned int)0,endSmpl-soundStream->getNumInputChannels());

		int numSmpl = endSmpl-startSmpl;
		int res=oFile->write(recBuffer+startSmpl, numSmpl);
		if (res < numSmpl) {
			cout << "!> Saving to file had some problems (less data written)!\n" << res << "/" << numSmpl;
		}
		sf_close(oFile->takeOwnership());
		delete oFile;
		isBlocked = false;
	}
	return 0;
}

int ofxWaveHandler::clearBuffer() {

	if (isBlocked) {
		cout << "!> The buffer is still blocked (clearBuffer)!\n";
		return -1;
	}
	isBlocked = true;

	cout << "deleted seconds: " << recPointer/(soundStream->getNumInputChannels()*soundStream->getSampleRate()) << "\n";
	if(recBuffer!=NULL) {
		free(recBuffer);
		recBuffer=NULL;
		recPointer = 0;
	}
	isBlocked = false;
	return 0;
}

void ofxWaveHandler::addSamples(float* input, int numSamples){
	if(!isBlocked){
		recPointer+= numSamples;
		recBuffer=(float*)realloc(recBuffer, recPointer*sizeof(float));
		memcpy(&recBuffer[recPointer-numSamples], input, numSamples*sizeof(float)); 	
	}
}

void ofxWaveHandler::updateWaveBuffer() {
	waveForm.begin();
    ofSetColor(150,150,150);
	ofRect(0, 0, waveFormWidth, waveFormHeight);
	
	if (recBuffer == NULL || isBlocked) {
		waveForm.end();
		if(isBlocked) {
			cout << "ezegyblokkk - ";
		}
		return;
	}
	isBlocked = true;
    ofSetColor(255,255,255);
    
	int channels = soundStream->getNumInputChannels();
    float per = (recPointer/channels) / waveFormWidth;

    for (int i = 0; i < waveFormWidth; i++) {
        float h = ((recBuffer[int(i*per)*channels] * waveFormHeight)*0.5);
        ofRect(i, waveFormHeight/2 - h, 1, h);
	}
    waveForm.end();
	isBlocked = false;
}

void ofxWaveHandler::drawWaveBuffer(float xPos, float yPos) {
	ofSetColor(150,0,0);
	ofRect(xPos-5, yPos-5, waveFormWidth+10, waveFormHeight+10);
	ofSetColor(255);
	waveForm.draw(xPos,yPos);
}

int ofxWaveHandler::getBufferLengthSmpls() {
	return recPointer/soundStream->getNumInputChannels();
}

float ofxWaveHandler::getBufferLengthSec() {
	return float(recPointer)/(soundStream->getNumInputChannels()*soundStream->getSampleRate());
}