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
        mesh.addTexCoord(glm::vec2(uvs[uvIdx], uvs[uvIdx + 1])); // 버텍스 uv는 각 버텍스당 2개의 좌표값을 갖는 glm::vec2 데이터를 추가해 줌.
    }
    
    // 4개의 버텍스 순서를 인덱싱해서 어떻게 쿼드 메쉬의 삼각형들을 그려줄 것인지 결정함.
    ofIndexType indices[6] = {0, 1, 2, 2, 3, 0};
    mesh.addIndices(indices, 6); // ofIndexType 을 메쉬에 추가함.
}

//--------------------------------------------------------------
void ofApp::setup(){
    // uv좌표 대신 스크린 픽셀 좌표를 사용하는 텍스쳐 유형을 기본설정 해두는 오프프레임웍스의 레거시 지원을 비활성화 하기 위해 아래 함수를 호출함. (우리는 uv좌표계로 텍스쳐를 다룰거니까!)
    ofDisableArbTex();
    ofEnableDepthTest(); // 깊이 테스트를 활성화해줘야 메쉬들의 z값끼리 비교할 수 있도록 깊이 버퍼에 z값을 저장할 수 있음.
    
    // 이 함수는 ofMesh, 메쉬의 width 절반값, height 절반값, 메쉬의 위치(메쉬의 가운데 기준) 좌표값을 넘겨줘서 버텍스 데이터를 추가하여 메쉬를 생성하는 함수
    // 아래와 같이 width, height 을 넘겨준다면, width 는 전체 -1 ~ 1 에서 0.5 가 될테니, 전체 x축 길이의 사분의 일(0.5 /. 2)이 될거고,
    // height 은 전체 -1 ~ 1 에서 1.0 이 될테니 전체 y축 길이의 절반 (1.0 / 2) 이 될거임.
    buildMesh(charMesh, 0.1, 0.2, glm::vec3(0.0, -0.2, 0.0)); // 배경메쉬가 추가되면서, 캐릭터 메쉬의 사이즈와 위치를 조절함
    buildMesh(backgroundMesh, 1.0, 1.0, glm::vec3(0.0, 0.0, 0.5)); // 배경메쉬 생성 시, z좌표값이 캐릭터메쉬보다 커야(즉, 0보다 커야) NDC 좌표계 상에서 캐릭터 뒤에 가므로, z좌표값을 0보다 크게 설정해 줌. (아마 opengl은 z축이 컴퓨터 안쪽으로 갈수록 양의 방향인 거 같음.)
    buildMesh(cloudMesh, 0.25, 0.15, glm::vec3(-0.55, 0.0, 0.0));
    buildMesh(sunMesh, 1.0, 1.0, glm::vec3(0.0, 0.0, 0.4));

    // alienImg.load("alien.png"); // ofShader 와 마찬가지로, bin/data 디렉토리를 기준으로 한 상대경로를 받으므로, 해당 디렉토리에 이미지 파일을 저장했다면 파일명만 인자로 넣어주면 됨.
    alienImg.load("walk_sheet.png"); // 캐릭터 메쉬를 캐릭터 텍스쳐 대신, 스프라이트 시트 텍스쳐로 그려줄거임.
    backgroundImg.load("forest.png"); // 배경 텍스쳐도 마찬가지로, bin/data 디렉토리에 저장해뒀으므로, 파일명만 인자로 넘겨줬음.
    cloudImg.load("cloud.png"); // 구름 텍스쳐도 bin/data 디렉토리에 저장해뒀으므로, 파일명만 인자로 넘겨줌.
    sunImg.load("sun.png"); // 태양 텍스쳐도 bin/data 디렉토리에 저장해뒀으므로, 파일명만 인자로 넘겨줌.
    
    // 스프라이트 시트 텍스쳐를 그리기 위해서 spriteSheet.vert 셰이더 파일을 새로 만들었으므로, 얘를 바인딩하기 위한 ofShader 객체도 새로 만들어서 로드해 줌.
    spritesheetShader.load("spritesheet.vert", "alphaTest.frag");
    
    // ofShader 객체에 bin/data 폴더에 작성한 셰이더 파일들을 불러와서 로드함.
    // bin/data 디렉토리를 기준으로 한 상대경로이므로, 파일명만 인자로 넣어주면 됨.
    alphaTestShader.load("passthrough.vert", "alphaTest.frag");
    
    // 알파 블렌딩 기법에 사용할 cloud.frag 셰이더를 새로운 ofShader 객체로 로드해 옴.
    cloudShader.load("passthrough.vert", "cloud.frag");
}

