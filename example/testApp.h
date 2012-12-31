#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxAnimatableFloat.h"
#include "ofxAnimatableOfPoint.h"
#include "ofxAnimatableOfColor.h"


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void mousePressed(int x, int y, int button);

		void drawPlot(int x, int y, int size, AnimCurve curve, string title);
	
	
		float position;
	
		ofxAnimatableFloat pos[NUM_ANIM_CURVES];
		string curveNames[NUM_ANIM_CURVES];
	
		ofxAnimatableFloat ball;
	
		ofxAnimatableOfColor	colorAnim;
		ofxAnimatableOfPoint	pointAnim;

		float width;
		float fr;
};

#endif
