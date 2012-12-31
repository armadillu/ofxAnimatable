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



![ofxAnimatable screenshot](http://farm9.staticflickr.com/8083/8329975610_ab506dddcb_z.jpg)

