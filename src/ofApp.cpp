#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    cout << "Hello World!";
    
    ofSetVerticalSync(true);
    cam.initGrabber(640, 480);
    
    tracker.setup();
    tracker.setRescale(.5);
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
    if(cam.isFrameNew()) {
        if(tracker.update(toCv(cam))) {
            classifier.classify(tracker);
        }		
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    cam.draw(0, 0);
    tracker.draw();
    
    int w = 100, h = 12;
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(5, 10);
    int n = classifier.size();
    int primary = classifier.getPrimaryExpression();
    for(int i = 0; i < n; i++){
        ofSetColor(i == primary ? ofColor::red : ofColor::black);
        ofRect(0, 0, w * classifier.getProbability(i) + .5, h);
        ofSetColor(255);
        ofDrawBitmapString(classifier.getDescription(i), 5, 9);
        ofTranslate(0, h + 5);
    }
    ofPopMatrix();
    ofPopStyle();
    
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()), ofGetWidth() - 20, ofGetHeight() - 10);
    drawHighlightString(
                        string() +
                        "r - reset\n" +
                        "e - add expression\n" +
                        "a - add sample\n" +
                        "s - save expressions\n"
                        "l - load expressions",
                        14, ofGetHeight() - 7 * 12);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key){
            
        case 'f':
        case 'F':
            ofToggleFullscreen();
            break;
            
        default:
            break;
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
