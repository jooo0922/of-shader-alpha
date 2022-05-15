#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    // 아래 5줄은 초기의 main() 함수에서 원하는 버전의 OpenGL 을 사용하기 위해 수정해줘야 하는 부분들
    ofGLWindowSettings glSettings; // glSettings 라는 변수에 윈도우 설정 관련 of 객체를 넣어줌
    glSettings.setSize(1024, 768); // 결과물을 보여줄 윈도우 사이즈 설정
    glSettings.windowMode = OF_WINDOW; // 윈도우 모드는 OF_FULLSCREEN(크기 조절 안되는 전체화면모드) / OF_WINDOW(크기 조절 가능한 실행창)
    glSettings.setGLVersion(4, 1); // OpenGL 4.1 버전을 쓰려면 (4, 1) 이렇게 정수값을 전달해줘야 함.
    ofCreateWindow(glSettings); // 설정이 변경된 윈도우 설정 of 객체를 ofCreateWindow() 함수에 전달해주면 실행창(윈도우)를 열어줌.
    
	ofRunApp(new ofApp());

}
