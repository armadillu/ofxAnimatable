# ofxAnimatable

[![Build Status](https://travis-ci.org/armadillu/ofxAnimatable.svg?branch=master)](https://travis-ci.org/armadillu/ofxAnimatable)
[![Build status](https://ci.appveyor.com/api/projects/status/9ba7ocv3jcre1k0v/branch/master?svg=true)](https://ci.appveyor.com/project/armadillu/ofxanimatable/branch/master)

Set up very simple animations, with curves, repeat modes and basic scheduling.

Let's see how to make a value animate indefinitely between two values, with an EaseInEaseOut curve.

	ofxAnimatableFloat	anim;

	setup(){		
	
		//set initial position
		anim.reset( 0.0f );

		//set repeating style
 		anim.setRepeatType(LOOP_BACK_AND_FORTH);

		//set animation curve
		anim.setCurve(EASE_IN_EASE_OUT);

		//start the animation, go from current val to the specified val
		anim.animateTo( 1.0f );

	}

	update(){
		//update with App timebase (60 fps)
		pointAnim.update( 1.0f/60.0f );
	}


Includes basic tweening curves, and customizable ones found [here](http://www.flong.com/texts/code/shapers_exp/) and [here](http://stackoverflow.com/questions/5161465/how-to-create-custom-easing-function-with-core-animation); also includes Google's [Swift Out](http://www.google.com/design/spec/animation/authentic-motion.html#authentic-motion-mass-weight) curve.

![ofxAnimatable screenshot](https://farm8.staticflickr.com/7298/16217582178_121402bd03_z_d.jpg)

see [big](https://farm8.staticflickr.com/7298/16217582178_4e5789e8cb_o_d.png)


##LICENSE
ofxAnimatable is made available under the [MIT](http://opensource.org/licenses/MIT) license.
