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


inline float cubicBezier(float x, float a, float b, float c, float d){

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


inline float findx (float t, float x0, float x1, float x2, float x3){
	return x0 * B0F(t) + x1 * B1F(t) + x2 * B2F(t) + x3 * B3F(t);
}
inline float  findy (float t, float y0, float y1, float y2, float y3){
	return y0 * B0F(t) + y1 * B1F(t) + y2 * B2F(t) + y3 * B3F(t);
}

inline float cubicBezierNearlyThroughTwoPoints(float x, float a, float b, float c, float d){

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
			y = pow( 2.0f * x, n) * 0.5f;
		} else {
			y = 1.0f + pow( 2.0f * (x-1.0f), n) * 0.5f;
		}
	}
	return y;
}

//http://stackoverflow.com/questions/5161465/how-to-create-custom-easing-function-with-core-animation
//https://github.com/PrimaryFeather/Sparrow-Framework/blob/master/sparrow/src/Classes/SPTransitions.m#L86

float easeOutBounce(float ratio){

	float s = 7.5625f;
	float p = 2.75f;
	float pow = 2.0f;
	float l;
	if (ratio < (1.0f/p)){
		l = s * powf(ratio, pow);
	}else{
		if (ratio < (2.0f/p)){
			ratio -= 1.5f/p;
			l = s * powf(ratio, pow) + 0.75f;
		}else{
			if (ratio < 2.5f/p){
				ratio -= 2.25f/p;
				l = s * powf(ratio, pow) + 0.9375f;
			}else{
				ratio -= 2.625f/p;
				l = s * powf(ratio, pow) + 0.984375f;
			}
		}
	}
	return l;
}


inline float easeInBounce(float ratio){
	return 1.0f - easeOutBounce(1.0f - ratio);
}

inline float easeInOutBounce(float ratio){
	if (ratio < 0.5f) return 0.5f * easeInBounce(ratio * 2.0f);
	else              return 0.5f * easeOutBounce((ratio - 0.5f) * 2.0f) + 0.5f;
}


inline float easeOutInBounce(float ratio){
	if (ratio < 0.5f) return 0.5f * easeOutBounce(ratio * 2.0f);
	else              return 0.5f * easeInBounce((ratio - 0.5f) * 2.0f) + 0.5f;
}


inline float easeInBack(float ratio, float offset = 0.0){
	float s = 1.70158f + offset;
	return powf(ratio, 2.0f) * ((s + 1.0f) * ratio - s);
}

inline float easeOutBack(float ratio, float offset = 0.0){
	float invRatio = ratio - 1.0f;
	float s = 1.70158f + offset;
	return powf(invRatio, 2.0f) * ((s + 1.0f) * invRatio + s) + 1.0f;
}

inline float easeInOutBack(float ratio, float offset = 0.0){
	if (ratio < 0.5f) return 0.5f * easeInBack (ratio * 2.0f, offset);
	else              return 0.5f * easeOutBack((ratio - 0.5f) * 2.0f, offset) + 0.5f;
}

inline float easeOutInBack(float ratio, float offset = 0.0){
	if (ratio < 0.5f) return 0.5f * easeOutBack(ratio * 2.0f, offset);
	else              return 0.5f * easeInBack((ratio - 0.5f) * 2.0f, offset) + 0.5f;
}

inline float easeInElastic(float ratio, float param1 = 1.0f, float param2 = 1.0f, float param3 = 1.0f){
	if (ratio == 0.0f || ratio == 1.0f) return ratio;
	else{
		float p = 0.3f * param2;
		float s = p * 0.25f;
		float invRatio = ratio - 1.0f;
		return -1.0f * powf(2.0f + param3, param1 * 10.0f * invRatio) * sinf((invRatio*invRatio-s) * TWO_PI / p);
	}
}


inline float easeOutElastic(float ratio, float param1 = 1.0f, float param2 = 1.0f, float param3 = 1.0f){
	if (ratio == 0.0f || ratio == 1.0f) return ratio;
	else{
		float p = 0.3f * param2;
		float s = p * 0.25f;
		return powf(2.0f + param3, -param1 * 10.0f * ratio) * sinf((ratio*ratio-s) * TWO_PI / p) + 1.0f;
	}
}

