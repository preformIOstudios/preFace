#include "ofApp.h"

ofxFaceTracker::Gesture gestureIds[] = {
    ofxFaceTracker::MOUTH_WIDTH,
    ofxFaceTracker::MOUTH_HEIGHT,
    ofxFaceTracker::LEFT_EYEBROW_HEIGHT,
    ofxFaceTracker::RIGHT_EYEBROW_HEIGHT,
    ofxFaceTracker::LEFT_EYE_OPENNESS,
    ofxFaceTracker::RIGHT_EYE_OPENNESS,
    ofxFaceTracker::JAW_OPENNESS,
    ofxFaceTracker::NOSTRIL_FLARE
};

string gestureNames[] = {
    "mouthWidth",
    "mouthHeight",
    "leftEyebrowHeight",
    "rightEyebrowHeight",
    "leftEyeOpenness",
    "rightEyeOpenness",
    "jawOpenness",
    "nostrilFlare"
};

int gestureCount = 8;

//--------------------------------------------------------------
void ofApp::setup(){
    cout << "Hello World!" << endl;
    
    ofSetVerticalSync(true);
    cam.initGrabber(640, 480);
    
    tracker.setup();
    tracker.setRescale(.5);
    
    classifier.load("expressions");
    
    //TODO:
    // - scan for all JSON files in "images" directory
    ofDirectory imgDir("images");
    imgDir.allowExt("json");
    imgDir.listDir();
    
    if (imgDir.numFiles()) {
        images.resize(imgDir.numFiles());
        
        for(int i = 0; i < imgDir.numFiles(); i++){
            
            bool parsingSuccessful = result.open(imgDir.getPath(i));
            
            if (parsingSuccessful)
            {
                //TODO:
                // - create GRT data from JSON files
                // - load images
                
                
                
                // debug output //
                //            ofLogNotice("ofApp::setup() -- result.getRawString() = ") << endl << result.getRawString() << endl;
            }
            else
            {
                ofLogNotice("ofApp::setup")  << "Failed to parse JSON" << endl;
            }
            cout << "ofApp::setup() -- imgDir.getPath(i) = \"" << imgDir.getPath(i) << "\"" << endl;
        }
    }
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
    ofBackground(0);
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
    if(key == 'f' || key == 'F'){
        ofToggleFullscreen();
    }
    if(key == 'r') {
        tracker.reset();
        classifier.reset();
    }
    if(key == 'e') {
        classifier.addExpression();
    }
    if(key == 'a') {
        classifier.addSample(tracker);
    }
    if(key == 's') {
        classifier.save("expressions");
    }
    if(key == 'l') {
        classifier.load("expressions");
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
