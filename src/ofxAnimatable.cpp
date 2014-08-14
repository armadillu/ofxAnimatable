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


float cubicBezier(float x, float a, float b, float c, float d){

	float y0a = 0.00f; // initial y
	float x0a = 0.00f; // initial x
	float y1a = b;    // 1st influence y
	float x1a = a;    // 1st influence x
	float y2a = d;    // 2nd influence y
	float x2a = c;    // 2nd influence x
	float y3a = 1.00f; // final y
	float x3a = 1.00f; // final x

	float A = x3a - 3.0f * x2a + 3.0f * x1a - x0a;
	float B = 3.0f * x2a - 6.0f * x1a + 3.0f * x0a;
	float C = 3.0f * x1a - 3.0f * x0a;
	float D = x0a;

	float E = y3a - 3.0f * y2a + 3.0f * y1a - y0a;
	float F = 3.0f * y2a - 6.0f * y1a + 3.0f * y0a;
	float G = 3.0f * y1a - 3.0f * y0a;
	float H = y0a;

	// Solve for t given x (using Newton-Raphelson), then solve for y given t.
	// Assume for the first guess that t = x.
	float currentt = x;
	int nRefinementIterations = 5;
	for (int i = 0; i < nRefinementIterations; i++){
		float currentx = xFromT (currentt,A,B,C,D);
		float currentslope = slopeFromT(currentt,A,B,C);
		currentt -= (currentx - x)*(currentslope);
		currentt = ofClamp(currentt, 0.0f, 1.0f);
	}

	return yFromT(currentt,E,F,G,H);
}


//////////////////////////////////////////////////////////
// Helper functions.


float  findx (float t, float x0, float x1, float x2, float x3){
	return x0 * B0F(t) + x1 * B1F(t) + x2 * B2F(t) + x3 * B3F(t);
}
float  findy (float t, float y0, float y1, float y2, float y3){
	return y0 * B0F(t) + y1 * B1F(t) + y2 * B2F(t) + y3 * B3F(t);
}

float cubicBezierNearlyThroughTwoPoints(float x, float a, float b, float c, float d){

	float y = 0.0f;
	float epsilon = 0.00001f;
	float min_param_a = 0.0f + epsilon;
	float max_param_a = 1.0f - epsilon;
	float min_param_b = 0.0f + epsilon;
	float max_param_b = 1.0f - epsilon;
	a = max(min_param_a, min(max_param_a, a));
	b = max(min_param_b, min(max_param_b, b));

	float x0 = 0.0f;
	float y0 = 0.0f;
	float x4 = a;
	float y4 = b;
	float x5 = c;
	float y5 = d;
	float x3 = 1.0f;
	float y3 = 1.0f;
	float x1,y1,x2,y2; // to be solved.

	// arbitrary but reasonable
	// t-values for interior control points
	float t1 = 0.3f;
	float t2 = 0.7f;

	float B0t1 = B0F(t1);
	float B1t1 = B1F(t1);
	float B2t1 = B2F(t1);
	float B3t1 = B3F(t1);
	float B0t2 = B0F(t2);
	float B1t2 = B1F(t2);
	float B2t2 = B2F(t2);
	float B3t2 = B3F(t2);

	float ccx = x4 - x0*B0t1 - x3*B3t1;
	float ccy = y4 - y0*B0t1 - y3*B3t1;
	float ffx = x5 - x0*B0t2 - x3*B3t2;
	float ffy = y5 - y0*B0t2 - y3*B3t2;

	x2 = (ccx - (ffx*B1t1)/B1t2) / (B2t1 - (B1t1*B2t2)/B1t2);
	y2 = (ccy - (ffy*B1t1)/B1t2) / (B2t1 - (B1t1*B2t2)/B1t2);
	x1 = (ccx - x2*B2t1) / B1t1;
	y1 = (ccy - y2*B2t1) / B1t1;

	x1 = max(0.0f + epsilon, min(1.0f - epsilon, x1));
	x2 = max(0.0f + epsilon, min(1.0f - epsilon, x2));

	// Note that this function also requires cubicBezier()!
	y = cubicBezier(x, x1, y1, x2, y2);
	y = MAX(0.0f, MIN(1.0f, y));
	return y;
}


/////////////////////////


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
		case CUBIC_BEZIER_PARAM: return "CUBIC_BEZIER_PARAM";
		case EXPONENTIAL_SIGMOID_PARAM: return "EXPONENTIAL_SIGMOID_PARAM";
		case SWIFT_GOOGLE: return "SWIFT_GOOGLE";
		case OBJECT_DROP: return "OBJECT_DROP";

		default: return "UNKNOWN_CURVE!";
	}
	return "error";
}

void ofxAnimatable::setup(){

	doubleExpSigmoidParam = 0.5f;
	quadraticBezierParamA = 0.25f;
	quadraticBezierParamB = 0.75f;
	cubicBezierParamA = 0.033f;
	cubicBezierParamB = 0.973f;
	cubicBezierParamC = 0.250f;
	cubicBezierParamD = 0.750f;
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
	curveStylePtr_ = &curveStyle_;
}

