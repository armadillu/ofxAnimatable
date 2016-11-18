#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(0);
	ofSetFrameRate(60);


	//create a plot to draw the timeline
	plot = new ofxHistoryPlot(NULL, "timeline", ofGetWidth(), false);
	plot->setBackgroundColor(ofColor(16));
	plot->setShowNumericalInfo(true);
	plot->setRange(0, 1);
	plot->setRespectBorders(true);
	plot->setLineWidth(1);
	plot->setCropToRect(true);
	plot->setDrawGrid(true);
	plot->setGridUnit(16);
	plot->setGridColor(ofColor(22,255));
	plot->setShowSmoothedCurve(false);

	//create the animation
	float phaseDuration = 1.0;
	queue.setInitialValue(0); //timeline starts at 0

	//square wave
	queue.addTransition(0,phaseDuration,LINEAR); //sustain 0
	queue.addTransition(1,0,LINEAR); //hard cut transition to 1
	queue.addTransition(1,phaseDuration,LINEAR); //sustain 1
	queue.addTransition(0,0,LINEAR); //hard cut transition to 0
	queue.addTransition(0,phaseDuration,LINEAR); //sustain 0

	//sin-like wave
	queue.addTransition(1,phaseDuration,TANH); //TANH to 1 (from 0)
	queue.addTransition(1,phaseDuration,LINEAR); //sustain 1
	queue.addTransition(0,phaseDuration,TANH); //TANH to 0 (from 1)
	queue.addTransition(0,phaseDuration,LINEAR); //sustain 0

	//we want to know when the animation ends
	ofAddListener(queue.eventQueueDone, this, &ofApp::onAnimQueueDone);

	queue.startPlaying(); //start the animation
}

void ofApp::onAnimQueueDone(ofxAnimatableQueue::EventArg&){
	queue.startPlaying(); //loop the animation endlessly
}

void ofApp::update(){

	float dt = 1./60.;
	queue.update(dt);

	float val = queue.getCurrentValue();
	plot->update(val);
}

void ofApp::draw(){
	plot->draw(0, 100, ofGetWidth(), 100);
}