//--------------------------------------------------------------
/**
 update() 함수는 뭐냐면, draw() 함수처럼 매 프레임마다 호출되지만,
 메쉬의 드로우콜 또는 렌더링과 직접적으로 관련이 없는 로직들,
 즉, draw() 에 넣기 애매한 로직들을 update() 로직에 넣어주는 거라고 보면 됨.
 
 캐릭터 이동에 필요한 값인 charPos 를 계산해주는 로직의 경우
 이 부분에 넣어주는 게 적당하겠지.
 */
void ofApp::update(){
    if (walkRight) {
        // 오른쪽 화살표 키가 눌러진 상태임이 감지되면,
        // ofGetLastFrameTime() 이라는 델타 타임값을 리턴해주는 함수를 이용해서, x축 방향으로의 이동속도 값인 speed 를 구하고,
        // 이 값을 x컴포넌트로 갖는 vec3 값을 charPos 에 누적계산해 줌.
        float speed = 0.5 * ofGetLastFrameTime();
        charPos += glm::vec3(speed, 0, 0);
    }
    
    /**
     ofGetLastFrameTime() 에 대한 설명
     
     이전 프레임의 시작부터 현재 프레임 시작까지 경과한 시간값을 리턴해주는 오픈프레임웍스 내장함수.
     대부분의 그래픽 엔진에서는 이런 시간 간격값을 '델타타임' 이라고 부름.
     
     이런 걸 왜 받냐면, 디바이스나 디스플레이마다 Frame Rate 이 다르기 때문애
     렌더링 루프를 호출하는 간격이 전부 제각각임.
     
     어떤 거는 1초에 60번 호출하고, 어떤 거는 1초에 30번 호출하고 등등...
     
     그래서 이런 주사율에 관계없이 항상 일정하게 움직이는 속도를 유지하기 위해
     게임 쪽에서는 주로 이런 델타타임 값을 사용해서 속도를 맞춰준다고 함.
     
     자세한 설명은 p.131 참고
     */
}