void ofxAnimatable::drawCurve(int x, int y, int size, bool bg, ofColor c ){

#if defined(OF_VERSION) || defined(OF_VERSION_MAJOR)
	int xx = x;
	int yy = y;
	float s = size;
	ofPushStyle();
	if(bg){
		ofSetColor(22);
		ofRect(x, y, size, size);
	}
	float steps = size;
	string name = ofxAnimatable::getCurveName(*curveStylePtr_);
	glPointSize(1);
	ofMesh m;

	m.setMode(OF_PRIMITIVE_LINE_STRIP);
	float step = 1./steps;
	float p1, p2, p3, p4;
	fillInParams(p1,p2,p3, p4);
	for (float i = 0.0f ; i< 1.0f; i+= step){
		float val = calcCurveAt(i, *curveStylePtr_, p1, p2, p3, p4);
		m.addVertex( ofVec3f(xx + s * i, yy + s - s * val) );
	}

	ofSetColor(c);
	ofSetLineWidth(2);
	m.draw();
	if (*curveStylePtr_ == CUBIC_BEZIER_PARAM || *curveStylePtr_ == QUADRATIC_BEZIER_PARAM ||
		*curveStylePtr_ == EXPONENTIAL_SIGMOID_PARAM ){
		ofMesh pts;
		glPointSize(3);
		pts.setMode(OF_PRIMITIVE_POINTS);
		switch (*curveStylePtr_) {
			case CUBIC_BEZIER_PARAM:
				pts.addColor(ofColor::red);
				pts.addVertex(ofVec2f(xx + cubicBezierParamA * size, yy + cubicBezierParamB * size));
				pts.addColor(ofColor::green);
				pts.addVertex(ofVec2f(xx + cubicBezierParamC * size, yy +cubicBezierParamD * size));
				break;
			case QUADRATIC_BEZIER_PARAM:
				pts.addColor(ofColor::blue);
				pts.addVertex(ofVec2f(xx + quadraticBezierParamA * size, yy + quadraticBezierParamB * size));
				break;
			case EXPONENTIAL_SIGMOID_PARAM:
				pts.addColor(ofColor::cyan);
				pts.addVertex(ofVec2f(xx + doubleExpSigmoidParam * size, yy + size));
				break;
		}
		pts.draw();
	}

	ofSetLineWidth(1);
	ofSetColor(255,255); //axes
	ofLine(xx,yy + s, xx + s, yy + s);
	ofLine(xx,yy, xx, yy + s);
	ofSetColor(255,32); //linear
	ofLine(xx,yy + s, xx + s, yy );
	ofSetColor(170); //label
	ofDrawBitmapString(name, x, y + s + 15);
	ofPopStyle();
#endif
}



void ofxAnimatable::fillInParams(float &p1, float &p2, float &p3, float &p4){

	switch (*curveStylePtr_) { //in case our curve has extra params, fill them in
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

		case CUBIC_BEZIER_PARAM:
			p1 = cubicBezierParamA;
			p2 = cubicBezierParamB;
			p3 = cubicBezierParamC;
			p4 = cubicBezierParamD;

		default:
			break;
	}
}

float ofxAnimatable::calcCurveAt( float percent ){

	float p1, p2, p3, p4;
	fillInParams(p1,p2,p3,p4);
	float r = calcCurveAt(percent, *curveStylePtr_, p1, p2, p3, p4);
	
	currentSpeed_ =  r - prevSpeed_; //this is very ghetto and should be done properly! TODO
	prevSpeed_ = r;
	return r;
}

float ofxAnimatable::calcCurveAt(float percent, AnimCurve type, float p1, float p2, float p3, float p4){

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
			r = quadraticBezier(percent, p1, p2); break; 
		}

		case EXPONENTIAL_SIGMOID_PARAM:{
			r = doubleExponentialSigmoid(percent, p1); break;
		}

		case CUBIC_BEZIER_PARAM:{
			r = cubicBezier(percent, p1, p2, p3, p4); break;
		}

		case SWIFT_GOOGLE:{
			r = cubicBezier(percent, 0.444f, 0.013f, 0.188f, 0.956f); break;
		}

		case OBJECT_DROP:{
			if ( percent < 0.75f ){
				r = cosf( (2.0f * M_PI / 3.0f) * percent );
			}else{
				float range = 0.125f;
				float diffRange = 0.125f;
				float amp = p1;
				float freq = 8.0f;

				if ( percent < 0.75f + range ){
					r = amp * sinf( freq * M_PI * percent );
				}else{
					diffRange *= 0.5f; range += diffRange; amp *= 0.5f; freq *= 2.0f;
					if ( percent < 0.75f + range ){
						r = amp * sinf( freq * M_PI * percent );
					}else{
						diffRange *= 0.5f; range += diffRange; amp *= 0.5f; freq *= 2.0f;
						if ( percent < 0.75f + range ){
							r = amp * sinf( freq * M_PI * percent );
						}else{
							diffRange *= 0.5f; range += diffRange; amp *= 0.5f; freq *= 2.0f;
							if ( percent < 0.75f + range ){
								r = amp * sinf( freq * M_PI * percent );
							}else{
								diffRange *= 0.5f; range += diffRange; amp *= 0.5f; freq *= 2.0f;
								if ( percent < 0.75f + range ){
									r = amp * sinf( freq * M_PI * percent );
								}else{
									diffRange *= 0.5f; range += diffRange; amp *= 0.5f; freq *= 2.0f;
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

void ofxAnimatable::setCurve( AnimCurve *curveStyle){
	curveStylePtr_ = curveStyle;
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
