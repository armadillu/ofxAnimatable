/*
 *  ofxAnimatableOfPoint.h
 *  Cocoa Test
 *
 *  Created by Oriol Ferrer Mesià on 28/10/09.
 *  Copyright 2009 uri.cat. All rights reserved.
 *
 */

#ifndef _ANIMATABLE_POINT_INC
#define _ANIMATABLE_POINT_INC

#include "ofxAnimatable.h"
#include "ofMain.h"


class ofxAnimatableOfPoint : public ofxAnimatable{
	
	public:

		ofxAnimatableOfPoint();
		~ofxAnimatableOfPoint(){};
		
		void update(float dt);	
		void draw();			//draws the current pos point in space
	

		// apply for immediate effect ////////////////////////////////////////////////////////////////////
	
		void setPosition( ofPoint );
		void setPositionX(float);
		void setPositionY(float);
		void setPositionZ(float);
		void reset();		
	
		// animated over time ///////////////////////////////////////////////////////////////////////////

		///starts new animation to "where" regardless, using the current pos as the inital value
		void animateTo( ofPoint where );
		void animateToAfterDelay( ofPoint where, float delay );

		void animateToIfFinished( ofPoint where );
		
		//gets
		ofPoint getCurrentPosition();
		ofPoint getTargetPosition(){ return targetPoint_;}
		ofPoint getOriginalPosition(){ return originalPoint_;}
	

	private:

		void startAfterWait();

		ofPoint 		originalPoint_;
		ofPoint 		targetPoint_;
		ofPoint 		targetTempPoint_;


};

			
#endif