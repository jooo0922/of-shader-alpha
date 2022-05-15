#pragma once

#include "ofMain.h"

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        // ofApp.cpp 에서 사용할 멤버 변수들을 헤더파일에 선언해놓음.
        ofMesh charMesh; // 캐릭터 텍스쳐를 씌워서 사용할 메쉬 객체 변수 선언
        ofShader charShader; // 위에 캐릭터 메쉬를 draw 할 때 바인딩해서 사용할 셰이더 객체 변수 선언
        ofImage alienImg; // 캐릭터 텍스쳐를 로드한 뒤 프래그먼트 셰이더로 전송할 때 사용할 이미지 객체 변수 선언
};
