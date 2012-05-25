/*
 *  ofxAnimatableOfColor.h
 *  Cocoa Test
 *
 *  Created by Oriol Ferrer Mesià on 28/10/09.
 *  Copyright 2009 uri.cat. All rights reserved.
 *
 */

#ifndef _ANIMATABLE_COLOR_INC
#define _ANIMATABLE_COLOR_INC

#include "ofxAnimatable.h"
#include "ofMain.h"


class ofxAnimatableOfColor : public ofxAnimatable {
	
	public:

		ofxAnimatableOfColor();
		~ofxAnimatableOfColor(){};
		
		void update(float dt);	
		void applyCurrentColor(); //apply current color
	
		// apply for immediate effect ////////////////////////////////////////////////////////////////////
	
		void setColor( ofColor newColor );
		void revertToBaseColor();
		void setAlphaOnly( float a );
	
		// animated over time ///////////////////////////////////////////////////////////////////////////

			///starts new fade to "col" regardless, using the current color as the inital value
		void animateTo( ofColor col );
		void animateToAfterDelay( ofColor newColor, float delay );
		void animateToIfFinished( ofColor col );
		void fadeIn();
		void fadeOut();
		void animateToAlpha( float a );
		void startBlinking(float blinkDuration = 1.0f);			///will set repeat to LOOP_BACK_AND_FORTH, curve to EASE_IN_EASE_OUT
	
		//gets
		ofColor getCurrentColor();
		ofColor getTargetColor(){ return targetColor_;}
		ofColor getOriginalColor(){ return originalColor_;}
	

		  
	private:

		void startAfterWait();
	
		ofColor 		originalColor_;
		ofColor 		targetColor_;
		ofColor 		targetTempColor_;
	
};

			
#endif