#pragma once

#include "ofMain.h"
#include "ofxRemoteUIServer.h"
#include "ofxAnimatableFloat.h"
#include "ofxAnimatableOfPoint.h"
#include "ofxAnimatableOfColor.h"


class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	// APP SETUP ////////////////////////////////////////////

	void drawPlot(int x, int y, int size, AnimCurve curve, string title, ofColor);


	ofxAnimatableFloat pos[NUM_ANIM_CURVES];
	string curveNames[NUM_ANIM_CURVES];

	float width;
	float fr;

	float quadBezierA;
	float quadBezierB;

	float expSigmoidSteep;

	float cubicBezAx;
	float cubicBezAy;
	float cubicBezBx;
	float cubicBezBy;

	float cubicBez2Ax;
	float cubicBez2Ay;
	float cubicBez2Bx;
	float cubicBez2By;

	float cubicBezCamelAx;
	float cubicBezCamelAy;
	float cubicBezCamelBx;
	float cubicBezCamelBy;

	float dropBounceHeight;

	float elasticG;
	float elasticFreq;
	float elasticDecay;

	float easeOutOffset;

	int bounceNum;
	float bounceElast;




};
