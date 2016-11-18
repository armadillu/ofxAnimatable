//
//  ofxAnimatableQueue.h
//  BaseApp
//
//  Created by Oriol Ferrer Mesià on 18/11/16.
//
//

#pragma once
#include "ofMain.h"
#include "ofxAnimatableFloat.h"

/*
 Very dumb animation queue composed of ofxAnimatableFloats;
 think of it as a simple timeline building-blocks editor;
 
 HOW IT WORKS
 
 1 - set an initial timeline value:
 	queue.setInitialValue(0);

 2 - add N value transitions to define your timeline:
 	queue.addTransition()
 
 EXAMPLES:
 
 Note how to keep the timeline flat for N seconds, you must add a transition staying in
 the same value for those N seconds; ie to do a "square wave" timeline like the one below,
 you would:
 
         _________
         |       |
 ________|       |________
   2s        2s      2s

 float phaseDuration = 2;
 
 queue.setInitialValue(0);
 queue.addTransition(0,phaseDuration,LINEAR);
 queue.addTransition(1,0,LINEAR);
 queue.addTransition(1,phaseDuration,LINEAR);
 queue.addTransition(0,0,LINEAR);
 queue.addTransition(0,phaseDuration,LINEAR);
 
 NOTES
	
 Because of the simple design chosen, there are some inherent limitations

 1 - transitions only define a target value, the timeline will animate from the last value it was
     left at (so transitions are incremental - you only know where you are going, not where you come from). 
     That's why the 1st value must be sat manually with setInitialValue();
 2 - Looping is entirely up to you - there's an event triggered when its done for your convenience.
 3 - You can't change the playhead (ie scrub the timeline) - only restart it
 4 - to make Jump-cut kind transitions, just make them duration 0

 */

class ofxAnimatableQueue{

public:
	
	ofxAnimatableQueue();

	struct EventArg{
		ofxAnimatableQueue* who;
	};

	void setInitialValue(float val); //initial value of the timeline
	void addTransition(float targetValue, float duration, AnimCurve curve);
	void clearQueue(); //removes all transitions

	float getCurrentValue();

	void startPlaying(); //resets playhead to 0
	void pausePlayback(); //just stops updating
	void resumePlayback(); //just resumes updating
	bool isPlaying(){return playing;}


	void update(float dt);

	ofEvent<ofxAnimatableQueue::EventArg> eventQueueDone;

protected:

	struct AnimationPhase{
		float targetValue;
		float duration;
		AnimCurve curve;
	};

	vector<AnimationPhase> animSteps;
	int currentStep = 0;
	bool playing = false;

	ofxAnimatableFloat anim;

};