inline float easeInOutElastic(float ratio, float param1 = 1.0f, float param2 = 1.0f, float param3 = 1.0f){
	if (ratio < 0.5f) return 0.5f * easeInElastic( ratio * 2.0f, param1, param2, param3);
	else              return 0.5f * easeOutElastic((ratio -0.5f ) * 2.0f, param1, param2, param3) + 0.5f;
}

inline float easeOutInElastic(float ratio, float param1 = 1.0f, float param2 = 1.0f, float param3 = 1.0f){
	if (ratio < 0.5f) return 0.5f * easeOutElastic(ratio * 2.0f, param1, param2, param3);
	else              return 0.5f * easeInElastic((ratio - 0.5f) * 2.0f, param1, param2, param3) + 0.5f;
}


inline float doubleExponentialSigmoid (float x, float a){

	float epsilon = 0.00001f;
	float min_param_a = 0.0f + epsilon;
	float max_param_a = 1.0f - epsilon;
	a = min(max_param_a, max(min_param_a, a));
	a = 1.0f - a; // for sensible results

	float y = 0.0f;
	if ( x <= 0.5f ){
		y = ( pow( 2.0f * x, 1.0f/a) ) * 0.5f;
	} else {
		y = 1.0f - ( pow( 2.0f * (1.0f-x), 1.0f / a) ) * 0.5f;
	}
	return y;
}

inline float doubleExponentialSeat (float x, float a){

	float epsilon = 0.00001f;
	float min_param_a = 0.0f + epsilon;
	float max_param_a = 1.0f - epsilon;
	a = min(max_param_a, max(min_param_a, a));

	float y = 0.0f;
	if (x <= 0.5f){
		y = (powf(2.0f * x, 1.0f - a) ) * 0.5f;
	} else {
		y = 1.0f - ( pow(2.0f * ( 1.0f - x ), 1.0f-a) ) * 0.5f;
	}
	return y;
}


inline float exponentialEasing (float x, float a){

	float epsilon = 0.00001f;
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
		float y = powf(x, 1.0f/(1.0f-a));
		return y;
	}
}


//http://en.wikipedia.org/wiki/Smoothstep

inline float smoothStep(float x){
	return x * x * (3.0f - 2.0f * x);
}

inline float smootherStep(float x){
	return x * x * x * ( x * ( x * 6.0f - 15.0f) + 10.0f);
}

inline float quadraticBezier(float x, float a, float b){
	// adapted from BEZMATH.PS (1993)
	// by Don Lancaster, SYNERGETICS Inc.
	// http://www.tinaja.com/text/bezmath.html

	float epsilon = 0.00001f;
	a = max(0.0f, min(1.0f, a));
	b = max(0.0f, min(1.0f, b));
	if (a == 0.5f){
		a += epsilon;
	}

	// solve t from x (an inverse operation)
	float om2a = 1.0f - 2.0f * a;
	float t = (sqrt( a * a + om2a * x) - a) / om2a;
	float y = (1.0f - 2.0f * b) * (t * t) + (2.0f * b) * t;
	return y;
}


float customBounce(float t,
				   float numBm, float acc,
				   float* bDurations, float* bVels ){

	if(bDurations == NULL || bVels == NULL) return 0.0f;

	t = ofClamp(t, 0.0f, 1.0f);
	int index = 0;
	float totalDuration = 0.0f;

	// get index to particular bounce
	while(index < numBm && t > totalDuration + bDurations[index]){
		totalDuration += bDurations[index];
		index++;
	}

	// get duration within bounce (if not the first one)
	t = t - totalDuration;

	float height = 0.0f;
	if (index == 0){
		height = 1.0f + acc * (t * t) * 0.5f;
	}else{
		height = t * (bVels[index] + (acc * t) * 0.5f);
	}
	return height;
}

ofxAnimatable::ofxAnimatable(){
	autoUpdating = false;
}

