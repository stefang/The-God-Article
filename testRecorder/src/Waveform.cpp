//
//  Waveform.cpp
//  testRecorder
//
//  Created by Stefan Goodchild on 06/05/2014.
//
//

#include "Waveform.h"

Waveform::Waveform() {
}

void Waveform::setup(int w, int h) {
    width = w;
    height = h;
    wave.allocate(w, h);
}

void Waveform::load(string f) {
    
//    mutex.lock();
    
    source = SndfileHandle(ofToDataPath(f));
    
    cout << "Opened file: " << f << endl;
    
    printf ("    Sample rate : %d\n", source.samplerate ()) ;
    printf ("    Channels    : %d\n", source.channels ()) ;
    printf ("    Error       : %s\n", source.strError());
    printf ("    Frames      : %d\n", int(source.frames())); // frames is essentially samples
    
    puts("");
    
    wave.allocate(width-10, height-10);
    
    wave.begin();
    ofSetColor(150,150,150);
    ofRect(0, 0, width-10, height-10);
    ofSetColor(255,255,255);
    
    float per = source.frames() / width*1.01;
    
    for (int i = 0; i < source.frames(); i++) {
        source.readf(&data, per);
        float h = ((data * height)*0.5);
        ofRect(i, height/2 - h, 1, h);
    }
    wave.end();
    
//    mutex.unlock();

    
}

void Waveform::draw() {
    ofSetColor(50,50,50);
    ofRect(15, 250, width, height);
    if (source.frames() > 0) {
        ofSetColor(255);
        wave.draw(ofPoint(20,255));
        ofSetColor(255,0,0);
        ofRect(20 + ((width-10)*pos), 255, 3, height-10);
    }
}

void Waveform::update(float p) {
    pos = p;
}