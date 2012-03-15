/*
 *  ofxAnimatable.cpp
 *  Cocoa Test
 *
 *  Created by Oriol Ferrer Mesià on 30/10/09.
 *  Copyright 2009 uri.cat. All rights reserved.
 *
 */

#include "ofxAnimatable.h"
#include <Math.h>

void ofxAnimatable::setup(){


	transitionSpeed_ = 1.0f / DEFAULT_ANIMATION_DURATION;
	percentDone_ = 0.0f;
	animating_ = false;
	paused_ = false;
	playcount_ = 0;
	repeat_ = PLAY_ONCE;
	curveStyle_ = EASE_IN_EASE_OUT;
}


float ofxAnimatable::calcCurveAt( float percent ){

	float r = percent;
	
	switch ( curveStyle_ ) {
			
		case EASE_IN_EASE_OUT:
			r = 0.5f - 0.5f * cosf( M_PI * percent ); break;
		
		case EASE_IN:
			r = 1.0f + sinf( M_PI_2 * percent - M_PI_2); break;

		case EASE_OUT:	
			r = sinf( M_PI_2 * percent ); break;

		case LINEAR:
			break;
			
		case TANH:
			r = + 0.5f + 0.5f * tanh( 2.0f * M_PI * percent - M_PI ) * 1.0037417f; break;

		case SINH:
			r = 0.5f + 0.23482f * sinh( 3.0f * percent - 1.5f ); break;

		case BOUNCY:{
			float k = 0.5;
			r = 0.5f - 0.51f * cosf( M_PI * percent + k*percent - k * 0.5f ); break;
		}
	}
	
	return r;
}


void ofxAnimatable::update(float dt){

	if (delay_ > 0.0f ){
		delay_ -= dt;
		if (delay_ < 0.0f){
			startAfterWait();
		}
	}

	if (animating_ == true && paused_ == false){
		
		percentDone_ += transitionSpeed_ * dt;
		
		if (percentDone_ >= 1.0f){

			animating_ = false;
			percentDone_ = 1.0f;
						
			switch (repeat_) {

				case PLAY_ONCE:	
					break;	//nothing to do;

				case LOOP_BACK_AND_FORTH_SWAP_CURVE:
					if ( curveStyle_ == EASE_IN)
						curveStyle_ = EASE_OUT;
					else if ( curveStyle_ == EASE_OUT)
							curveStyle_ = EASE_IN;					
					
				case LOOP_BACK_AND_FORTH:
					swapOriginDestination();	//this will tell our subclass to swap values so we can loop back
					
				case LOOP:	
					animating_ = true;
					percentDone_ = 0.0f;							
					break;

				
				case LOOP_BACK_AND_FORTH_ONCE_SWAP_CURVE:
					if ( curveStyle_ == EASE_IN)
						curveStyle_ = EASE_OUT;
					else if ( curveStyle_ == EASE_OUT)
							curveStyle_ = EASE_IN;					

				case LOOP_BACK_AND_FORTH_ONCE:
					if (playcount_ >= 1){	//time to stop
						//we are done
					}else{	
						animating_ = true;
						percentDone_ = 0.0f;
						playcount_++;
						swapOriginDestination();	//this will tell our subclass to swap values so we can loop back
					}
					break;
			}
		}
	}
}


void ofxAnimatable::startAnimation(){
	percentDone_ = 0.0f;
	delay_ = 0.0f;
	animating_ = true;
	playcount_ = 0;
	paused_ = false;
}

void ofxAnimatable::startAnimationAfterDelay(float delay){
	delay_ = delay;
	//animating_ = false;
}


void ofxAnimatable::reset(){
	percentDone_ = 0.0f;
	delay_ = 0.0f;
	animating_ = false;
	playcount_ = 0;
	paused_ = false;
}


void ofxAnimatable::setDuration( float seconds ){
	transitionSpeed_ = 1.0f / seconds;
}


void ofxAnimatable::setRepeatType( animRepeat repeat ){
	repeat_ = repeat;
}


void ofxAnimatable::setCurve( animCurve curveStyle){
	curveStyle_ = curveStyle;
}


float ofxAnimatable::getPercentDone(){ 
	return percentDone_; 
}		


bool ofxAnimatable::isAnimating(){ 
	return animating_;
}


bool ofxAnimatable::hasFinishedAnimating(){ 
	return !animating_;
}

bool ofxAnimatable::isWaitingForAnimationToStart(){ 
	return ( delay_ > 0 );
}

bool ofxAnimatable::isOrWillBeAnimating(){ 
	return isAnimating() || isWaitingForAnimationToStart() ;
}

void ofxAnimatable::pause(){
	paused_ = true;
}


void ofxAnimatable::resume(){
	paused_ = false;
}