ofxAnimatable::~ofxAnimatable(){
	if(autoUpdating){
		stopAutoUpdate();
	}
}

// taken from http://sbcgamesdev.blogspot.com/2013/05/bounce-timing-function_8.html and moded heavily
void ofxAnimatable::initCustomBounce(){

	// get "some" acceleration and calculate time for bounces
	float acceleration = 0.001f;
	float totalDuration = 0.0f;
	float height = 1.0f;
	for (int i = 0; i < bounceNumB; i++){
		// s = 1/2 a * t^2 ... 2s / a = t^2 ... sqrt(2s / a) = t
		float duration = sqrtf(2.0f * height / acceleration) * 2.0f;
		if (i == 0) duration *= 0.5f;
		bounceDuration[i] = duration;
		totalDuration += duration;
		height *= bounceElast;
	}

	// adjust total duration to fit requested duration
	for (int i = 0; i < bounceNumB; i++){
		bounceDuration[i] = bounceDuration[i] / totalDuration;
	}

	// s = 1/2 a * t^2 ... 2s / t^2 = a
	bounceAcc = 2.0f / (bounceDuration[0] * bounceDuration[0]);

	// calculate initial bounce velocities
	for (int i = 0; i < bounceNumB; i++){
		if (i == 0) bounceVelocity[i] = 0.0f;
		else bounceVelocity[i] = bounceDuration[i] / 2.0f * bounceAcc;
	}
	// change the sign of acceleration to point downwards
	bounceAcc = -bounceAcc;
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

		case EASE_IN_BACK: return "EASE_IN_BACK";
		case EASE_OUT_BACK: return "EASE_OUT_BACK";
		case EASE_IN_OUT_BACK: return "EASE_IN_OUT_BACK";
		case EASE_OUT_IN_BACK: return "EASE_OUT_IN_BACK";

		case EASE_IN_BOUNCE: return "EASE_IN_BOUNCE";
		case EASE_OUT_BOUNCE: return "EASE_OUT_BOUNCE";
		case EASE_IN_OUT_BOUNCE: return "EASE_IN_OUT_BOUNCE";
		case EASE_OUT_IN_BOUNCE: return "EASE_OUT_IN_BOUNCE";

		case EASE_IN_ELASTIC: return "EASE_IN_ELASTIC";
		case EASE_OUT_ELASTIC: return "EASE_OUT_ELASTIC";
		case EASE_IN_OUT_ELASTIC: return "EASE_IN_OUT_ELASTIC";
		case EASE_OUT_IN_ELASTIC: return "EASE_OUT_IN_ELASTIC";

		case BOUNCE_IN_CUSTOM: return "BOUNCE_IN_CUSTOM";
		case BOUNCE_OUT_CUSTOM: return "BOUNCE_OUT_CUSTOM";

		case SMOOTH_STEP: return "SMOOTH_STEP";
		case SMOOTHER_STEP: return "SMOOTHER_STEP";

		default: return "UNKNOWN_CURVE!";
	}
	return "error";
}

