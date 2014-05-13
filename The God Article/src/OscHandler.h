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
    void drawOSCBuffer(int x = 0, int y = 0);
    void drawCircularBuffer(int x = 0, int y = 0, int pos = 0);

    void clearBuffer();
    int loadBuffer(string fileName);
    int saveBuffer(string fileName);
    
    int currentSlot;
    
    int width, height, speed;
    
    ofxOscReceiver* receiver;
    vector<float> values;
    vector<float> adjustments;
    vector< vector<float> > buffer;
    
    ofMesh ampFreq, breath;
    vector<ofMesh> fingers;
    
};

#endif /* defined(__The_God_Article__oschandler__) */
