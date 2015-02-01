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


	for ( int i = 0; i < NUM_ANIM_CURVES; i++ ){
		pos[i].animateFromTo( xMargin, xMargin + widthCol );
		pos[i].setDuration(2);
		pos[i].setRepeatType( LOOP_BACK_AND_FORTH );
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

	//app timebase, to send to all animatable objects
	float dt = 1.0f / 60.0f;
	
	for ( int i = 0; i < NUM_ANIM_CURVES; i++ ){
		pos[i].update( dt );
	}
	
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
	pos[QUADRATIC_BEZIER_PARAM].setQuadraticBezierParams(a, b);

	float steep = 0.5 + 0.5 * sin( 0.1 * t);
	pos[EXPONENTIAL_SIGMOID_PARAM].setDoubleExpSigmoidParam( steep );

	a = 0.5 + 0.5 * sin( 0.05 * t);
	b = 0.5 + 0.5 * sin( -0.03 * t + 3);
	float c = 0.5 + 0.5 * sin( 0.04 * t + 6.4);
	float d = 0.5 + 0.5 * sin( 0.06 * t + 44);
	pos[CUBIC_BEZIER_PARAM].setCubicBezierParams(a, b, c, d);


	float elastG = 1.0 + 0.5 * sinf(t * 0.1);
	float elastFreq = 1.0 + 0.5 * sinf(t * 0.1 + 1.0);
	pos[EASE_IN_ELASTIC].setElasticGain(elastG, elastFreq);
	pos[EASE_OUT_ELASTIC].setElasticGain(elastG, elastFreq);
	pos[EASE_IN_OUT_ELASTIC].setElasticGain(elastG, elastFreq);
	pos[EASE_OUT_IN_ELASTIC].setElasticGain(elastG, elastFreq);


	float easeOutOffset =  1.5 * sinf(t * 0.07);
	pos[EASE_IN_BACK].setEaseBackOffset(easeOutOffset);
	pos[EASE_OUT_BACK].setEaseBackOffset(easeOutOffset);
	pos[EASE_IN_OUT_BACK].setEaseBackOffset(easeOutOffset);
	pos[EASE_OUT_IN_BACK].setEaseBackOffset(easeOutOffset);


	//benchmark the curves individually, report through ofxTimeMeasurements
	#ifdef TIME_SAMPLE
	for ( int i = 0 ; i < NUM_ANIM_CURVES; i++ ){
		AnimCurve curve = (AnimCurve) (EASE_IN_EASE_OUT + i);
		string curveName = ofxAnimatable::getCurveName(curve);
		TS_START(curveName);
		int nIterations = 3000;
		for(int k = 0; k < nIterations; k++){
			float percent = k / float(nIterations - 1);
			ofxAnimatable::calcCurveAt(percent, curve, 0.5f, 0.5f, 0.5f, 0.5f);
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

	//all left animation plots
	int vOff = 20;
	for ( int i = 0 ; i < NUM_ANIM_CURVES; i++ ){
		float lineHeight = 1.7f;
		float yy = vOff + i * lineHeight * width;
		glColor4ub(255,255,255,64);
		ofLine(xMargin, yy + width * 0.5, xMargin + widthCol + width, yy + width * 0.5);
		glColor4ub(255,255,255,255);
		ofRect( pos[i].val(), yy, width, width);
		ofDrawBitmapString( curveNames[i] /*+ " vel: " + ofToString( pos[i].getCurrentSpeed(), 1)*/, xMargin + widthCol + 20, yy + 10);
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


	//plots
	int c = 0;
	int size = 80;
	int yy = size * 1.2;
	int rowHeight = 2.7 * size ;
	int xx = 330;
	int colWidth = size/2.35f;
	int x = 0;
	int row = 0;
	for ( int i = 0 ; i < NUM_ANIM_CURVES; i++ ){
		ofColor c = ofColor::red;
		c.setHsb(fmod(128 + 3 * 255.0f * i/float(NUM_ANIM_CURVES), 255), 255, 255);
		AnimCurve curve = (AnimCurve) (EASE_IN_EASE_OUT + i);
		string curveName = ofxAnimatable::getCurveName((AnimCurve)i);
		drawPlot( xx + x, yy + row * rowHeight - 13 * i, size, curve, curveName, c );
		x += (size + colWidth);
		if (  x > ofGetWidth() -  1.0f * size - xx){
			row++;
			x = 0;
		}
	}
}

void testApp::drawPlot(int x, int y, int size, AnimCurve curve, string title, ofColor c){

	pos[curve].drawCurve(x, y, size, true, c);
}



//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	pointAnim.animateTo( ofPoint(x, y) );
}