AnimCurve ofxAnimatable::getCurveFromName(const string& name){

	if(name == "EASE_IN_EASE_OUT") return EASE_IN_EASE_OUT;
	if(name == "EASE_IN") return EASE_IN;
	if(name == "EASE_OUT") return EASE_OUT;
	if(name == "LINEAR") return LINEAR;
	if(name == "BOUNCY") return BOUNCY;
	if(name == "TANH") return TANH;
	if(name == "SINH") return SINH;
	if(name == "SQUARE") return SQUARE;
	if(name == "LATE_SQUARE") return LATE_SQUARE;
	if(name == "EARLY_SQUARE") return EARLY_SQUARE;
	if(name == "BLINK_5") return BLINK_5;
	if(name == "BLINK_3") return BLINK_3;
	if(name == "BLINK_2") return BLINK_2;
	if(name == "BLINK_AND_FADE_1") return BLINK_AND_FADE_1;
	if(name == "BLINK_AND_FADE_2") return BLINK_AND_FADE_2;
	if(name == "BLINK_AND_FADE_3") return BLINK_AND_FADE_3;
	if(name == "LATE_LINEAR") return LATE_LINEAR;
	if(name == "LATE_EASE_IN_EASE_OUT") return LATE_EASE_IN_EASE_OUT;
	if(name == "EARLY_LINEAR") return EARLY_LINEAR;
	if(name == "VERY_LATE_LINEAR") return VERY_LATE_LINEAR;
	if(name == "VERY_LATE_EASE_IN_EASE_OUT") return VERY_LATE_EASE_IN_EASE_OUT;
	if(name == "QUADRATIC_EASE_IN") return QUADRATIC_EASE_IN;
	if(name == "QUADRATIC_EASE_OUT") return QUADRATIC_EASE_OUT;
	if(name == "EARLY_QUADRATIC_EASE_OUT") return EARLY_QUADRATIC_EASE_OUT;
	if(name == "QUADRATIC_BEZIER_PARAM") return QUADRATIC_BEZIER_PARAM;
	if(name == "CUBIC_BEZIER_PARAM") return CUBIC_BEZIER_PARAM;
	if(name == "EXPONENTIAL_SIGMOID_PARAM") return EXPONENTIAL_SIGMOID_PARAM;
	if(name == "SWIFT_GOOGLE") return SWIFT_GOOGLE;
	if(name == "OBJECT_DROP") return OBJECT_DROP;
	if(name == "EASE_IN_BACK") return EASE_IN_BACK;
	if(name == "EASE_OUT_BACK") return EASE_OUT_BACK;
	if(name == "EASE_IN_OUT_BACK") return EASE_IN_OUT_BACK;
	if(name == "EASE_OUT_IN_BACK") return EASE_OUT_IN_BACK;

	if(name == "EASE_IN_BOUNCE") return EASE_IN_BOUNCE;
	if(name == "EASE_OUT_BOUNCE") return EASE_OUT_BOUNCE;
	if(name == "EASE_IN_OUT_BOUNCE") return EASE_IN_OUT_BOUNCE;
	if(name == "EASE_OUT_IN_BOUNCE") return EASE_OUT_IN_BOUNCE;

	if(name == "EASE_IN_ELASTIC") return EASE_IN_ELASTIC;
	if(name == "EASE_OUT_ELASTIC") return EASE_OUT_ELASTIC;
	if(name == "EASE_IN_OUT_ELASTIC") return EASE_IN_OUT_ELASTIC;
	if(name == "EASE_OUT_IN_ELASTIC") return EASE_OUT_IN_ELASTIC;

	if(name == "BOUNCE_IN_CUSTOM") return BOUNCE_IN_CUSTOM;
	if(name == "BOUNCE_OUT_CUSTOM") return BOUNCE_OUT_CUSTOM;

	if(name == "SMOOTH_STEP") return SMOOTH_STEP;
	if(name == "SMOOTHER_STEP") return SMOOTHER_STEP;

	ofLogError("ofxAnimatable") << "Unknown Curve (" << name << ")";
	return EASE_IN_EASE_OUT;
}


void ofxAnimatable::setCustomBounceParams(int bounceNum, float bounceElast_){
	bounceNumB = ofClamp(bounceNum, 1, BOUNCES_MAX-1);
	bounceElast = bounceElast_;
	initCustomBounce();
}


