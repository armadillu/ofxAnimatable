#include "testApp.h"

int floorLine = 500;


//--------------------------------------------------------------
void testApp::setup(){

	
	ofBackground(99);
	ofSetFrameRate(60);
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	
	width = 10;	

	ofxAnimatableFloat f;
	f.animateFromTo( 300, 500 );
	f.setDuration(2);	
	f.setRepeatType( LOOP_BACK_AND_FORTH );
	
	for ( int i = 0; i < NUM_ANIM_CURVES; i++ ){
		pos[i] = f;
		AnimCurve curve = (AnimCurve) (EASE_IN_EASE_OUT + i );
		pos[i].setCurve( curve );
		curveNames[i] = ofxAnimatable::getCurveName( curve );
	}

	ball.reset(300);
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

	//animate our parametric curves
	float a = 0.5 + 0.5 * sin( 0.1 * ofGetFrameNum());
	float b = 0.5 + 0.5 * sin( 0.1 * ofGetFrameNum() + 400);
	pos[QUADRATIC_BEZIER_PARAM].setQuadraticBezierParams( a, b);

	float steep = 0.5 + 0.5 * sin( 0.1 * ofGetFrameNum());
	pos[EXPONENTIAL_SIGMOID_PARAM].setDoubleExpSigmoidParam( steep );

}

//--------------------------------------------------------------
void testApp::draw(){


	int vOff = 10;
	for ( int i = 0 ; i < NUM_ANIM_CURVES; i++ ){
		float lineHeight = 2.0;
		float yy = vOff + i * lineHeight * width;
		glColor4ub(255,255,255,64);
		ofLine(300, yy + width * 0.5, 500 + width, yy + width * 0.5);
		glColor4ub(255,255,255,255);
		ofRect( pos[i].val(), yy, width, width);
		ofDrawBitmapString(  curveNames[i] + "  vel: " + ofToString( pos[i].getCurrentSpeed(), 2), 515, yy + 10);
	}

	//ball and floor
	colorAnim.applyCurrentColor();
	ofCircle( ( 2 * ofGetFrameNum() )%ofGetWidth(), ball.val(), width);
	
	glColor4ub(255,255,255,255);
	ofRect(0, floorLine + width, ofGetWidth(), 1);

	//vertical lines
	ofRect(300, 0, 1, floorLine + width);
	ofRect(500 + width, 0, 1, floorLine + width);
	
	glColor4f( pointAnim.getPercentDone(), 1 - pointAnim.getPercentDone() , 0, 1);
	glPointSize(10);
	pointAnim.draw();

	glColor4ub(255,255,255,255);
	ofDrawBitmapString( ofToString( ofGetFrameRate()),  10, 10);
	
	int c = 0;
	int size = 80;
	int yy = floorLine + 50;
	int rowHeight = size + 30;
	int xx = 50;
	int off = size/2.5;
	int x = 0;
	int row = 0;
	for ( int i = 0 ; i < NUM_ANIM_CURVES; i++ ){
		AnimCurve curve = (AnimCurve) (EASE_IN_EASE_OUT + i);		
		drawPlot( xx + x, yy + row * rowHeight, size, curve, ofxAnimatable::getCurveName(curve) );
		x += (size + off);
		if (  x > ofGetWidth() -  2 * size){
			row++;
			x = 0;
		}
	}
}

void testApp::drawPlot(int x, int y, int size, AnimCurve curve, string title){

	int xx = x;
	int yy = y;
	float s = size;
	float steps = size;
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

