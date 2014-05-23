//
//  oschandler.h
//  The God Article
//
//  Created by Stefan Goodchild on 12/05/2014.
//
//

#ifndef __The_God_Article__oschandler__
#define __The_God_Article__oschandler__

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class oscHandler
{
    public :
    oscHandler(ofxOscReceiver* receiver=NULL, int width=0, int height=0);
    ~oscHandler();
    
    void update(bool record);
    void setSlot(int slot);

    void updateMeshes();
    void updateFFT();
    void drawOSCLive(int x = 0, int y = 0);
    void drawOSCBuffer(int x = 0, int y = 0);
    void drawCircularLive(int x = 0, int y = 0);
    void drawCircularBuffer(int x = 0, int y = 0, int pos = 0);
    void drawFingers();

    void clearBuffer();
    int loadBuffer(string fileName);
    int saveBuffer(string fileName);
    
    int currentSlot;
    
    int width, height, speed;
    
    float singleHeight;
    
    ofxOscReceiver* receiver;
    vector<float> values;
    vector<float> fft;
    vector<float> adjustments;
    vector< vector<float> > buffer;
    vector< vector<float> > fftbuffer;
    
    ofMesh amp, breath;
    ofPolyline freq;
    
    ofImage spectrogram;
    
    vector<ofMesh> fingers;
    
    float meshScale;
    
};

#endif /* defined(__The_God_Article__oschandler__) */