ofxAnimatable& ofxAnimatable::operator=(const ofxAnimatable& o) {
	doubleExpSigmoidParam = o.doubleExpSigmoidParam;
	quadraticBezierParamAx = o.quadraticBezierParamAx;
	bounceAmp = o.bounceAmp;
	cubicBezierParamAx = o.cubicBezierParamAx;
	cubicBezierParamAy = o.cubicBezierParamAy;
	cubicBezierParamBx = o.cubicBezierParamBx;
	cubicBezierParamBy = o.cubicBezierParamBy;
	elasticGain = o.elasticGain;
	elasticFreq = o.elasticFreq;
	elasticDecay = o.elasticFreq;
	easeBackOffset = o.easeBackOffset;

	bounceNumB = o.bounceNumB;
	bounceElast = o.bounceElast;
	bounceAcc = o.bounceAcc;

	for(int i = 0; i < BOUNCES_MAX; i++){
		bounceDuration[i] = o.bounceDuration[i];
		bounceVelocity[i] = o.bounceVelocity[i];
	}

	animating_ = o.animating_;
	paused_ = o.paused_;
	playcount_ = o.playcount_;
	transitionSpeed_ = o.transitionSpeed_;
	percentDone_ = o.percentDone_;
	delay_ = o.delay_;
	waitTime_ = o.waitTime_;
	repeat_ = o.repeat_;
	curveStyle_ = o.curveStyle_;

	ofLogWarning("ofxAnimatable") << "Copying an ofxAnimatable object, this could be troublesome if you dont understand how ofxAnimatable works internally";
	curveStylePtr_ = &curveStyle_;

	direction_ = o.direction_;

	return *this;
}


void ofxAnimatable::setup(){

	doubleExpSigmoidParam = 0.5f;
	quadraticBezierParamAx = 0.25f;
	quadraticBezierParamAy = 0.75f;
	cubicBezierParamAx = 0.033f;
	cubicBezierParamAy = 0.973f;
	cubicBezierParamBx = 0.250f;
	cubicBezierParamBy = 0.750f;
	elasticGain = 1.0f;
	elasticFreq = 1.0f;
	elasticDecay = 0.0f;

	bounceNumB = 7;
	bounceElast = 0.5;
	bounceAcc = 0.0f;
	initCustomBounce();

	desiredPlayCount = 0;
	easeBackOffset = 0.0f;
	bounceAmp = 0.05f;
	transitionSpeed_ = 1.0f / DEFAULT_ANIMATION_DURATION;
	percentDone_ = 0.0f;
	animating_ = false;
	paused_ = false;
	autoFlipCurve = false;
	direction_ = 1;
	playcount_ = 0;
	repeat_ = PLAY_ONCE;
	curveStyle_ = EASE_IN_EASE_OUT;
	curveStylePtr_ = &curveStyle_;
	currentSpeed_ = 0.0f;
	lastDT_ = 1;
	waitTime_ = 0.0f;
	delay_ = 0.0f;
}


