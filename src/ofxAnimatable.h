/*
 *  ofxAnimatable.h
 *
 *  Created by Oriol Ferrer Mesià on 30/10/09.
 *  Copyright 2009 uri.cat. All rights reserved.
 *
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>

#define DEFAULT_ANIMATION_DURATION	1.0f


enum AnimRepeat{
	PLAY_ONCE = 0,
	LOOP, 
	LOOP_BACK_AND_FORTH, 
	LOOP_BACK_AND_FORTH_ONCE,
};

enum AnimCurve{
	EASE_IN_EASE_OUT = 0,
	EASE_IN, 
	EASE_OUT, 
	LINEAR,	
	LATE_LINEAR, 
	VERY_LATE_LINEAR, 
	BOUNCY,	//this needs work
	TANH, 
	SINH, 	
	EARLY_SQUARE, 
	SQUARE, 
	LATE_SQUARE, 		
	LATE_EASE_IN_EASE_OUT, 
	VERY_LATE_EASE_IN_EASE_OUT,
	QUADRATIC_EASE_IN,
	QUADRATIC_EASE_OUT,
	NUM_ANIM_CURVES //leave that on the last to see how many we have
};


class ofxAnimatable{
	
	public:

		void setup();
		void update(float dt);

		void pause();					//really needed?
		void resume();					//

		void setCurve( AnimCurve curveStyle_ );
		void setRepeatType( AnimRepeat repeat );
		void setDuration( float seconds );
	
		float getDuration(){ return 1.0f/transitionSpeed_; }

		float getPercentDone();			///get how much of the animation has been "walked"
		bool isAnimating();				///is the animation still going on?
		bool hasFinishedAnimating();	///has the animation finished?
		bool isWaitingForAnimationToStart();	///an animation has been scheduled with "animateToAfterDelay"
		bool isOrWillBeAnimating();		/// object is either animating now or it's waiting to be launch animation after a delay
		float getCurrentSpeed(){ return fabs( direction_ * currentSpeed_ / (lastDT_ * transitionSpeed_)) ;} ///as a percentage of linear speed
	
		static std::string getCurveName(AnimCurve c);

		virtual ~ofxAnimatable(void) {}
		ofxAnimatable() {}


	protected:

		bool		animating_;
		bool		paused_;
	
		int 		playcount_;
	
		float		transitionSpeed_;	///this is 1/N (N == # of updates) it will take for the transition to end
		float		percentDone_;		/// [0..1]
			
	
		float		delay_;
		AnimRepeat 	repeat_;
		AnimCurve	curveStyle_;
	
		int			direction_;  // 1 : forward,   -1 : backward
	
		float calcCurveAt( float percent );

		void startAnimation();			///Used by subclasses to indicate we are starting an anim
		void startAnimationAfterDelay(float delay);
		void reset();					///Used by subclasses to indicate a reset of an animation

	private:
	
		virtual void startAfterWait() = 0;
		float currentSpeed_;
		float prevSpeed_;
		float lastDT_;
};

