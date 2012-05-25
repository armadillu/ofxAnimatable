/*
 *  ofxAnimatableOfPoint.cpp
 *  Cocoa Test
 *
 *  Created by Oriol Ferrer Mesià on 28/10/09.
 *  Copyright 2009 uri.cat. All rights reserved.
 *
 */

#include "ofxAnimatableOfPoint.h"


ofxAnimatableOfPoint::ofxAnimatableOfPoint(){

	ofxAnimatable::setup();	//defaults to (0,0,0) >> (0,0,0)
}


void ofxAnimatableOfPoint::update(float dt){

	ofxAnimatable::update( dt );
}


void ofxAnimatableOfPoint::draw(){
	
	ofPushMatrix();
		ofPoint p = getCurrentPosition();
		ofTranslate(p.x, p.y, p.z);
		float s = 10.0f;
		ofRect( -s * 0.5f, -s * 0.5f, s, s );
	ofPopMatrix();
	
}


void ofxAnimatableOfPoint::setPosition( ofPoint newPos ){
	
	ofxAnimatable::reset();
	originalPoint_ = newPos;
	targetPoint_ = newPos;
}


void ofxAnimatableOfPoint::setPositionX( float newX ){
	
	originalPoint_ = getCurrentPosition();
	originalPoint_.x = newX;
	targetPoint_ = originalPoint_;
	ofxAnimatable::reset();
}


void ofxAnimatableOfPoint::setPositionY( float newY ){
	
	originalPoint_ = getCurrentPosition();
	originalPoint_.y = newY;
	targetPoint_ = originalPoint_;
	ofxAnimatable::reset();
}


void ofxAnimatableOfPoint::setPositionZ( float newZ ){
	
	originalPoint_ = getCurrentPosition();
	originalPoint_.z = newZ;
	targetPoint_ = originalPoint_;
	ofxAnimatable::reset();
}


void ofxAnimatableOfPoint::reset(){

	ofxAnimatable::reset();
	targetPoint_ = originalPoint_;
}


void ofxAnimatableOfPoint::animateTo( ofPoint where ){

	originalPoint_ = getCurrentPosition();
	targetPoint_ = where;
	ofxAnimatable::startAnimation();
}

void ofxAnimatableOfPoint::animateToAfterDelay( ofPoint where, float delay ){
	
	if (delay <= 0.0f){
		animateTo(where);
	}else{
		//originalTempPoint_ = getCurrentPosition();
		targetTempPoint_ = where;
		ofxAnimatable::startAnimationAfterDelay(delay);
	}
}


void ofxAnimatableOfPoint::animateToIfFinished( ofPoint where ){

	if (animating_ == false){
		animateTo(where);
	}
}


ofPoint ofxAnimatableOfPoint::getCurrentPosition(){
	float mappedDistribution = calcCurveAt(percentDone_);	///percentDone_ is [0..1] & tells me where we are between orig and target
	return originalPoint_ + ( targetPoint_ - originalPoint_) * mappedDistribution ;
}


void ofxAnimatableOfPoint::startAfterWait(){
	animateTo(targetTempPoint_);
}
