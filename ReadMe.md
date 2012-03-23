Set of classes that to set up very simple animations, with curves, repeat modes and scheduling. As if now, you can animate ofPoints, ofColors and floats.

Let's see how to make an ofPoint animate indefinitely between two positions, with an EaseInEaseOut curve.

	ofxAnimatableOfPoint	pointAnim;

	setup(){		
	
		//set initial position
		pointAnim.setPosition( ofPoint(0,0) );

		//set repeating style
 		pointAnim.setRepeatType(ofxAnimatable::LOOP_BACK_AND_FORTH);

		//set animation curve
		pointAnim.setCurve(ofxAnimatable::EASE_IN_EASE_OUT);

		//start the animation, go from current pos to the specified point
		pointAnim.animateTo( ofPoint(100,100) );

	}

	update(){
		//update with App timebase (60 fps)
		pointAnim.update( 1./60. );
	}

	draw(){
		//draw
		pointAnim.draw();	
	}



![ofxAnimatable screenshot](http://farm8.staticflickr.com/7214/6863126704_4ce47b2f56_z.jpg)

