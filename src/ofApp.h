#pragma once

#include "ofMain.h"
#include "ui/UIManager.h"

class ofApp : public ofBaseApp{

	public:
		void setup() override;
		void update() override;
		void draw() override;
		void exit() override;

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
		void mouseDragged(int x, int y, int button) override;
		void mousePressed(int x, int y, int button) override;
		void mouseReleased(int x, int y, int button) override;
		void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
		void mouseEntered(int x, int y) override;
		void mouseExited(int x, int y) override;
		void windowResized(int w, int h) override;
		void dragEvent(ofDragInfo dragInfo) override;
		void gotMessage(ofMessage msg) override;


		// UI manager holds buttons, drawing and hit-testing logic
		void renderButtons();
		void handleButtonHover(int x, int y);
		void handleButtonClick(int x, int y);
		ui::UIManager uiManager;

		bool showUI = true;

		void action1();
		void action2();
		void action3();
		void action4();
		void action5();
};
