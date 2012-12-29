/**
*  ofxAnimatableFloat.cpp
*  
* 
*  Created by Oriol Ferrer Mesia on 2008/10/09.
*
*/

#include "ofxAnimatableFloat.h"

ofxAnimatableFloat::ofxAnimatableFloat(){

	ofxAnimatable::setup();
	originalVal_ = targetVal_= 0.0f;
}


void ofxAnimatableFloat::update( float dt ){
	
	ofxAnimatable::update( dt );
}


void ofxAnimatableFloat::animateTo( float newVal ){

	originalVal_ = val();
	targetVal_ = newVal;
	
	ofxAnimatable::startAnimation();	
}


void ofxAnimatableFloat::animateToAfterDelay( float newVal, float delay ){
	
	if (delay <= 0.0f){
		animateTo(newVal);
	}else{
		//originalTempVal_ = val();
		targetTempVal_ = newVal;
		ofxAnimatable::startAnimationAfterDelay(delay);
	}
}


void ofxAnimatableFloat::animateFromTo( float originalValue, float destinationValue ){

	ofxAnimatable::startAnimation();
	
	originalVal_ = originalValue;
	targetVal_ = destinationValue;
}


void ofxAnimatableFloat::animateToIfFinished( float newVal ){
	if ( animating_ == false ){
		animateTo(newVal);
	}
}


float ofxAnimatableFloat::val(){
	
	float mappedDistribution = calcCurveAt( percentDone_ );	///percentDone_ is [0..1] & tells me where we are between orig and target
	return originalVal_ + ( targetVal_ - originalVal_ ) * mappedDistribution ;
}


void ofxAnimatableFloat::reset( float newVal ){

	ofxAnimatable::reset();
	originalVal_ = targetVal_ = newVal; 
}


void ofxAnimatableFloat::reset(){

	ofxAnimatable::reset();
	targetVal_ = originalVal_;
}


void ofxAnimatableFloat::startAfterWait(){
	animateTo(targetTempVal_);
}

