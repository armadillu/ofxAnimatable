Set of classes that to set up very simple animations, with curves, repeat modes and scheduling. As if now, you can animate ofPoints, ofColors and floats.

Let's see how to make an ofPoint animate indefinitely between two positions, with an EaseInEaseOut curve.

	ofxAnimatableOfPoint	pointAnim;

	setup(){		
	
		//set initial position
		pointAnim.setPosition( ofPoint(0.0f,0.0f) );

		//set repeating style
 		pointAnim.setRepeatType(LOOP_BACK_AND_FORTH);

		//set animation curve
		pointAnim.setCurve(EASE_IN_EASE_OUT);

		//start the animation, go from current pos to the specified point
		pointAnim.animateTo( ofPoint(100.0f, 100.0f) );

	}

	update(){

		//update with App timebase (60 fps)
		pointAnim.update( 1.0f/60.0f );
	}

	draw(){

		pointAnim.draw();	
	}

Includes basic tweening curves, and customizable ones found [here](http://www.flong.com/texts/code/shapers_exp/); also includes Google's [Swift Out](http://www.google.com/design/spec/animation/authentic-motion.html#authentic-motion-mass-weight).

![ofxAnimatable screenshot](https://farm4.staticflickr.com/3882/14733332819_0042c08fe9_z_d.jpg)

see [big](https://farm4.staticflickr.com/3882/14733332819_c4af5c87a1_o_d.png)

