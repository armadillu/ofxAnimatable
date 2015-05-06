/*
 *  ofxAnimatable.h
 *
 *  Created by Oriol Ferrer Mesià on 30/10/09.
 *  Copyright 2009 uri.cat. All rights reserved.
 *
 */


#define DEFAULT_ANIMATION_DURATION	1.0f

#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
using namespace std;

#pragma once
#include "ofMain.h"

#define B0F(t) ((1.0f - t) * (1.0f - t) * (1.0f - t))
#define B1F(t) (3.0f * t * ( 1.0f - t) * (1.0f - t))
#define B2F(t) (3.0f * t * t * (1.0f - t))
#define B3F(t) (t * t * t)
#define slopeFromT(t,A,B,C)  (1.0f / (3.0f * A * t * t + 2.0f * B * t + C))
#define xFromT(t, A, B, C, D) (A * (t * t * t) + B * (t * t) + C * t + D)
#define yFromT(t, E, F, G, H) (E * (t * t * t) + F * (t * t) + G * t + H)

#define BOUNCES_MAX	20

enum AnimRepeat{
	PLAY_ONCE = 0,
	LOOP, 
	LOOP_BACK_AND_FORTH, 
	LOOP_BACK_AND_FORTH_ONCE,
	PLAY_N_TIMES,
	LOOP_BACK_AND_FORTH_N_TIMES
};

enum AnimCurve{
	EASE_IN_EASE_OUT = 0,
	EASE_IN, 
	EASE_OUT, 
	LINEAR,
	EARLY_LINEAR,
	LATE_LINEAR,
	VERY_LATE_LINEAR, 
	BOUNCY,	//this needs work TODO
	OBJECT_DROP,
	TANH,
	SINH, 	
	EARLY_SQUARE, 
	SQUARE,
	LATE_SQUARE, //13

	BLINK_5,
	BLINK_3,
	BLINK_2,
	BLINK_AND_FADE_1,
	BLINK_AND_FADE_2,
	BLINK_AND_FADE_3,

	LATE_EASE_IN_EASE_OUT, //20
	VERY_LATE_EASE_IN_EASE_OUT,
	QUADRATIC_EASE_IN,
	QUADRATIC_EASE_OUT,
	EARLY_QUADRATIC_EASE_OUT,

	QUADRATIC_BEZIER_PARAM, //http://www.flong.com/texts/code/shapers_exp/

	CUBIC_BEZIER_PARAM,

	EXPONENTIAL_SIGMOID_PARAM,

	SWIFT_GOOGLE,

	EASE_IN_BOUNCE,		//28
	EASE_OUT_BOUNCE,
	EASE_IN_OUT_BOUNCE,
	EASE_OUT_IN_BOUNCE,

	EASE_IN_BACK,
	EASE_OUT_BACK,
	EASE_IN_OUT_BACK,
	EASE_OUT_IN_BACK,

	EASE_IN_ELASTIC,
	EASE_OUT_ELASTIC,
	EASE_IN_OUT_ELASTIC,
	EASE_OUT_IN_ELASTIC,

	BOUNCE_IN_CUSTOM,
	BOUNCE_OUT_CUSTOM,

	SMOOTH_STEP, //http://en.wikipedia.org/wiki/Smoothstep
	SMOOTHER_STEP,

	NUM_ANIM_CURVES //leave that on the last to see how many we have
};


class ofxAnimatable{
	
	public:

		ofxAnimatable();
		virtual ~ofxAnimatable();

		virtual void setup();
		virtual void update(float dt);

		void autoUpdate(ofEventArgs&); //to by called by autoUpdate, dont call directly

		void startAutoUpdate();
		void stopAutoUpdate();

		void pause();					//really needed?
		void resume();					//

		void setCurve( AnimCurve curveStyle_ );
		void setCurve( AnimCurve* curveStyle_ );	//use this if you want to change
													//anim curve styles on the fly.
													//just make sure the poitner remains valid!
	
		void setRepeatType( AnimRepeat repeat );
		void setRepeatTimes(int times); //you must set setRepeatType to PLAY_N_TIMES or LOOP_BACK_AND_FORTH_N_TIMES
		void setDuration( float seconds );
		void setAutoFlipCurve(bool autoF);//when repeat set to back and forth, and autoFlip==true it will auto invert your curve when looping (ease_in > ease_out and so on)

		void setDoubleExpSigmoidParam(float param){doubleExpSigmoidParam = param;} //only for QUADRATIC_BEZIER_PARAM curve
		void setQuadraticBezierParams(float a, float b){quadraticBezierParamAx = a; quadraticBezierParamAy = b; } //only for EXPONENTIAL_SIGMOID_PARAM curve
		void setDropObjectParams(float bounceHeightPercent){bounceAmp = bounceHeightPercent;} //only for DROP_OBJECT curve
		void setCubicBezierParams(float a, float b, float c, float d){cubicBezierParamAx = a; cubicBezierParamAy = b; cubicBezierParamBx = c; cubicBezierParamBy = d; } //only for EXPONENTIAL_SIGMOID_PARAM curve

