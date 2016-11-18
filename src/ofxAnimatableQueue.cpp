//
//  ofxAnimatableQueue.cpp
//  BaseApp
//
//  Created by Oriol Ferrer Mesià on 18/11/16.
//
//

#include "ofxAnimatableQueue.h"

ofxAnimatableQueue::ofxAnimatableQueue(){

}

void ofxAnimatableQueue::setInitialValue(float val){
	anim.reset(val);
}


void ofxAnimatableQueue::addTransition(float targetValue, float duration, AnimCurve curve){
	AnimationPhase phase;
	phase.targetValue = targetValue;
	phase.duration = duration;
	phase.curve = curve;
	animSteps.push_back(phase);
}


void ofxAnimatableQueue::clearQueue(){
	animSteps.clear();
}


void ofxAnimatableQueue::update(float dt){

	if(playing){

		anim.update(dt);

		if (anim.hasFinishedAnimating()){
			if(currentStep < animSteps.size()){
				AnimationPhase newPhase = animSteps[currentStep];
				//animSteps.erase(animSteps.begin());

				if(newPhase.duration <= 0.0f){
					anim.reset(newPhase.targetValue);
				}else{
					anim.setCurve(newPhase.curve);
					anim.setDuration(newPhase.duration);
					anim.animateTo(newPhase.targetValue);
				}
				currentStep++;
			}else{
				playing = false;
				EventArg arg = {this};
				ofNotifyEvent(eventQueueDone, arg);
			}
		}
	}
}


float ofxAnimatableQueue::getCurrentValue(){
	return anim.val();
}

void ofxAnimatableQueue::startPlaying(){
	currentStep = 0;
	playing = true;
}

void ofxAnimatableQueue::pausePlayback(){
	playing = false;
}

void ofxAnimatableQueue::resumePlayback(){
	playing = true;
}
