#include "testApp.h"

int floorLine = 750;
int xMargin = 0;
int widthCol = 60;

//--------------------------------------------------------------
void testApp::setup(){

	
	ofBackground(44);
	ofSetFrameRate(60);
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	ofSetVerticalSync(true);
#ifdef TIME_SAMPLE
	TIME_SAMPLE_SET_FRAMERATE(60);
	TIME_SAMPLE_SET_PRECISION(3);
	TIME_SAMPLE_SET_AVERAGE_RATE(0.01);
	TIME_SAMPLE_SET_DRAW_LOCATION(TIME_MEASUREMENTS_BOTTOM_LEFT);
#endif
	width = 10;

	ofxAnimatableFloat f;
	f.animateFromTo( xMargin, xMargin + widthCol );
	f.setDuration(2);	
	f.setRepeatType( LOOP_BACK_AND_FORTH );
	
	for ( int i = 0; i < NUM_ANIM_CURVES; i++ ){
		pos[i] = f;
		AnimCurve curve = (AnimCurve) (EASE_IN_EASE_OUT + i );
		pos[i].setCurve( curve );
		curveNames[i] = ofxAnimatable::getCurveName( curve );
	}

	ball.reset(floorLine - 100);
	ball.setCurve(EASE_IN);
	ball.setRepeatType(LOOP_BACK_AND_FORTH);
	ball.setDuration(0.55);
	ball.animateTo( floorLine );
	
	colorAnim.setColor( ofColor::black );
	colorAnim.setDuration( 0.5f );
	colorAnim.setRepeatType(LOOP_BACK_AND_FORTH);
	colorAnim.setCurve(LINEAR);
	colorAnim.animateTo( ofColor::white );
	
	pointAnim.setPosition( ofPoint(100,100) );
	pointAnim.setRepeatType(PLAY_ONCE);
	pointAnim.setCurve(EASE_IN_EASE_OUT);

	//img.loadImage("fitted_curve.png");
}

//--------------------------------------------------------------
void testApp::update(){

	//app timebase, to send to all animatable objets
	float dt = 1.0f / ofGetFrameRate();
	
	for ( int i = 0; i < NUM_ANIM_CURVES; i++ )
		pos[i].update( dt );
	
	ball.update( dt );
	
	colorAnim.update( dt );
	pointAnim.update( dt );

	if ( !pointAnim.isOrWillBeAnimating() ){
		pointAnim.animateToAfterDelay( ofPoint( ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight())), 0.5);
	}

	float t = ofGetFrameNum();
	//animate our parametric curves
	float a = 0.5 + 0.5 * sin( 0.06 * t);
	float b = 0.5 + 0.5 * sin( 0.04 * t + 400);
	pos[QUADRATIC_BEZIER_PARAM].setQuadraticBezierParams( a, b);

	float steep = 0.5 + 0.5 * sin( 0.1 * t);
	pos[EXPONENTIAL_SIGMOID_PARAM].setDoubleExpSigmoidParam( steep );

	a = 0.5 + 0.5 * sin( 0.05 * t);
	b = 0.5 + 0.5 * sin( -0.03 * t + 3);
	float c = 0.5 + 0.5 * sin( 0.04 * t + 6.4);
	float d = 0.5 + 0.5 * sin( 0.06 * t + 44);
	pos[CUBIC_BEZIER_PARAM].setCubicBezierParams(a, b, c, d);

	//benchamrk the curves individually, report through ofxTimeMeasurements
	#ifdef TIME_SAMPLE
	for ( int i = 0 ; i < NUM_ANIM_CURVES; i++ ){
		AnimCurve curve = (AnimCurve) (EASE_IN_EASE_OUT + i);
		string curveName = ofxAnimatable::getCurveName(curve);
		TS_START(curveName);
		for(int k = 0; k < 5000; k++){
			ofxAnimatable::calcCurveAt(0.5f, curve, 0.5f, 0.5f, 0.5f, 0.5f);
		}
		TS_STOP(curveName);
	}
	#endif

}