void ofxAnimatable::drawCurve(int x, int y, int size, bool bg, ofColor c ){

#if defined(OF_VERSION) || defined(OF_VERSION_MAJOR)
	int xx = x;
	int yy = y;
	float s = size;
	ofPushStyle();
	if(bg){
		ofSetColor(0,230);
		ofRect(x, y, size, size);
	}
	float steps = size;
	string name = ofxAnimatable::getCurveName(*curveStylePtr_);
	glPointSize(1);
	ofMesh m;

	m.setMode(OF_PRIMITIVE_LINE_STRIP);
	float step = 1.0f/steps;
	float p1, p2, p3, p4;
	float *pa1, *pa2;
	fillInParams(p1,p2,p3,p4, &pa1, &pa2);
	for (float i = 0.0f; i < 1.0f; i+= step){
		float val = calcCurveAt(i, *curveStylePtr_, p1, p2, p3, p4, pa1, pa2);
		m.addVertex( ofVec3f(xx + s * i, yy + s - s * val) );
	}

	ofSetColor(c);
	ofSetLineWidth(2);
	m.draw();

	ofSetLineWidth(1);
	ofSetColor(255,255); //axes
	ofLine(xx,yy + s, xx + s, yy + s);
	ofLine(xx,yy, xx, yy + s);
	ofSetColor(255,32); //linear
	ofLine(xx,yy + s, xx + s, yy );
	ofSetColor(c); //label
	ofDrawBitmapString(name, x, y + s + 15);

	if (*curveStylePtr_ == CUBIC_BEZIER_PARAM ||
		*curveStylePtr_ == QUADRATIC_BEZIER_PARAM ||
		*curveStylePtr_ == EXPONENTIAL_SIGMOID_PARAM ||
		*curveStylePtr_ == BOUNCE_IN_CUSTOM ||
		*curveStylePtr_ == BOUNCE_OUT_CUSTOM

		){
		ofMesh pts;
		glPointSize(4);
		pts.setMode(OF_PRIMITIVE_POINTS);
		int blink = (ofGetFrameNum()%6 > 2) ? 255 : 0;
		switch (*curveStylePtr_) {
			case CUBIC_BEZIER_PARAM:
				pts.addColor(ofColor(ofColor::cyan,blink));
				pts.addVertex(ofVec2f(xx + cubicBezierParamAx * size, yy + size - cubicBezierParamAy * size));
				pts.addColor(ofColor(ofColor::magenta,blink));
				pts.addVertex(ofVec2f(xx + cubicBezierParamBx * size, yy + size - cubicBezierParamBy * size));
				ofSetColor(ofColor::cyan);
				ofDrawBitmapString("A", pts.getVertices()[0] + ofVec2f(5,4));
				ofSetColor(ofColor::magenta);
				ofDrawBitmapString("B", pts.getVertices()[1] + ofVec2f(5,4));
				break;
			case QUADRATIC_BEZIER_PARAM:
				pts.addColor(ofColor(ofColor::blue,blink));
				pts.addVertex(ofVec2f(xx + quadraticBezierParamAx * size, yy + size - quadraticBezierParamAy * size));
				ofDrawBitmapString("QB", pts.getVertices()[0] + ofVec2f(5,4));
				break;
			case EXPONENTIAL_SIGMOID_PARAM:
				pts.addColor(ofColor(ofColor::cyan,blink));
				pts.addVertex(ofVec2f(xx + doubleExpSigmoidParam * size, yy + size * 0.5));
				ofDrawBitmapString("ES", pts.getVertices()[0] + ofVec2f(5,4));
				break;
			case BOUNCE_IN_CUSTOM:
			case BOUNCE_OUT_CUSTOM:
				pts.addColor(ofColor(ofColor::orange,blink));
				pts.addVertex(ofVec2f(xx + (bounceNumB / (BOUNCES_MAX-1)) * size, yy + size));
				pts.addColor(ofColor(ofColor::purple,blink));
				pts.addVertex(ofVec2f(xx , yy + size -bounceElast * size));
				ofDrawBitmapString("B1", pts.getVertices()[0] + ofVec2f(5,4));
				ofDrawBitmapString("BE", pts.getVertices()[1] + ofVec2f(5,4));
				break;
		}
		pts.draw();
	}

	ofPopStyle();
#endif
}



inline void ofxAnimatable::fillInParams(float &p1, float &p2, float &p3, float &p4,
										float **pa1, float **pa2){

	switch (*curveStylePtr_) { //in case our curve has extra params, fill them in
		case QUADRATIC_BEZIER_PARAM:
			p1 = quadraticBezierParamAx;
			p2 = quadraticBezierParamAy;
			break;
		case EXPONENTIAL_SIGMOID_PARAM:
			p1 = doubleExpSigmoidParam;
			break;

		case OBJECT_DROP:
			p1 = bounceAmp;
			break;

		case CUBIC_BEZIER_PARAM:
			p1 = cubicBezierParamAx;
			p2 = cubicBezierParamAy;
			p3 = cubicBezierParamBx;
			p4 = cubicBezierParamBy;
			break;

		case EASE_IN_ELASTIC:
		case EASE_OUT_ELASTIC:
		case EASE_IN_OUT_ELASTIC:
		case EASE_OUT_IN_ELASTIC:
			p1 = elasticGain;
			p2 = elasticFreq;
			p3 = elasticDecay;
			break;

		case EASE_IN_BACK:
		case EASE_OUT_BACK:
		case EASE_IN_OUT_BACK:
		case EASE_OUT_IN_BACK:
			p1 = easeBackOffset;
			break;

		case BOUNCE_IN_CUSTOM:
		case BOUNCE_OUT_CUSTOM:
			p1 = bounceNumB;
			p2 = bounceAcc;
			*pa1 = &bounceDuration[0];
			*pa2 = &bounceVelocity[0];
			break;

		default: break;
	}
}

