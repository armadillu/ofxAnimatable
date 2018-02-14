#include "ofApp.h"

int floorLine = 630;
int xMargin = 0;
int widthCol = 60;

void ofApp::setup(){

	ofBackground(22);

	RUI_SETUP();

	// PARAMS
	RUI_NEW_GROUP("PARAMS");

	RUI_NEW_GROUP("OBJ DROP");
	dropBounceHeight = 0.21;
	RUI_SHARE_PARAM(dropBounceHeight, 0, 1);

	RUI_NEW_GROUP("QUADRATIC BEZIER");
	quadBezierA = 0.87; quadBezierB = 0.18;
	RUI_SHARE_PARAM(quadBezierA, 0, 1);
	RUI_SHARE_PARAM(quadBezierB, 0, 1);

	RUI_NEW_GROUP("EXP SIGMOID");
	expSigmoidSteep = 0.65;
	RUI_SHARE_PARAM(expSigmoidSteep, 0, 1);

	RUI_NEW_GROUP("CUBIC BEZIER");
	cubicBezAx = 0.19; cubicBezAy = 0.91;
	cubicBezBx = 0.97; cubicBezBy = 0.2;
	RUI_SHARE_PARAM(cubicBezAx, -0.5, 2);
	RUI_SHARE_PARAM(cubicBezAy, -0.5, 2);
	RUI_NEW_COLOR();
	RUI_SHARE_PARAM(cubicBezBx, -0.5, 2);
	RUI_SHARE_PARAM(cubicBezBy, -0.5, 2);

	RUI_NEW_GROUP("CUBIC BEZIER 2");
	cubicBez2Ax = 0.36; cubicBez2Ay = 0.22;
	cubicBez2Bx = 0.55; cubicBez2By = 0.81;
	RUI_SHARE_PARAM(cubicBez2Ax, -0.5, 2);
	RUI_SHARE_PARAM(cubicBez2Ay, -0.5, 2);
	RUI_NEW_COLOR();
	RUI_SHARE_PARAM(cubicBez2Bx, -0.5, 2);
	RUI_SHARE_PARAM(cubicBez2By, -0.5, 2);

	RUI_NEW_GROUP("CUBIC BEZIER CAMEL");
	cubicBezCamelAx = 0.36; cubicBezCamelAy = 0.22;
	cubicBezCamelBx = 0.55; cubicBezCamelBy = 0.81;
	RUI_SHARE_PARAM(cubicBezCamelAx, -0.5, 2);
	RUI_SHARE_PARAM(cubicBezCamelAy, -0.5, 2);
	RUI_NEW_COLOR();
	RUI_SHARE_PARAM(cubicBezCamelBx, -0.5, 2);
	RUI_SHARE_PARAM(cubicBezCamelBy, -0.5, 2);

	RUI_NEW_GROUP("ELASTIC");
	elasticG = 0.26; elasticFreq = 0.52; elasticDecay = 0.08;
	RUI_SHARE_PARAM(elasticG, 0, 1);
	RUI_SHARE_PARAM(elasticFreq, 0, 1);
	RUI_SHARE_PARAM(elasticDecay, 0, 1);

	RUI_NEW_GROUP("EASE_OUT");
	easeOutOffset = -0.78;
	RUI_SHARE_PARAM(easeOutOffset, -4, 4);

	RUI_NEW_GROUP("BOUNCE");
	bounceNum = 4; bounceElast = 0.38;
	RUI_SHARE_PARAM(bounceNum, 0, 10);
	RUI_SHARE_PARAM(bounceElast, 0, 1);

	for ( int i = 0; i < NUM_ANIM_CURVES; i++ ){
		pos[i].animateFromTo( xMargin, xMargin + widthCol );
		pos[i].setDuration(2);
		pos[i].setRepeatType( LOOP_BACK_AND_FORTH );
		AnimCurve curve = (AnimCurve) (EASE_IN_EASE_OUT + i );
		pos[i].setCurve( curve );
		curveNames[i] = ofxAnimatable::getCurveName( curve );
	}
}


