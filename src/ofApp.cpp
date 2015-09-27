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
    
    trainedData = false;
    predictedLabel = 0;
    GRTtrainingData.setNumDimensions(8);
    
    ofSetVerticalSync(true);
    cam.initGrabber(640, 480);
    
    tracker.setup();
    tracker.setRescale(.5);
    
    classifier.load("expressions");
    
    // list all JSON files in "images" directory
    ofDirectory imgDir("images");
    imgDir.allowExt("json");
    imgDir.listDir();
    
    if (imgDir.numFiles()) {
        images.resize(imgDir.numFiles());
        
        // create GRT data and load images for each JSON file
        for(int i = 0; i < imgDir.numFiles(); i++){
            
            bool parsingSuccessful = result.open(imgDir.getPath(i));
            
            if (parsingSuccessful)
            {
                vector<double> sample;
                for(int j = 0; j < gestureCount; j++) {
                    // TODO: get actual sample data from JSON file
//                    sample.push_back(result[gestureIds[j]].asDouble());
                    sample.push_back(ofRandom(100.0));
                }
                
                // add GRT data point
                GRTtrainingData.addSample(i+1, sample);
                
                // load associated image
                ofImage* img = new ofImage;
                string imgName = ofToDataPath("images/" + imgDir.getFile(i).getBaseName()+".png");
                img->loadImage(imgName);
                images[i] = img;
                
                cout << "ofApp::setup() -- imgName = " << imgName << endl;
            }
            else
            {
                ofLogNotice("ofApp::setup")  << "Failed to parse JSON" << endl;
            }
            cout << "ofApp::setup() -- imgDir.getPath(i) = \"" << imgDir.getPath(i) << "\"" << endl;
        }
        
        GRTtrainingData.save(ofToDataPath("GRTtrainingData.grt"));
        GRTpipeline.setClassifier( KNN() );
        if( !GRTpipeline.train(GRTtrainingData)  ){
            cout << "ofApp::setup() -- ERROR: Failed to train the pipeline!\n";
        } else {
            cout << "ofApp::setup() -- trained the pipeline!\n";
            trainedData = true;
            GRTpipeline.save(ofToDataPath("GRTpipeline.grt"));
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
    
    // predict label
    if (trainedData) {
        vector<double> sample;

        for(int j = 0; j < gestureCount; j++) {
            sample.push_back(tracker.getGesture(gestureIds[j]));
        }

        predictedLabel = GRTpipeline.predict(sample); // TODO: this seems to always predict label 1
        predictedLabel = ofRandom(images.size()); // TODO: remove random label generation?
        cout << "ofApp::update() -- sample = " << ofToString(sample) << endl;
        cout << "ofApp::update() -- predictedLabel = " << predictedLabel << endl;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);
    cam.draw(0, 0);
    tracker.draw();
    
    if (trainedData && predictedLabel) {
        // TODO: draw predicted image
        images[predictedLabel-1]->draw(ofGetWidth() / 2.0f, 0);
    }
    
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
    
    // TODO: set up key presses for different rendering modes
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
