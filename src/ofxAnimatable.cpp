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

float doublePolynomialSigmoid(float x, int n){

	float y = 0.0f;
	if (n%2 == 0){
		// even polynomial
		if (x<=0.5f){
			y = pow(2.0f*x, n)/2.0f;
		} else {
			y = 1.0f - pow(2.0f*(x-1.0f), n)/2.0f;
		}
	}

	else {
		// odd polynomial
		if (x <= 0.5f){
			y = pow( 2.0f * x, n) / 2.0f;
		} else {
			y = 1.0 + pow( 2.0f * (x-1.0f), n) / 2.0;
		}
	}
	return y;
}

float doubleExponentialSigmoid (float x, float a){

	float epsilon = 0.00001f;
	float min_param_a = 0.0f + epsilon;
	float max_param_a = 1.0f - epsilon;
	a = min(max_param_a, max(min_param_a, a));
	a = 1.0f - a; // for sensible results

	float y = 0.0f;
	if ( x <= 0.5f ){
		y = ( pow( 2.0f * x, 1.0f/a) ) / 2.0f;
	} else {
		y = 1.0 - ( pow( 2.0f * (1.0f-x), 1.0f / a) ) / 2.0f;
	}
	return y;
}

float doubleExponentialSeat (float x, float a){

	float epsilon = 0.00001f;
	float min_param_a = 0.0f + epsilon;
	float max_param_a = 1.0f - epsilon;
	a = min(max_param_a, max(min_param_a, a));

	float y = 0.0f;
	if (x <= 0.5f){
		y = (powf(2.0 * x, 1.0f - a) ) / 2.0f;
	} else {
		y = 1.0f - ( pow(2.0f * ( 1.0f - x ), 1.0f-a) ) / 2.0f;
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
		float y = powf(x, 1.0/(1-a));
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
		case BLINK_5: return "BLINK_5";
		case BLINK_3: return "BLINK_3";
		case BLINK_2: return "BLINK_2";
		case BLINK_AND_FADE_1: return "BLINK_AND_FADE_1";
		case BLINK_AND_FADE_2: return "BLINK_AND_FADE_2";
		case BLINK_AND_FADE_3: return "BLINK_AND_FADE_3";
		case LATE_LINEAR: return "LATE_LINEAR";
		case LATE_EASE_IN_EASE_OUT: return "LATE_EASE_IN_EASE_OUT";
		case EARLY_LINEAR: return "EARLY_LINEAR";
		case VERY_LATE_LINEAR: return "VERY_LATE_LINEAR";
		case VERY_LATE_EASE_IN_EASE_OUT: return "VERY_LATE_EASE_IN_EASE_OUT";		
		case QUADRATIC_EASE_IN: return "QUADRATIC_EASE_IN";
		case QUADRATIC_EASE_OUT: return "QUADRATIC_EASE_OUT";
		case EARLY_QUADRATIC_EASE_OUT: return "EARLY_QUADRATIC_EASE_OUT";
		case QUADRATIC_BEZIER_PARAM: return "QUADRATIC_BEZIER_PARAM";
		case EXPONENTIAL_SIGMOID_PARAM: return "EXPONENTIAL_SIGMOID_PARAM";
		case OBJECT_DROP: return "OBJECT_DROP";

		default: return "UNKNOWN_CURVE!";
	}
	return "error";
}

