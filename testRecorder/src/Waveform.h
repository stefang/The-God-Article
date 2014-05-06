//
//  Waveform.h
//  testRecorder
//
//  Created by Stefan Goodchild on 06/05/2014.
//
//

#include "sndfile.hh"
#include "ofMain.h"

#ifndef waveform
#define waveform

#pragma once


class Waveform
{
    
    public :
    Waveform();
    
    void setup(int w, int h);
    void load(string f);
    void update(float p);
    void draw();
    
    int width, height;
    
    SndfileHandle source;
    ofFbo wave;
    
    float pos = 0;
    
    float data;
    
    ofMutex mutex;
    
};

#endif