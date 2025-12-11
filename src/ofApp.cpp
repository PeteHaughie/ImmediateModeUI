#include "ofApp.h"
#include "ui/UIManager.h"
#include <cmath>

//--------------------------------------------------------------
void ofApp::setup() {

	{
		uiManager.addButton(100, 100, 50, 50, "Button 1", "rectangle", 0.0f, [this]() { this->action1(); }, "push");
	}

		uiManager.addButton(100, 200, 50, 50, "Button 2", "ellipse", 0.0f, [this]() { this->action2(); }, "toggle");

		uiManager.addButton(200, 200, 100, 50, "Button 3", "ellipse", 0.0f, [this]() { this->action3(); }, "toggle");

		// polygon: use convenience addPolygon overload
		uiManager.addPolygon(100, 300, { {0,0}, {50,50}, {50,0} }, "Button 4", 0.0f, [this]() { this->action4(); }, "push");

		uiManager.addButton(250, 100, 80, 40, "Button 5", "rectangle", 45.0f, [this]() { this->action5(); }, "push");
}

void ofApp::renderButtons() {
	uiManager.draw();
}

// Transform a world point (x,y) into button-local coordinates relative to the button center,
// with rotation removed. Returned vector is (x_rel, y_rel) where (0,0) is the center.
// moved helpers into UIManager

void ofApp::action1() {
    ofLog() << "Action 1 executed!";
}

void ofApp::action2() {
    ofLog() << "Action 2 executed!";
}

void ofApp::action3() {
    ofLog() << "Action 3 executed!";
}

void ofApp::action4() {
	ofLog() << "Action 4 executed!";
}

void ofApp::action5() {
	ofLog() << "Action 5 executed!";
}

void ofApp::handleButtonHover(int x, int y) {
	uiManager.onMouseMoved(x, y);
}

void ofApp::handleButtonClick(int x, int y) {
	uiManager.onMousePressed(x, y);
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (showUI) {
		renderButtons();
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'd') {
		showUI = !showUI;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	handleButtonHover(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	handleButtonClick(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}
