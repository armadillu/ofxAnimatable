/*
 *  ofxAnimatableOfColor.cpp
 *  Cocoa Test
 *
 *  Created by Oriol Ferrer Mesià on 28/10/09.
 *  Copyright 2009 uri.cat. All rights reserved.
 *
 */

#include "ofxAnimatableOfColor.h"


ofxAnimatableOfColor::ofxAnimatableOfColor(){

	setup();	
	originalColor_ = ofColor::black;
	targetColor_ = ofColor::white;
}


void ofxAnimatableOfColor::update(float dt){
	ofxAnimatable::update( dt );
}


void ofxAnimatableOfColor::applyCurrentColor(){
	ofSetColor( getCurrentColor() );
}


void ofxAnimatableOfColor::setColor(ofColor newColor){
	ofxAnimatable::reset();
	originalColor_ = newColor;
	targetColor_ = newColor;
}


void ofxAnimatableOfColor::revertToBaseColor(){
	ofxAnimatable::reset();
}
	

void ofxAnimatableOfColor::setAlphaOnly( float a ){

	originalColor_ = getCurrentColor();
	originalColor_.a = a;
	targetColor_ = originalColor_;
	ofxAnimatable::reset();
}


void ofxAnimatableOfColor::animateTo( ofColor col ){

	originalColor_ = getCurrentColor();
	targetColor_ = col;
	ofxAnimatable::startAnimation();
}


void ofxAnimatableOfColor::animateToAfterDelay( ofColor newColor, float delay ){
	
	//originalTempColor_ = getCurrentColor();
	targetTempColor_ = newColor;
	ofxAnimatable::startAnimationAfterDelay(delay);
}


void ofxAnimatableOfColor::animateToIfFinished( ofColor col ){
	if (animating_ == false){
		animateTo( col );
	}
}


void ofxAnimatableOfColor::fadeIn(){

	ofColor targetC = getCurrentColor();
	
	if ( sizeof(targetC.r) == sizeof(float) )
		targetC.a = (float)1.0f;
	else if ( sizeof(targetC.r) == sizeof(unsigned char) )
		targetC.a = (unsigned char) numeric_limits<unsigned char>::max();
	else if ( sizeof(targetC.r) == sizeof(unsigned short) )
		targetC.a = (unsigned char) numeric_limits<unsigned short>::max();

	animateTo( targetC );	
}


void ofxAnimatableOfColor::fadeOut(){

	ofColor targetC = getCurrentColor();
	targetC.a = 0.0f;
	animateTo( targetC );
}


void ofxAnimatableOfColor::animateToAlpha( float a ){

	originalColor_ = getCurrentColor();
	targetColor_ = originalColor_;
	targetColor_.a = a;
	ofxAnimatable::startAnimation();	
}


void ofxAnimatableOfColor::startBlinking( float blinkDuration){

	setRepeatType(LOOP_BACK_AND_FORTH);
	setCurve(EASE_IN_EASE_OUT);
	setAlphaOnly(0.0f);
	setDuration( blinkDuration );
	ofColor c = getCurrentColor();
	c.a = 255;
	animateTo( c );
}


ofColor ofxAnimatableOfColor::getCurrentColor(){

	float mappedDistribution = calcCurveAt(percentDone_);	///percentDone_ is [0..1] & tells me where we are between orig and target
	float newC[4];
	ofColor r;
	for (int i = 0; i < 4; i++){
		newC[i] = ( (int)targetColor_[i] - (int)originalColor_[i]) * mappedDistribution;
		r[i] = originalColor_[i] + newC[i];
	}
	return r;
}

void ofxAnimatableOfColor::startAfterWait(){
	animateTo(targetTempColor_);
}