float ofxAnimatable::calcCurveAt( float percent ){

	float p1, p2, p3, p4;
	float *pa1, *pa2;
	fillInParams(p1,p2,p3,p4, &pa1, &pa2);
	float r = calcCurveAt(percent, *curveStylePtr_, p1, p2, p3, p4, pa1, pa2);
	
	currentSpeed_ =  r - prevSpeed_; //this is very ghetto and should be done properly! TODO
	prevSpeed_ = r;
	return r;
}

float ofxAnimatable::calcCurveAt(float percent, AnimCurve type, float p1, float p2,
								 float p3, float p4,
								 float * pa1, float * pa2){

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

												///////////////////////////////////////// BACK

		case EASE_IN_BACK:
			r = easeInBack(percent, p1); break;

		case EASE_OUT_BACK:
			r = easeOutBack(percent, p1); break;

		case EASE_OUT_IN_BACK:
			r = easeOutInBack(percent, p1); break;

		case EASE_IN_OUT_BACK:
			r = easeInOutBack(percent, p1); break;

												///////////////////////////////////////// BOUNCE
		case EASE_IN_BOUNCE:
			r = easeInBounce(percent); break;

		case EASE_OUT_BOUNCE:
			r = easeOutBounce(percent); break;

		case EASE_IN_OUT_BOUNCE:
			r = easeInOutBounce(percent); break;

		case EASE_OUT_IN_BOUNCE:
			r = easeOutInBounce(percent); break;

												///////////////////////////////////////// ELASTIC
		case EASE_IN_ELASTIC:
			r = easeInElastic(percent, p1, p2, p3); break;

		case EASE_OUT_ELASTIC:
			r = easeOutElastic(percent, p1, p2, p3); break;

		case EASE_IN_OUT_ELASTIC:
			r = easeInOutElastic(percent, p1, p2, p3); break;

		case EASE_OUT_IN_ELASTIC:
			r = easeOutInElastic(percent, p1, p2, p3); break;

												///////////////////////////////////////// CUSTOM BOUNCE
		case BOUNCE_IN_CUSTOM:
			r = customBounce(1.0f - percent, p1, p2, pa1, pa2); break;

		case BOUNCE_OUT_CUSTOM:
			r = 1.0f - customBounce(percent, p1, p2, pa1, pa2); break;

												//////////////////////////////////////// SMOOTH STEP
		case SMOOTH_STEP:
			r = smoothStep(percent); break;

		case SMOOTHER_STEP:
			r = smootherStep(percent); break;

	}
	return r;
}

void ofxAnimatable::startAutoUpdate(){

	if(!autoUpdating){
		autoUpdating = true;
		ofAddListener(ofEvents().update, this, &ofxAnimatable::autoUpdate);
	}else{
		ofLogError("ofxAnimatable") << "startAutoUpdate() can't start, already autoUpdating";
	}
}

void ofxAnimatable::stopAutoUpdate(){
	if(autoUpdating){
		autoUpdating = false;
		ofRemoveListener(ofEvents().update, this, &ofxAnimatable::autoUpdate);
	}else{
		ofLogError("ofxAnimatable") << "stopAutoUpdate() can't stop, not autoUpdating";
	}
}