void ofApp::update(){

	float dt = 1./60.;


	for ( int i = 0; i < NUM_ANIM_CURVES; i++ ){
		pos[i].update( dt );
	}

	float t = ofGetFrameNum();
	//animate our parametric curves

	pos[QUADRATIC_BEZIER_PARAM].setQuadraticBezierParams(quadBezierA, quadBezierB);


	pos[EXPONENTIAL_SIGMOID_PARAM].setDoubleExpSigmoidParam( expSigmoidSteep );


	pos[CUBIC_BEZIER_PARAM].setCubicBezierParams(cubicBezAx, cubicBezAy, cubicBezBx, cubicBezBy);
	pos[CUBIC_BEZIER2_PARAM].setCubicBezierParams(cubicBez2Ax, cubicBez2Ay, cubicBez2Bx, cubicBez2By);

	pos[CUBIC_BEZIER_CAMEL_LIKE].setCubicBezierParams(cubicBezCamelAx, cubicBezCamelAy, cubicBezCamelBx, cubicBezCamelBy);


	pos[EASE_IN_ELASTIC].setElasticParams(elasticG, elasticFreq, elasticDecay);
	pos[EASE_OUT_ELASTIC].setElasticParams(elasticG, elasticFreq, elasticDecay);
	pos[EASE_IN_OUT_ELASTIC].setElasticParams(elasticG, elasticFreq, elasticDecay);
	pos[EASE_OUT_IN_ELASTIC].setElasticParams(elasticG, elasticFreq, elasticDecay);

	pos[OBJECT_DROP].setDropObjectParams(dropBounceHeight);

	pos[EASE_IN_BACK].setEaseBackOffset(easeOutOffset);
	pos[EASE_OUT_BACK].setEaseBackOffset(easeOutOffset);
	pos[EASE_IN_OUT_BACK].setEaseBackOffset(easeOutOffset);
	pos[EASE_OUT_IN_BACK].setEaseBackOffset(easeOutOffset);

	pos[BOUNCE_IN_CUSTOM].setCustomBounceParams(bounceNum, bounceElast);
	pos[BOUNCE_OUT_CUSTOM].setCustomBounceParams(bounceNum, bounceElast);
}


void ofApp::draw(){

	//plots
	int yy;;
	int xx = 30;
	int size = 70; //of each plot
	int rowPad = 40;
	int colPad = 35;
	int vOffset = 13;
	int numCol = (ofGetWidth() - xx) / (size + colPad);
	yy = numCol * vOffset;
	int x = 0;
	int row = 0;
	int col = 0;
	for ( int i = 0 ; i < NUM_ANIM_CURVES; i++ ){
		ofColor c = ofColor::red;
		c.setHsb(fmod(128 + 3 * 255.0f * i/float(NUM_ANIM_CURVES), 255), 255, 255);
		AnimCurve curve = (AnimCurve) (EASE_IN_EASE_OUT + i);
		string curveName = ofxAnimatable::getCurveName((AnimCurve)i);
		drawPlot( xx + x, yy + row * (size + rowPad) - vOffset * col, size, curve, curveName, c );
		x += size + colPad;
		col ++;
		if (  x > ofGetWidth() -  1.0f * size - xx){
			row++;
			col = 0;
			x = 0;
		}
	}
}


void ofApp::drawPlot(int x, int y, int size, AnimCurve curve, string title, ofColor c){

	pos[curve].drawCurve(x, y, size, true, c);
}


void ofApp::keyPressed(int key){
}


void ofApp::keyReleased(int key){

}


void ofApp::mouseMoved(int x, int y ){

}


void ofApp::mouseDragged(int x, int y, int button){

}


void ofApp::mousePressed(int x, int y, int button){

}


void ofApp::mouseReleased(int x, int y, int button){

}


void ofApp::windowResized(int w, int h){

}


void ofApp::gotMessage(ofMessage msg){

}


void ofApp::dragEvent(ofDragInfo dragInfo){
	
}