//--------------------------------------------------------------
void ofApp::draw(){
    /**
     이전에 사용했던 블렌딩 모드를 비활성화함.
     
     지금 태양메쉬를 그리느라
     매 프레임마다 가산 블렌딩 모드를 활성화하고
     draw() 함수를 끝내고 있음.
     
     이렇게 되면 다음 프레임에서는
     이전 프레임에서 사용했던 가산 블렌딩 모드가
     그대로 적용되어 있기 때문에,
     다음 프레임에서 배경 및 캐릭터에도 가산 블렌딩 모드가
     적용되는 불상사가 발생함.
     
     그래서 엄청 밝아보임.
     
     그래서 새로운 프레임을 그리기 전
     이전 프레임에서 사용하던 모든 블렌딩 모드를 비활성화하는 것.
     
     왜냐면, 맨 초반에는 어차피 캐릭터, 배경 메쉬만 그릴거고,
     얘내는 알파테스팅만 적용하지 어떤 블렌딩도 필요 없기 때문임.
     
     이런 식으로, 알파 블렌딩 외에 복수 개의 블렌드 모드를
     사용할 경우, 다음 프레임을 그리기 전
     블렌딩 모드를 깔끔하게 초기화해준 뒤 그리기 시작하는 게 좋음.
     */
    ofDisableBlendMode();
    
    /**
     깊이테스트 활성화는 이미 setup() 함수에서 해줬지만,
     아래에서 태양메쉬가 구름메쉬의 투명픽셀에 가리는 현상을 해결하기 위해
     깊이테스트를 비활성화하고 draw() 함수가 끝났음.
     
     그런데 깊이테스팅도 블렌딩모드와 마찬가지로
     한 번 설정하고 나면 이후 드로우콜에서 GPU가 같은 설정을 유지함.
     
     그런데 캐릭터메쉬와 배경메쉬의 경우
     깊이버퍼가 존재해야, 즉 GPU에게 "캐릭터메쉬는 배경메쉬 앞에 있다!"
     라고 알려줘야 캐릭터메쉬가 묻히지 않고 제대로 렌더링되므로,
     
     캐릭터메쉬와 배경메쉬를 그리기 전에는
     ofEnableDepthTest(); 함수로
     깊이테스트를 다시 활성화시켜주는 게 맞겠지!
     */
    ofEnableDepthTest();
    
    // 텍스쳐 조절에 필요한 vec2 값과 offset 적용 시 얼마만큼 곱해서 더해줄 것인지, 그 frame 값을 매 프레임마다 계산해 줌.
    static float frame = 0.0; // 프레임 변수 초기화
    frame = (frame > 10) ? 0.0 : frame += 0.2; // draw() 함수가 1번 호출될 때마다 0.2씩 더해지니, frame의 정수부분이 변하려면 5번의 draw() 함수 호출, 즉 모니터 주사율이 60fps 라는 가정 하에, 약 1/60 * 5 = 1/30 = 0.0333...ms 즉, 33.3333...초에 한 번씩 frame의 정수부분이 바뀌므로, 프레임도 마찬가지로 33.333...초에 한 번씩 바뀔거임.
    glm::vec2 spriteSize = glm::vec2(0.28, 0.19); // spriteSheet 텍스쳐에서 프레임 하나만큼의 사이즈(너비, 높이 각각)에 해당하는 uv좌표를 지정해놓음. (버텍스 셰이더에서 0 ~ 1 사이의 uv 좌표값에 이만큼의 vec2()를 매번 곱해줘서 전체 텍스쳐 중에서 프레임 하나에 해당하는 사이즈 만큼의 부분의 uv좌표값만 넘겨주도록 함.)
    glm::vec2 spriteFrame = glm::vec2((int)frame % 3, (int)frame / 3);
    /**
     위에 spriteSize 에 곱한 다음 uv좌표에 추가적으로 더해줌으로써,
     offset을 적용하기 위해 구해주는 vec2 값.
     
     지금 (int)frame 으로 해주고 있는데,
     아마 frame 값의 정수부분만 가져오는 게 아닌가 싶음.
     
     그 정수값은 0 ~ 10 까지 있을거고,
     frame % 3 은 0 ~ 10 까지의 값을 3으로 나눈 나머지값이니 0, 1, 2 중 하나가 나올거고,
     frame / 3 은 0 ~ 10 까지의 값을 3으로 나는 몫이니 0, 1, 2, 3 중 하나가 나올거임.
     spriteFrame 의 x컴포넌트는 0, 1, 2 중 하나이고,
     y컴포넌트는 0, 1, 2, 3 중 하나일거임.
     
     지금 보면, spriteSheet 텍스쳐도 프레임이 x축 방향으로 3개로 나눠져있고 (0, 1, 2),
     y축 방향으로 4개로 나눠져있으므로(0, 1, 2, 3) 어떤 원리인지 대충 감이 오지?
     
     실제로 frame 이 0이면 spriteFrame = vec2(0, 0) 이므로, x축으로 0번째, y축으로 0번째 프레임이 그려지고,
     frame 이 1이면 spriteFrame = vec2(1, 0) 이므로, x축으로 1번째, y축으로 0번째 프레임이 그려지고,
     frame 이 2이면 spriteFrame = vec2(2, 0) 이므로, x축으로 2번째, y축으로 0번째 프레임이 그려지고,
     frame 이 3이면 spriteFrame = vec2(0, 1) 이므로, x축으로 0번째, y축으로 1번째 프레임이 그려지고,
     .....
     
     이런 식으로 쭉쭉 나가서 11개의 프레임이 33.3333.. 초마다 한 번씩 바뀌면서
     주기적으로 한 바퀴를 도는 식으로 스프라이트 시트 애니메이션을 그려내는 것임!
     */
    
    spritesheetShader.begin(); // spriteSheet.vert 라는 uv 사이즈 및 오프셋 조절용 셰이더를 새로 바인딩한 ofShader 객체를 사용해서 캐릭터를 그려줄거임.
    spritesheetShader.setUniform2f("size", spriteSize); // 위에서 계산한 spriteSize 값을 버텍스 셰이더의 size 유니폼변수로 넘겨줌
    spritesheetShader.setUniform2f("offset", spriteFrame); // 위에서 계산한 spriteFrame 값을 버텍스 셰이더의 offset 유니폼변수로 넘겨줌
    spritesheetShader.setUniformTexture("tex", alienImg, 0); // alphaTest.frag 셰이더에 존재하는 tex 라는 sampler2D 변수에 스프라이트 시트 텍스쳐를 넘겨줌.
    spritesheetShader.setUniform3f("translation", charPos); // update() 함수에서 계산되고 있는 움직임값인 charPos 를 버텍스 셰이더의 유니폼 변수로 넘겨줌.
    charMesh.draw(); // 캐릭터 메쉬를 그려줌.
    spritesheetShader.end(); // spriteSheetShader 셰이더 객체 사용을 중단함.
    
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
    
    // alphaTestShader.setUniformTexture("tex", alienImg, 0);
    // charMesh.draw();
    
    alphaTestShader.setUniformTexture("tex", backgroundImg, 0);
    backgroundMesh.draw();
    
    alphaTestShader.end(); // 캐릭터 메쉬와 배경메쉬까지 그리고, 현재 바인딩된 알파테스트 셰이더 사용을 중단하라고 gpu에게 명령함.
    
    /**
     반투명한 물체인 구름과 태양을 그릴 때에는
     항상 깊이테스트 (깊이 버퍼 저장)을 비활성화해줘야 함.
     
     왜냐하면, 반투명 물체인 구름 메쉬의 깊이 버퍼값을 가지고 있다면,
     더군다나 투명 픽셀들을 알파테스팅으로 discard 해주지도 않은 상태이면,
     p.109 에서 설명한 것처럼 구름메쉬의 투명한 픽셀들이 그 뒤에 있는 태양메쉬를 가리는
     기이한 현상이 발생함.
     
     왜냐? 깊이테스트가 setup() 함수에서 활성화되어있는 상태라서
     GPU가 '태양메쉬는 구름메쉬 뒤에 있구나' 라는 걸 알게 되고,
     그래서 구름메쉬의 투명 픽셀이 태양메쉬를 가리도록 한거임.
     
     그러면 구름메쉬를 그리기 전에
     GPU가 깊이 정보를 모르도록, 즉 깊이 테스트를 비활성화하면
     GPU는 깊이 정보를 알 수 없으니 drawcall 을 호출한 순서대로
     태양메쉬를 맨 앞에 그리고, 구름, 배경, 캐릭터메쉬들을 전부 다 뒤로 그려버림.
     
     그러니 구름의 투명픽셀이 태양메쉬를 더 이상 가리지 않을 수 있게 되는 것임.
     
     그래서 구름메쉬를 그리기 전 ofDisableDepthTest(); 함수로
     깊이테스트를 비활성화하는 것.
     
     -> 이런 식으로 반투명한 물체들 그리기 전에는 항상 깊이테스트 비활성화를
     습관적으로 해주는 게 좋음.
     */
    ofDisableDepthTest();
    
    // 구름메쉬를 드로우콜하여 그리기 전에, 블렌드 모드를 알파 블렌딩으로 설정해서 블렌딩 공식 설정함.
    ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ALPHA);
    
    // 새로운 알파블렌딩 셰이더를 사용하기 위해 gpu에게 cloudShader 에 로드된 셰이더를 바인딩하라고 명령함
    cloudShader.begin();
    
    // 해당 셰이더로 구름 텍스쳐를 전송한 뒤, 구름메쉬를 그려줌.
    cloudShader.setUniformTexture("tex", cloudImg, 0);
    // 알파 블렌딩의 원리 상, 반투명한 물체들은 항상 불투명한 물체들이 다 그려지고 난 뒤에 마지막에 그려줘야
    // 화면에 제대로 렌더링됨. 반투명한 프래그먼트가 백 버퍼에 먼저 저장되면 알파 블렌딩이 발생하지 않음. p.116 참고
    cloudMesh.draw();
    
    // 태양메쉬를 드로우콜하여 그리기 전에, 블렌드 모드를 가산 블렌딩으로 설정해서 블렌딩 공식 설정함.
    ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

    /**
     태양메쉬를 그릴 때 구름 셰이더를 그대로 사용함.
     이래도 되는 이유는, 태양 텍스쳐의 알파채널값이 너무 작아서
     구름 셰이더에서 사용하는 cutoff 기준값인 0.8에 안걸리기 때문.
     
     여기서 알 수 있는 점은, 셰이더 코드와
     opengl 에서 처리하는 블렌딩 처리 및 블렌딩 공식은 별개라는 점.
     
     실제로 p.115, 117 에 나온 블렌딩 공식을
     셰이더 코드에 사용한 적은 단 한 번도 없음.
     
     블렌딩 공식은 내가 블렌딩을 적용해서 그리려는 대상의
     드로우콜을 호출하기 전에 ofEnableBlendMode() 함수를 통해
     설정할 수 있음.
     
     별다른 설정을 하지 않는다면,
     오픈프레임웍스는 알파 블렌딩을 기본 설정으로 사용함.
     */
    cloudShader.setUniformTexture("tex", sunImg, 0);
    sunMesh.draw();
    
    cloudShader.end(); // 셰이더 사용을 끝냄.
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // 오른쪽 화살표 키 입력이 감지되면,
    // 움직임 값인 charPos 를 갱신하도록 walkRight 을 true로 변경함.
    if (key == ofKey::OF_KEY_RIGHT) {
        walkRight = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    // 오른쪽 화살표 키를 눌렀다가 떼는 동작이 감지되면, 화살표를 떼었으므로,
    // 더 이상 캐릭터메쉬가 움직이지 못하게 charPos 값 갱신하지 못하도록 walkRight 을 false로 변경함.
    if (key == ofKey::OF_KEY_RIGHT) {
        walkRight = false;
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
