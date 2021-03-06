#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofDisableArbTex();

	ofxFBXSceneSettings settings;

	//string filename = "piranha.FBX";
	string filename = "astroBoy_walk.fbx";

	if (scene.load(filename, settings)) {
		cout << "ofApp :: loaded the scene OK" << endl;
	}
	else {
		cout << "ofApp :: Error loading the scene" << endl;
	}

	cam.lookAt(ofVec3f(0, 0, 0));
	cam.setDistance(20);
	cam.setFarClip(100);
	cam.setNearClip(.5f);

	fbxMan.setup(&scene);
	fbxMan.setAnimation(0);
	fbxMan.setPosition(0, -7, 0);

	cout << fbxMan.getSkeletonInfo() << endl;

	bRenderNormals = false;
	bRenderMeshes = true;
	bDrawBones = false;
}

//--------------------------------------------------------------
void ofApp::update(){
	light.setPosition(cos(ofGetElapsedTimef()*2.) * 7, 4 + sin(ofGetElapsedTimef()) * 2.5, 10);

	ofVec3f target(ofMap(ofGetMouseX(), 0, ofGetWidth(), -10, 10, true), fbxMan.getPosition().y, fbxMan.getPosition().z + 10);
	fbxMan.lookAt(target);
	fbxMan.pan(180);

	fbxMan.getCurrentAnimation().setSpeed(ofMap(ofGetMouseY(), 100, ofGetHeight() - 100, 0.5, 2.5, true));

	fbxMan.update();

	// perform any bone manipulation here //
	ofxFBXBone* bone = fbxMan.getBone("head");
	if (bone != NULL) {
		bone->disableAnimation(true);
		bone->pointTo(light.getPosition(), ofVec3f(1, 0, 0));
	}

	fbxMan.lateUpdate();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255, 255, 255);

	glEnable(GL_DEPTH_TEST);


	cam.begin();

	ofEnableLighting();
	light.enable();

	if (bRenderMeshes) {
		ofSetColor(255, 255, 255);
		fbxMan.draw();
	}

	light.disable();
	ofDisableLighting();

	if (bDrawBones) {
		fbxMan.drawSkeletons(0.5);
	}

	if (bRenderNormals) {
		ofSetColor(255, 0, 255);
		fbxMan.drawMeshNormals(0.5, false);
	}

	ofNoFill();
	ofSetColor(50, 50, 50);
	ofDrawBox(0, 0, 0, 14);
	ofFill();

	ofSetColor(light.getDiffuseColor());
	ofDrawSphere(light.getPosition(), 1);

	cam.end();

	glDisable(GL_DEPTH_TEST);

	int numBones = 0;
	vector< shared_ptr<ofxFBXSkeleton> >& skeletons = fbxMan.getSkeletons();
	for (int i = 0; i < skeletons.size(); i++) {
		numBones += skeletons[i]->getNumBones();
	}

	ofSetColor(60, 60, 60);
	stringstream ds;
	ds << "Render normals (n): " << bRenderNormals << endl;
	ds << "Render meshes (m): " << bRenderMeshes << endl;
	ds << "Render " << numBones << " bones (b): " << bDrawBones << endl;
	if (fbxMan.areAnimationsEnabled()) {
		ds << "Toggle play/pause (spacebar): playing: " << fbxMan.getCurrentAnimation().isPlaying() << endl;
		ds << "Previous/Next animation (up/down): " << fbxMan.getCurrentAnimation().name << endl;
	}
	ds << "Scale is " << fbxMan.getScale() << endl;
	if (fbxMan.getNumPoses() > 0) {
		ds << "Pose: " << fbxMan.getCurrentPose()->getName() << " num poses: " << fbxMan.getNumPoses() << " enabled (p): " << fbxMan.arePosesEnabled() << endl;
	}
	ofDrawBitmapString(ds.str(), 50, 30);


	for (int i = 0; i < fbxMan.getNumAnimations(); i++) {
		stringstream ss;
		ofxFBXAnimation& anim = fbxMan.getAnimation(i);
		if (i == fbxMan.getCurrentAnimationIndex()) {
			ss << "- ";
		}
		ss << "name: " << anim.name << " " << ofToString(anim.getPositionSeconds(), 3) << " | " << ofToString(anim.getDurationSeconds(), 3) << " frame: " << anim.getFrameNum() << " / " << anim.getTotalNumFrames() << endl;
		ofDrawBitmapString(ss.str(), 50, i * 30 + 450);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (scene.getNumAnimations() > 1) {
		if (key == OF_KEY_DOWN) {
			int newAnimIndex = fbxMan.getCurrentAnimationIndex() + 1;
			if (newAnimIndex > scene.getNumAnimations() - 1) {
				newAnimIndex = 0;
			}
			fbxMan.setAnimation(newAnimIndex);

		}
		if (key == OF_KEY_UP) {
			int newAnimIndex = fbxMan.getCurrentAnimationIndex() - 1;
			if (newAnimIndex < 0) {
				newAnimIndex = scene.getNumAnimations() - 1;
			}
			fbxMan.setAnimation(newAnimIndex);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == ' ') {
		fbxMan.getCurrentAnimation().togglePlayPause();
	}
	if (key == 'n') {
		bRenderNormals = !bRenderNormals;
	}
	if (key == 'm') {
		bRenderMeshes = !bRenderMeshes;
	}
	if (key == 'b') {
		bDrawBones = !bDrawBones;
	}
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
