#include "ofApp.h"

// 캐릭터 텍스쳐를 입힐 쿼드 메쉬 생성 함수를 따로 밖으로 빼서 정리함. (코드가 길어서 setup 에서는 해당 함수를 호출해서 사용하도록 함.)
// 주의사항: ofMesh 객체를 인자로 넘겨줄 때, 파라미터 타입을 ofMesh로 지정할 시 & 기호를 반드시 붙여서 'ofMesh&' 로 써줘야 함! -> 이거 안붙이면 렌더링이 아예 안됨.
void buildMesh(ofMesh& mesh, float w, float h, glm::vec3 pos) {
    // 캐릭터 메쉬를 위치시키려는 vec3 pos 값과 width, height 값으로 계산하여
    // 4개 버텍스의 x, y, z 좌표값들을 verts 라는 float 배열에 담아놓음.
    float  verts[] = {
        -w + pos.x, -h + pos.y, pos.z,
        -w + pos.x, h + pos.y, pos.z,
        w + pos.x, h + pos.y, pos.z,
        w + pos.x, -h + pos.y, pos.z,
    }; // 좌표값 계산 과정을 보면, w, h 는 아마 캐릭터 메쉬의 너비, 높이 자체가 아닌, w, h 를 각각 두 배 시켜줘야
    // 이 좌표값으로 그려지는 캐릭터 메쉬의 너비값과 높이값이 나오게 될 것임.
    
    // 4개 버텍스의 uv좌표값들을 uvs 라는 float 배열에 담아놓음
    float uvs[] = {
        0, 0,
        0, 1,
        1, 1,
        1, 0
    };
    
    // for loop 를 돌면서 verts, uvs 배열에 담긴 좌표값들을 꺼내와서 인자로 전달받은
    // ofMesh 에 버텍스 및 uv좌표를 추가해 줌.
    for (int i = 0; i < 4; ++i) { // 4개의 버텍스가 필요하니 for loop를 4번 돌리고,
        int idx = i * 3; // 버텍스 위치는 버텍스 하나 당 3개의 좌표값이 필요하니 인덱스값도 3배수로 들어가는군.
        int uvIdx = i * 2; // 버텍스 uv좌표는 버텍스 하나 당 2개의 좌표값이 필요하니 인덱스값도 2배수로 들어가는군.
        mesh.addVertex(glm::vec3(verts[idx], verts[idx + 1], verts[idx + 2])); // 버텍스 위치는 각 버텍스당 3개의 좌표값을 갖는 glm::vec3 데이터를 추가해줌.
        mesh.addTexCoord(glm::vec2(uvs[uvIdx], uvs[uvIdx + 1])); // 버텍스 uv는 각 버텍스당 2개의 좌표값을 갖는 glm::vec2 데이터를 추가해\줌.
    }
    
    // 4개의 버텍스 순서를 인덱싱해서 어떻게 쿼드 메쉬의 삼각형들을 그려줄 것인지 결정함.
    ofIndexType indices[6] = {0, 1, 2, 2, 3, 0};
    mesh.addIndices(indices, 6); // ofIndexType 을 메쉬에 추가함.
}

