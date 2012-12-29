/*
 *  ofxAnimatable.cpp
 *  Cocoa Test
 *
 *  Created by Oriol Ferrer Mesià on 30/10/09.
 *  Copyright 2009 uri.cat. All rights reserved.
 *
 */

#include "ofxAnimatable.h"

//from http://www.flong.com/texts/code/shapers_exp/

float doubleExponentialSeat (float x, float a){

	float epsilon = 0.00001;
	float min_param_a = 0.0 + epsilon;
	float max_param_a = 1.0 - epsilon;
	a = min(max_param_a, max(min_param_a, a));

	float y = 0;
	if (x<=0.5){
		y = (pow(2.0*x, 1-a))/2.0;
	} else {
		y = 1.0 - (pow(2.0*(1.0-x), 1-a))/2.0;
	}
	return y;
}

float doubleExponentialSigmoid (float x, float a){

	float epsilon = 0.00001;
	float min_param_a = 0.0 + epsilon;
	float max_param_a = 1.0 - epsilon;
	a = min(max_param_a, max(min_param_a, a));
	a = 1.0-a; // for sensible results

	float y = 0;
	if (x<=0.5){
		y = (pow(2.0*x, 1.0/a))/2.0;
	} else {
		y = 1.0 - (pow(2.0*(1.0-x), 1.0/a))/2.0;
	}
	return y;
}

float exponentialEasing (float x, float a){

	float epsilon = 0.00001;
	float min_param_a = 0.0f + epsilon;
	float max_param_a = 1.0f - epsilon;
	a = max(min_param_a, min(max_param_a, a));

	if (a < 0.5f){
		// emphasis
		a = 2.0f*(a);
		float y = pow(x, a);
		return y;
	} else {
		// de-emphasis
		a = 2.0f*(a-0.5f);
		float y = pow(x, 1.0/(1-a));
		return y;
	}
}


float quadraticBezier (float x, float a, float b){
	// adapted from BEZMATH.PS (1993)
	// by Don Lancaster, SYNERGETICS Inc.
	// http://www.tinaja.com/text/bezmath.html

	float epsilon = 0.00001;
	a = max(0.0f, min(1.0f, a));
	b = max(0.0f, min(1.0f, b));
	if (a == 0.5f){
		a += epsilon;
	}

	// solve t from x (an inverse operation)
	float om2a = 1 - 2*a;
	float t = (sqrt(a*a + om2a*x) - a)/om2a;
	float y = (1-2*b)*(t*t) + (2*b)*t;
	return y;
}


std::string ofxAnimatable::getCurveName(AnimCurve c){
	
	switch (c) {
		case EASE_IN_EASE_OUT: return "EASE_IN_EASE_OUT";
		case EASE_IN: return "EASE_IN";
		case EASE_OUT: return "EASE_OUT";
		case LINEAR: return "LINEAR";
		case BOUNCY: return "BOUNCY";
		case TANH: return "TANH";
		case SINH: return "SINH";
		case SQUARE: return "SQUARE";
		case LATE_SQUARE: return "LATE_SQUARE";
		case EARLY_SQUARE: return "EARLY_SQUARE";
		case LATE_LINEAR: return "LATE_LINEAR";
		case LATE_EASE_IN_EASE_OUT: return "LATE_EASE_IN_EASE_OUT";
		case VERY_LATE_LINEAR: return "VERY_LATE_LINEAR";
		case VERY_LATE_EASE_IN_EASE_OUT: return "VERY_LATE_EASE_IN_EASE_OUT";		
		case QUADRATIC_EASE_IN: return "QUADRATIC_EASE_IN";
		case QUADRATIC_EASE_OUT: return "QUADRATIC_EASE_OUT";
		case DOUBLE_EXPONENTIAL_SEAT: return "DOUBLE_EXPONENTIAL_SEAT";
		default: return "UNKNOWN_CURVE!";
	}
	return "error";
}

