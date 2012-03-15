#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofBackground(128, 128, 128);
	ofSetFrameRate(60);
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	ofSetVerticalSync(true);
		
	width = 10;	

	pos[0].animateFromTo( 300, 500 );
	pos[0].setDuration(2);	
	pos[0].setRepeatType( ofxAnimatable::LOOP_BACK_AND_FORTH );
	for ( int i = 1; i < NUM_CURVES; i++ )
		pos[i] = pos[0];
	
	pos[0].setCurve(ofxAnimatable::LINEAR);
	curveNames[0] = "LINEAR";

	pos[1].setCurve(ofxAnimatable::EASE_IN);
	curveNames[1] = "EASE_IN";
	
	pos[2].setCurve(ofxAnimatable::EASE_OUT);
	curveNames[2] = "EASE_OUT";
	
	pos[3].setCurve(ofxAnimatable::EASE_IN_EASE_OUT);
	curveNames[3] = "EASE_IN_EASE_OUT";

	pos[4].setCurve(ofxAnimatable::TANH);
	curveNames[4] = "TANH";

	pos[5].setCurve(ofxAnimatable::SINH);
	curveNames[5] = "SINH";

	pos[6].setCurve(ofxAnimatable::BOUNCY);	
	curveNames[6] = "BOUNCY";

	
	ball.reset(300);
	ball.setCurve(ofxAnimatable::EASE_IN);
	ball.setRepeatType(ofxAnimatable::LOOP_BACK_AND_FORTH_SWAP_CURVE);
	ball.setDuration(0.55);
	ball.animateTo( 400 );
	
	colorAnim.setColor( ofColor::black );
	colorAnim.setDuration( 0.5f );
	colorAnim.setRepeatType(ofxAnimatable::LOOP_BACK_AND_FORTH);
	colorAnim.setCurve(ofxAnimatable::LINEAR);
	colorAnim.animateTo( ofColor::white );
	
	pointAnim.setPosition( ofPoint(100,100) );
	pointAnim.setRepeatType(ofxAnimatable::PLAY_ONCE);
	pointAnim.setCurve(ofxAnimatable::EASE_IN_EASE_OUT);
	
}

//--------------------------------------------------------------
void testApp::update(){

	float dt = 1.0f / ofGetFrameRate();
	
	for ( int i = 0; i < NUM_CURVES; i++ )
		pos[i].update( dt );
	
	ball.update( dt );
	
	colorAnim.update( dt);
	pointAnim.update( dt);

	if ( !pointAnim.isOrWillBeAnimating() ){
		pointAnim.animateToAfterDelay( ofPoint( ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight())), 0.5);
	}
		
//	fr = 30 + 30 * mouseY / ofGetHeight() ;
//	fr = ofClamp(fr, 1, 60);
//	ofSetFrameRate(fr);
	
}

//--------------------------------------------------------------
void testApp::draw(){

	glColor3ub(255,255,255);
	for ( int i = 0 ; i < NUM_CURVES; i++ ){
		ofRect( pos[i].val(), 100 + i * 2.5 * width, width, width);
		ofDrawBitmapString(  curveNames[i], 515, 100 + i * 2.5 * width + 10);
	}

	//ball and floor
	colorAnim.setColor();
	ofCircle( ( 2 * ofGetFrameNum() )%ofGetWidth(), ball.val(), width);
	
	glColor3ub(255,255,255);
	ofRect(0, 400 + width, ofGetWidth(), 1);

	//vertical lines
	ofRect(300, 0, 1, ofGetHeight());
	ofRect(500+width, 0, 1, ofGetHeight());
	
	glColor3f( pointAnim.getPercentDone(), 1 - pointAnim.getPercentDone() , 0);
	glPointSize(10);
	pointAnim.draw();

	glColor3ub(255,255,255);
	ofDrawBitmapString( ofToString( ofGetFrameRate()),  10, 10);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

	pointAnim.animateTo( ofPoint(x, y) );
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