//--------------------------------------------------------------
void ofApp::setup(){
    // uv좌표 대신 스크린 픽셀 좌표를 사용하는 텍스쳐 유형을 기본설정 해두는 오프프레임웍스의 레거시 지원을 비활성화 하기 위해 아래 함수를 호출함. (우리는 uv좌표계로 텍스쳐를 다룰거니까!)
    ofDisableArbTex();
    ofEnableDepthTest();
    
    // 이 함수는 ofMesh, 메쉬의 width 절반값, height 절반값, 메쉬의 위치(메쉬의 가운데 기준) 좌표값을 넘겨줘서 버텍스 데이터를 추가하여 메쉬를 생성하는 함수
    // 아래와 같이 width, height 을 넘겨준다면, width 는 전체 -1 ~ 1 에서 0.5 가 될테니, 전체 x축 길이의 사분의 일(0.5 /. 2)이 될거고,
    // height 은 전체 -1 ~ 1 에서 1.0 이 될테니 전체 y축 길이의 절반 (1.0 / 2) 이 될거임.
    buildMesh(charMesh, 0.1, 0.2, glm::vec3(0.0, -0.2, 0.0)); // 배경메쉬가 추가되면서, 캐릭터 메쉬의 사이즈와 위치를 조절함
    buildMesh(backgroundMesh, 1.0, 1.0, glm::vec3(0.0, 0.0, 0.5)); // 배경메쉬 생성 시, z좌표값이 캐릭터메쉬보다 커야(즉, 0보다 커야) NDC 좌표계 상에서 캐릭터 뒤에 가므로, z좌표값을 0보다 크게 설정해 줌. (아마 opengl은 z축이 컴퓨터 안쪽으로 갈수록 양의 방향인 거 같음.)

    alienImg.load("alien.png"); // ofShader 와 마찬가지로, bin/data 디렉토리를 기준으로 한 상대경로를 받으므로, 해당 디렉토리에 이미지 파일을 저장했다면 파일명만 인자로 넣어주면 됨.
    backgroundImg.load("forest.png"); // 배경 텍스쳐도 마찬가지로, bin/data 디렉토리에 저장해뒀으므로, 파일명만 인자로 넘겨줬음.
    
    // ofShader 객체에 bin/data 폴더에 작성한 셰이더 파일들을 불러와서 로드함.
    // bin/data 디렉토리를 기준으로 한 상대경로이므로, 파일명만 인자로 넣어주면 됨.
    alphaTestShader.load("passthrough.vert", "alphaTest.frag");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    alphaTestShader.begin();
    
    /**
     예제에서도 그렇고,
     캐릭터메쉬의 드로우콜을 먼저 하고,
     배경 메쉬의 드로우콜을 그 다음 호출하고 있음.
     
     그런데 이대로 실행해버리면, 캐릭터 메쉬가 사라지고
     배경메쉬만 렌더링되는 걸 볼 수 있음.
     
     그런데, 분명 배경메쉬의 z좌표값을 캐릭터메쉬보다 크게 잡아서
     배경메쉬가 더 뒤로 가도록 렌더링되는게 맞는데,
     왜 캐릭터메쉬가 배경메쉬에 묻혀서 안보이는걸까?
     
     그 이유는, 1차적으로는 드로우콜의 순서가
     캐릭터메쉬가 먼저 호출되었기 때문에,
     캐릭터 메쉬 그려지고 난 다음 배경 메쉬가 그려져서
     그런 것도 있음.
     
     그래서 배경메쉬의 드로우콜을 먼저 시켜주면
     캐릭터가 정상적으로 그려지기는 함.
     
     그렇지만, 이거는 본질적인 해결방법은 아님.
     
     메쉬의 z좌표값을 비교해서 렌더링하려면,
     gpu가 '깊이 버퍼' 를 저장해둬야 함.
     
     이 깊이버퍼가 있어야 gpu가 메쉬를 렌더링할 때,
     뭐가 앞이고 뭐가 뒤인지를 구분할 수 있음.
     
     문제는, 이 과정은 '기본적으로, 알아서, 자동으로 되는 게 아니다' 라는 것!
     그래서 오픈프레임웍스에서는 ofEnableDepthTest() 라는 함수를 직접 호출해야
     깊이버퍼를 생성하도록 gpu 에게 지시할 수 있음.
     */
    // alphaTestShader.setUniformTexture("tex", backgroundImg, 0);
    // backgroundMesh.draw();
    
    alphaTestShader.setUniformTexture("tex", alienImg, 0);
    charMesh.draw();
    
    alphaTestShader.setUniformTexture("tex", backgroundImg, 0);
    backgroundMesh.draw();
    
    alphaTestShader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