void ofxAnimatable::setup(){

	transitionSpeed_ = 1.0f / DEFAULT_ANIMATION_DURATION;
	percentDone_ = 0.0f;
	animating_ = false;
	paused_ = false;
	direction_ = 1;
	playcount_ = 0;
	repeat_ = PLAY_ONCE;
	curveStyle_ = EASE_IN_EASE_OUT;
	currentSpeed_ = 0.0f;
	lastDT_ = 1;
	waitTime_ = 0.0f;
	delay_ = 0.0f;
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

		case LATE_LINEAR:
			r = ( percent < 0.5f ) ? 0.0f : 2.0f * percent - 1.0f; break;

		case VERY_LATE_LINEAR:
			r = ( percent < 0.75f ) ? 0.0f : 4.0f * percent - 3.0f; break;

		case TANH:
			r = 0.5f + 0.5f * tanh( 2.0f * M_PI * percent - M_PI ) * 1.00374187319732; break;

		case SINH:
			r = 0.5f + 0.23482f * sinh( 3.0f * percent - 1.5f ); break;

		case SQUARE:
			r = (percent < 0.5f) ? 0.0f : 1.0f; break;
			
		case LATE_SQUARE:
			r = (percent < 0.75f) ? 0.0f : 1.0f; break;

		case EARLY_SQUARE:
			r = (percent < 0.25f) ? 0.0f : 1.0f; break;

		case LATE_EASE_IN_EASE_OUT:
			r = (percent < 0.5f) ? 0.0f : 0.5f + 0.5f * cosf( 2.0f * M_PI * percent); break;

		case VERY_LATE_EASE_IN_EASE_OUT:
			r = (percent < 0.75f) ? 0.0f : 0.5f + 0.5f * cosf( 4.0f * M_PI * percent); break;
			
		case QUADRATIC_EASE_IN:
			r = percent * percent; break;

		case QUADRATIC_EASE_OUT:
			r = 1.0f - (percent - 1.0f) * (percent - 1.0f); break;

		case BOUNCY:{
			float k = 0.5f;
			r = 0.5f - 0.51f * cosf( M_PI * percent + k * percent - k * 0.5f ); break;
		}

		case DOUBLE_EXPONENTIAL_SEAT:{
			r = quadraticBezier(percent, 0.7, 0.99); //TODO PARAM!
		}
			
		default: ;
	}
	
	currentSpeed_ =  r - prevSpeed_;
	prevSpeed_ = r;
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
		
		percentDone_ += direction_ * transitionSpeed_ * dt;
		
		if ( percentDone_ >= 1.0f || percentDone_ <= 0.0f ){

			animating_ = false;
			
			if (percentDone_ >= 1.0f) percentDone_ = 1.0f;
			else percentDone_ = 0.0f;
						
			switch (repeat_) {

				case PLAY_ONCE:	
					break;	//nothing to do;
					
				case LOOP_BACK_AND_FORTH:
					direction_ = -direction_;
					animating_ = true;
					break;
					
				case LOOP:	
					animating_ = true;
					percentDone_ = 0.0f;
					break;

				case LOOP_BACK_AND_FORTH_ONCE:
					if (playcount_ >= 1){	//time to stop
						//we are done
					}else{	
						direction_ = -direction_;
						animating_ = true;						
						playcount_++;
					}
					break;
			}
		}
	}
	lastDT_ = dt;
}


void ofxAnimatable::startAnimation(){
	direction_ = 1;
	percentDone_ = 0.0f;
	delay_ = 0.0f;
	animating_ = true;
	playcount_ = 0;
	paused_ = false;
	currentSpeed_ = 0.0f;
	prevSpeed_ = 0.0f;
}

void ofxAnimatable::startAnimationAfterDelay(float delay){
	direction_ = 1;
	delay_ = delay;
	waitTime_ = delay_;
	//animating_ = false;
}


void ofxAnimatable::reset(){
	percentDone_ = 0.0f;
	delay_ = 0.0f;
	animating_ = false;
	playcount_ = 0;
	paused_ = false;
	currentSpeed_ = 0.0f;
	prevSpeed_ = 0.0f;
}


void ofxAnimatable::setDuration( float seconds ){
	transitionSpeed_ = 1.0f / seconds;
}


void ofxAnimatable::setRepeatType( AnimRepeat repeat ){
	repeat_ = repeat;
}


void ofxAnimatable::setCurve( AnimCurve curveStyle){
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
	return ( delay_ > 0.0f );
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
