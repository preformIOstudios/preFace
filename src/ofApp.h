#pragma once

#include "ofMain.h"
#include "ofxFaceTracker.h"
#include "GRT.h"
#include "ofxJSON.h"

using namespace ofxCv;
using namespace cv;
using namespace GRT;

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
    
    ofVideoGrabber cam;
    ofxFaceTracker tracker;
    ExpressionClassifier classifier;

    ofxJSONElement result;

    vector<ofImage*> images;
    vector<ofVec4f> positions;
    
    GestureRecognitionPipeline GRTpipeline;
    ClassificationData  GRTtrainingData;
    bool    trainedData;
    bool drawInteractive, drawDebug, drawSmallDebug;
    int    predictedLabel;
    
};