void ofxAnimatable::autoUpdate(ofEventArgs&){
	update(1.0f / ofGetTargetFrameRate());
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

			AnimationEvent args;
			args.repeatSyle = repeat_;
			args.direction = direction_;
			args.percentDone = percentDone_;
			args.direction = direction_;
			args.playCount = playcount_;
			args.who = this;
			
			switch (repeat_) {

				case PLAY_N_TIMES:
					if(playcount_ == desiredPlayCount - 1){
						ofNotifyEvent(animFinished, args, this);
					}else{
						playcount_++;
						animating_ = true;
						percentDone_ = 0.0f;
					}
					break;

				case PLAY_ONCE:
					ofNotifyEvent(animFinished, args, this);
					break;	//nothing to do;
					
				case LOOP_BACK_AND_FORTH:
					direction_ = -direction_;
					animating_ = true;
					args.direction = direction_;
					playcount_++;
					if(autoFlipCurve){
						if(isCurveInvertable(curveStyle_)){
							curveStyle_ = getInverseCurve(curveStyle_);
						}
					}
					ofNotifyEvent(animLooped, args, this);
					break;
					
				case LOOP:
					animating_ = true;
					percentDone_ = 0.0f;
					args.direction = direction_;
					playcount_++;
					ofNotifyEvent(animLooped, args, this);
					break;

				case LOOP_BACK_AND_FORTH_ONCE:
					if (playcount_ >= 1){	//time to stop
						//we are done
						ofNotifyEvent(animFinished, args, this);
					}else{	
						direction_ = -direction_;
						animating_ = true;
						playcount_++;
					}
					if(autoFlipCurve){
						if(isCurveInvertable(curveStyle_)){
							curveStyle_ = getInverseCurve(curveStyle_);
						}
					}
					args.direction = direction_;
					args.playCount = playcount_;
					ofNotifyEvent(animLooped, args, this);
					break;

				case LOOP_BACK_AND_FORTH_N_TIMES:
					if ((playcount_ + 1) >= desiredPlayCount * 2 && direction_ == -1){	//time to stop
						ofNotifyEvent(animFinished, args, this);
						//we are done
					}else{
						direction_ = -direction_;
						animating_ = true;
						playcount_++;
					}
					if(autoFlipCurve){
						if(isCurveInvertable(curveStyle_)){
							curveStyle_ = getInverseCurve(curveStyle_);
						}
					}
					args.direction = direction_;
					args.playCount = playcount_;
					ofNotifyEvent(animLooped, args, this);
					break;

			}
		}
	}
	lastDT_ = dt;
}

bool ofxAnimatable::isCurveInvertable(AnimCurve c){

	switch (c) {
		case EASE_IN: return true; /**/
		case EASE_OUT: return true;
		case QUADRATIC_EASE_IN: return true; /**/
		case QUADRATIC_EASE_OUT: return true;
		case EASE_IN_BOUNCE: return true; /**/
		case EASE_OUT_BOUNCE: return true;
		case EASE_IN_BACK: return true; /**/
		case EASE_OUT_BACK: return true;
		case EASE_IN_ELASTIC: return true; /**/
		case EASE_OUT_ELASTIC: return true;
	}
	return false;
}

AnimCurve ofxAnimatable::getInverseCurve(AnimCurve c){
	switch (c) {
		case EASE_IN: return EASE_OUT; /**/
		case EASE_OUT: return EASE_IN;
		case QUADRATIC_EASE_IN: return QUADRATIC_EASE_OUT; /**/
		case QUADRATIC_EASE_OUT: return QUADRATIC_EASE_IN;
		case EASE_IN_BOUNCE: return EASE_OUT_BOUNCE; /**/
		case EASE_OUT_BOUNCE: return EASE_IN_BOUNCE;
		case EASE_IN_BACK: return EASE_OUT_BACK; /**/
		case EASE_OUT_BACK: return EASE_IN_BACK;
		case EASE_IN_ELASTIC: return EASE_OUT_ELASTIC; /**/
		case EASE_OUT_ELASTIC: return EASE_IN_ELASTIC;
	}
	ofLogError("ofxAnimatable") << getCurveName(c) << " curve has no inverse!";
	return c;
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

void ofxAnimatable::setRepeatTimes(int n){
	desiredPlayCount = n;
}

void ofxAnimatable::setRepeatType( AnimRepeat repeat ){
	repeat_ = repeat;
}


void ofxAnimatable::setCurve( AnimCurve curveStyle){
	curveStyle_ = curveStyle;
}

void ofxAnimatable::setCurve( AnimCurve *newCurveStylePtr){
	curveStylePtr_ = newCurveStylePtr; //replacing the pointer, careful!
}

void ofxAnimatable::setAutoFlipCurve(bool autoF){
	autoFlipCurve = autoF;
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