void ofxAnimatable::setup(){

	doubleExpSigmoidParam = 0.5;
	quadraticBezierParamA = 0.25;
	quadraticBezierParamB = 0.75;
	bounceAmp = 0.05;
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

void ofxAnimatable::drawCurve(int x, int y, int size, bool bg){

#if (OF_AVAILABLE)
	int xx = x;
	int yy = y;
	float s = size;
	if(bg){
		ofSetColor(22);
		ofRect(x, y, size, size);
	}
	float steps = size;
	string name = ofxAnimatable::getCurveName(curveStyle_);
	glPointSize(1);
	glColor4ub(255,255,255, 64);
	ofLine(xx,yy + s, xx + s, yy + s);
	ofLine(xx,yy, xx, yy + s);
	glColor4ub(255,255,255, 32);
	ofLine(xx,yy + s, xx + s, yy );
	glColor4ub(255,255,255, 255);
	ofMesh m;
	m.setMode(OF_PRIMITIVE_LINE_STRIP);
	float step = 1./steps;
	float p1, p2, p3;
	fillInParams(p1,p2,p3);
	for (float i = 0.0f ; i< 1.0f; i+= step){
		float val = calcCurveAt(i, curveStyle_, p1, p2, p3);
		m.addVertex( ofVec3f(xx + s * i, yy + s - s * val) );
	}
	m.draw();
	glColor4ub(255,255,255, 255);
	ofDrawBitmapString(name, x, y + s + 15);
#endif
}


void ofxAnimatable::fillInParams(float &p1, float &p2, float &p3){

	switch (curveStyle_) { //in case our curve has extra params, fill them in
		case QUADRATIC_BEZIER_PARAM:
			p1 = quadraticBezierParamA;
			p2 = quadraticBezierParamB;
			break;
		case EXPONENTIAL_SIGMOID_PARAM:
			p1 = doubleExpSigmoidParam;
			break;

		case OBJECT_DROP:
			p1 = bounceAmp;
			break;

		default:
			break;
	}
}

float ofxAnimatable::calcCurveAt( float percent ){

	float p1, p2, p3;
	fillInParams(p1,p2,p3);
	float r = calcCurveAt(percent, curveStyle_, p1, p2, p3);
	
	currentSpeed_ =  r - prevSpeed_; //this is very ghetto and should be done properly! TODO
	prevSpeed_ = r;
	return r;
}

float ofxAnimatable::calcCurveAt(float percent, AnimCurve type, float param1, float param2, float param3){

	float r = percent;
	switch ( type ) {

		case EASE_IN_EASE_OUT:
			r = 0.5f - 0.5f * cosf( M_PI * percent ); break;

		case EASE_IN:
			r = 1.0f + sinf( M_PI_2 * percent - M_PI_2); break;

		case EASE_OUT:
			r = sinf( M_PI_2 * percent ); break;

		case LINEAR:
			break;

		case EARLY_LINEAR:
			r = ( percent < 0.25f ) ? 0.0f : 1.33333333f * (percent - 0.25f); break;

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

		case BLINK_5:{
			float v = percent * 5.0f;
			r = (fmod(v, 1.01f) < 0.5f ? 0.0f : 1.0f);
			}break;

		case BLINK_3:{
			float v = percent * 3.0f;
			r = (fmod(v, 1.01f) < 0.5f ? 0.0f : 1.0f);
			}break;

		case BLINK_2:{
			float v = percent * 2.0f;
			r = (fmod(v, 1.01f) < 0.5f ? 0.0f : 1.0f);
			}break;

		case BLINK_AND_FADE_1:{
			float v = percent * 2.0f;
			r = (fmod(v, 1.01f) < 0.5f ? 0.0f : 1.0f);
			if (percent >= 0.75) r = 4 * percent - 4 * 0.75f ;
		}break;

		case BLINK_AND_FADE_2:{
			float v = percent * 3.0f;
			r = (fmod(v, 1.01f) < 0.5f ? 0.0f : 1.0f);
			if (percent >= 0.833333f) r = 6 * percent - 6 * 0.833333f ;
		}break;

		case BLINK_AND_FADE_3:{
			float v = percent * 4.0f;
			r = (fmod(v, 1.01f) < 0.5f ? 0.0f : 1.0f);
			if (percent >= 0.875f) r = 8 * percent - 8 * 0.875f ;
		}break;


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

		case EARLY_QUADRATIC_EASE_OUT:{
			float p = 1.333333333f;
			float x = (percent - 0.25) * p;
			r = (percent < 0.25f) ? 0.0f :  1.0f - ( x - 1.0f) * ( x - 1.0f);
		}break;

		case BOUNCY:{
			float k = 0.5f;
			r = 0.5f - 0.51f * cosf( M_PI * percent + k * percent - k * 0.5f );
			}break;

		case QUADRATIC_BEZIER_PARAM:{
			r = quadraticBezier(percent, param1, param2); break; 
		}

		case EXPONENTIAL_SIGMOID_PARAM:{
			r = doubleExponentialSigmoid(percent, param1); break;
		}

		case OBJECT_DROP:{
			if ( percent < 0.75f ){
				r = cosf( (2.0f * M_PI / 3.0f) * percent );
			}else{
				float range = 0.125f;
				float diffRange = 0.125;
				float amp = param1;
				float freq = 8;

				if ( percent < 0.75f + range ){
					r = amp * sinf( freq * M_PI * percent );
				}else{
					diffRange *= 0.5; range += diffRange; amp *= 0.5f; freq *= 2.0f;
					if ( percent < 0.75f + range ){
						r = amp * sinf( freq * M_PI * percent );
					}else{
						diffRange *= 0.5; range += diffRange; amp *= 0.5f; freq *= 2.0f;
						if ( percent < 0.75f + range ){
							r = amp * sinf( freq * M_PI * percent );
						}else{
							diffRange *= 0.5; range += diffRange; amp *= 0.5f; freq *= 2.0f;
							if ( percent < 0.75f + range ){
								r = amp * sinf( freq * M_PI * percent );
							}else{
								diffRange *= 0.5; range += diffRange; amp *= 0.5f; freq *= 2.0f;
								if ( percent < 0.75f + range ){
									r = amp * sinf( freq * M_PI * percent );
								}else{
									diffRange *= 0.5; range += diffRange; amp *= 0.5f; freq *= 2.0f;
									if ( percent < 0.75f + range ){
										r = amp * sinf( freq * M_PI * percent );
									}else{
										r = 0;
									}
								}
							}
						}
					}
				}
			}
			r = 1.0f-r;
			break;
		}

		default: ;
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

void ofxAnimatable::setPercentDone(float p){
	if( p < 0.0f ) p = 0.0f;
	percentDone_ = p;
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

float ofxAnimatable::getCurrentSpeed(){
	float r = fabs( direction_ * currentSpeed_ / (lastDT_ * transitionSpeed_));
	return r;
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