//--------------------------------------------------------------
void testApp::draw(){

	//ghetto test for cuadratic bezier curve params
//	float ss = 160.0;
//	float offx = 10;
//	if (!ofGetKeyPressed()){
//		a = (ofGetMouseX() - offx)/ss;
//		b = (ofGetMouseY() - floorLine + ss)/ss;
//	}else{
//		c = (ofGetMouseX() - offx)/ss;
//		d = (ofGetMouseY() - floorLine + ss)/ss;
//	}
//	pos[CUBIC_BEZIER_PARAM].setCubicBezierParams(a, b, c, d);
//
//	ofPushMatrix();
//	ofTranslate(offx, floorLine - ss);
//	ofSetColor(255);
//	img.draw(0,0);
//	pos[CUBIC_BEZIER_PARAM].drawCurve(0, 0, ss, false, ofColor::red);
//	ofDrawBitmapString("a:" + ofToString(a,3) +
//					   "  b:" + ofToString(b,3) +
//					   "\nc:" + ofToString(c,3) +
//					   "  d:" + ofToString(d,3), 0, ss + 30);
//	ofSetColor(255,0,0);
//	ofCircle(ss * a, ss * b, 3);
//	ofSetColor(0,255,0);
//	ofCircle(ss * c, ss * d, 3);
//	ofPopMatrix();

	int vOff = 20;
	for ( int i = 0 ; i < NUM_ANIM_CURVES; i++ ){
		float lineHeight = 1.7f;
		float yy = vOff + i * lineHeight * width;
		glColor4ub(255,255,255,64);
		ofLine(xMargin, yy + width * 0.5, xMargin + widthCol + width, yy + width * 0.5);
		glColor4ub(255,255,255,255);
		ofRect( pos[i].val(), yy, width, width);
		ofDrawBitmapString( curveNames[i] + " vel: " + ofToString( pos[i].getCurrentSpeed(), 1), xMargin + widthCol + 20, yy + 10);
	}

	//ball and floor
	colorAnim.applyCurrentColor();
	ofCircle( ( 2 * ofGetFrameNum() )%ofGetWidth(), ball.val(), width);
	
	glColor4ub(255,255,255,255);
	ofRect(0, floorLine + width, ofGetWidth(), 1);

	//vertical lines
	ofRect(xMargin, 0, 1, floorLine + width);
	ofRect(xMargin + widthCol + width, 0, 1, floorLine + width);
	
	glColor4f( pointAnim.getPercentDone(), 1 - pointAnim.getPercentDone() , 0, 1);
	glPointSize(10);
	pointAnim.draw();

	glColor4ub(255,255,255,255);
	ofDrawBitmapString( ofToString( ofGetFrameRate()),  10, 10);
	
	int c = 0;
	int size = 100;
	int yy = size;
	int rowHeight = 2.4 * size ;
	int xx = 400;
	int off = size/3.0f;
	int x = 0;
	int row = 0;
	for ( int i = 0 ; i < NUM_ANIM_CURVES; i++ ){
		string curveName = ofxAnimatable::getCurveName((AnimCurve)i);
		AnimCurve curve = (AnimCurve) (EASE_IN_EASE_OUT + i);
		drawPlot( xx + x, yy + row * rowHeight - 15 * i, size, curve, ofxAnimatable::getCurveName(curve) );
		x += (size + off);
		if (  x > ofGetWidth() -  1.0f * size - xx){
			row++;
			x = 0;
		}
	}
}

void testApp::drawPlot(int x, int y, int size, AnimCurve curve, string title){

	ofxAnimatableFloat a = pos[curve];
	a.setCurve(curve);
	a.setDuration(1);
	a.reset(0);
	a.animateTo(1);
	a.drawCurve(x, y, size);
}



//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	pointAnim.animateTo( ofPoint(x, y) );
}