		//elastic decay should not be < 0!
		void setElasticParams(float gain, float freq, float decay){elasticGain = 1.0f / gain; elasticFreq = 1.0f / freq; elasticDecay = decay - 1.0f;}
		void setEaseBackOffset(float offset){easeBackOffset = offset;}

		void setCustomBounceParams(int bounceNum, float bounceElast);
		float getDuration(){ return 1.0f/transitionSpeed_; }

		int getPlayCount(){return playcount_;}; ///returns 2x what the user requested when LOOP_BACK_AND_FORTH - when using PLAY_N_TIMES
		int getDesiredPlayCount(){return desiredPlayCount;}; ///when using PLAY_N_TIMES

		float getPercentDone();			///get how much of the animation has been "walked"
		void setPercentDone(float p);			//Will allow to skip to any point of animation. use carefully
		bool isAnimating();				///is the animation still going on?
		bool hasFinishedAnimating();	///has the animation finished?
		bool isWaitingForAnimationToStart();	///an animation has been scheduled with "animateToAfterDelay"
		bool isOrWillBeAnimating();		/// object is either animating now or it's waiting to be launch animation after a delay

		float getCurrentSpeed(); ///as a percentage of linear speed
		float timeLeftForAnimationToStart(){ return delay_; }
		float waitTimeLeftPercent(){ return 1.0f - delay_ / waitTime_; }
	
		static string getCurveName(AnimCurve c);
		static AnimCurve getCurveFromName(const string& name);

		static bool isCurveInvertable(AnimCurve c); //easeIn becomes easeOut, etc
		static AnimCurve getInverseCurve(AnimCurve c); //does this curve have an inverse?

		//carefull with those, you'd better know what you are doing, those should almost be protected
		//exposing this to get direct access to simple curve values
		float calcCurveAt( float percent );
		void drawCurve(int x, int y, int size, bool bg = false, ofColor c = ofColor(255,0,128));

		static float calcCurveAt(float percent, AnimCurve type,
								 float param1 = 0.5, float param2 = 0.5,
								 float param3 = 0.5, float param4 = 0.5,
								 float *pa1 = NULL, float *pa2 = NULL);

		//we need a custom step when assigning ofxAnimatable objs
		//otherwise the curve* of the 2nd will point to the curve of the 1st
		ofxAnimatable& operator=(const ofxAnimatable& other);

	struct AnimationEvent{
			float percentDone;
			AnimRepeat repeatSyle;
			float direction;
			int playCount;
			ofxAnimatable * who;
		};

		ofEvent<AnimationEvent> animFinished;
		ofEvent<AnimationEvent> animLooped;

	protected:

		bool		animating_;
		bool		paused_;
	
		int 		playcount_;
		int			desiredPlayCount;
	
		float		transitionSpeed_;	///this is 1/N (N == # of updates) it will take for the transition to end
		float		percentDone_;		/// [0..1]
			
	
		float		delay_;		//countdown timer that stores delay when startAnimationAfterDelay() is used
		float		waitTime_;	//original wait delay_

		AnimRepeat 	repeat_;
		bool		autoFlipCurve; //when set to back and forth, will auto invert your curve when looping (ease_in > ease_out)
		AnimCurve	curveStyle_;
		AnimCurve	*curveStylePtr_;	//by default points to curveStyle_
										//we always use the pointer!
	
		int			direction_;  // 1 : forward,   -1 : backward
	


		void startAnimation();			///Used by subclasses to indicate we are starting an anim
		void startAnimationAfterDelay(float delay);
		void reset();					///Used by subclasses to indicate a reset of an animation
		inline void fillInParams(float&p1, float &p2, float &p3, float &p4,
								 float ** pa1, float ** pa2);

		bool autoUpdating;

	private:

		void initCustomBounce();

		virtual void startAfterWait() = 0;
		float currentSpeed_;
		float prevSpeed_;
		float lastDT_;

		//for some of the curves
		float doubleExpSigmoidParam;
		float quadraticBezierParamAx, quadraticBezierParamAy;
		float bounceAmp;
		float cubicBezierParamAx, cubicBezierParamAy, cubicBezierParamBx, cubicBezierParamBy;
		float elasticGain, elasticFreq, elasticDecay;
		float easeBackOffset;
		float bounceNumB, bounceElast; //params

		//storing intermediate states for bounce
		float bounceAcc;
		float bounceDuration[BOUNCES_MAX];
		float bounceVelocity[BOUNCES_MAX];
};

	