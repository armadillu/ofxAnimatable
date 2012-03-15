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


void ofxAnimatableOfColor::setColor(){
	ofSetColor(getCurrentColor());
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


void ofxAnimatableOfColor::startBlinking(){

	setRepeatType(LOOP_BACK_AND_FORTH);
	setCurve(EASE_IN_EASE_OUT);
	setAlphaOnly(0.0f);
	ofColor c = getCurrentColor();
	c.a = 1.0f;
	animateTo( c );
}


ofColor ofxAnimatableOfColor::getCurrentColor(){

	float mappedDistribution = calcCurveAt(percentDone_);	///percentDone_ is [0..1] & tells me where we are between orig and target
	int newC[4];
	for (int i = 0; i < 4; i++){
		newC[i] = ( (int)targetColor_.r - (int)originalColor_.r) * mappedDistribution;
	}
	return originalColor_ + ofColor( newC[0], newC[1], newC[2], newC[3] );
}


void ofxAnimatableOfColor::swapOriginDestination(){
	
	ofColor tempVal = originalColor_;
	originalColor_ = targetColor_;
	targetColor_ = tempVal;
}

void ofxAnimatableOfColor::startAfterWait(){
	animateTo(targetTempColor_);
}
