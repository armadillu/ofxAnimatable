#include "testApp.h"

int floorLine = 630;
int xMargin = 0;
int widthCol = 60;

//--------------------------------------------------------------
void testApp::setup(){

	ofBackground(22);
	ofSetFrameRate(60);
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	#ifdef TIME_SAMPLE
	TIME_SAMPLE_SET_FRAMERATE(60);
	TIME_SAMPLE_SET_PRECISION(3);
	TIME_SAMPLE_SET_AVERAGE_RATE(0.01);
	TIME_SAMPLE_SET_DRAW_LOCATION(TIME_MEASUREMENTS_BOTTOM_RIGHT);
	TIME_SAMPLE_DISABLE();
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

	pointAnim.setDuration(0.3);
	pointAnim.setPosition( ofPoint(100,100) );
	pointAnim.setRepeatType(PLAY_N_TIMES);
	pointAnim.setCurve(QUADRATIC_EASE_OUT);
	pointAnim.setRepeatTimes(3);
	//pointAnim.setAutoFlipCurve(true);

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

//	if ( !pointAnim.isOrWillBeAnimating() ){
//		pointAnim.animateToAfterDelay( ofPoint( ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight())), 0.5);
//	}

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

	float elastG = 1.0 + 0.5 * sinf(t * 0.05);
	float elastFreq = 1.0 + 0.5 * sinf(t * 0.075 + 1.0);
	float elastDecay = 0.5 + 0.25 * sinf(t * 0.0375 - 1.0);
	pos[EASE_IN_ELASTIC].setElasticParams(elastG, elastFreq, elastDecay);
	pos[EASE_OUT_ELASTIC].setElasticParams(elastG, elastFreq, elastDecay);
	pos[EASE_IN_OUT_ELASTIC].setElasticParams(elastG, elastFreq, elastDecay);
	pos[EASE_OUT_IN_ELASTIC].setElasticParams(elastG, elastFreq, elastDecay);

	float easeOutOffset =  1.5 * sinf(t * 0.07);
	pos[EASE_IN_BACK].setEaseBackOffset(easeOutOffset);
	pos[EASE_OUT_BACK].setEaseBackOffset(easeOutOffset);
	pos[EASE_IN_OUT_BACK].setEaseBackOffset(easeOutOffset);
	pos[EASE_OUT_IN_BACK].setEaseBackOffset(easeOutOffset);

	int numBounces =  1 + 6 * fabs(sinf(t * 0.005));
	float bounceElast = 0.2 + 0.5 * fabs(sinf(-t * 0.02));
	pos[BOUNCE_IN_CUSTOM].setCustomBounceParams(numBounces, bounceElast);
	pos[BOUNCE_OUT_CUSTOM].setCustomBounceParams(numBounces, bounceElast);

	//benchmark the curves individually, report through ofxTimeMeasurements
	#ifdef TIME_SAMPLE
	for ( int i = 0 ; i < NUM_ANIM_CURVES; i++ ){
		AnimCurve curve = (AnimCurve) (EASE_IN_EASE_OUT + i);
		string curveName = ofxAnimatable::getCurveName(curve);
		TS_START(curveName);
		int nIterations = 3000;
		float buffer1[20];
		float buffer2[20];
		for(int k = 0; k < nIterations; k++){
			float percent = k / float(nIterations - 1);
			ofxAnimatable::calcCurveAt(percent, curve, 0.5f, 0.5f, 0.5f, 0.5f, &buffer1[0], &buffer2[0]);
		}
		TS_STOP(curveName);
	}
	#endif

}

//--------------------------------------------------------------
void testApp::draw(){

	//all left animation plots
	int vOff = 8;
	for ( int i = 0 ; i < NUM_ANIM_CURVES; i++ ){
		float lineHeight = 1.35f;
		float yy = vOff + i * lineHeight * width;
		glColor4ub(255,255,255,64);
		ofLine(xMargin, yy + width * 0.5, xMargin + widthCol + width, yy + width * 0.5);
		ofColor c = ofColor::red;
		c.setHsb(fmod(128 + 3 * 255.0f * i/float(NUM_ANIM_CURVES), 255), 255, 255);
		ofSetColor(c);
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
	int yy;;
	int xx = 300;
	int size = 70; //of each plot
	int rowPad = 30;
	int colPad = 25;
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

void testApp::drawPlot(int x, int y, int size, AnimCurve curve, string title, ofColor c){

	pos[curve].drawCurve(x, y, size, true, c);
}



//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	pointAnim.animateTo( ofPoint(x, y) );
}

