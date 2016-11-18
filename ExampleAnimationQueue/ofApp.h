#pragma once

#include "ofMain.h"
#include "ofxAnimatableQueue.h"
#include "ofxHistoryPlot.h"


class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

	ofxAnimatableQueue queue;
	ofxHistoryPlot * plot;

	void onAnimQueueDone(ofxAnimatableQueue::EventArg&);

};
