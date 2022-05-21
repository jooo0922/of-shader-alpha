// 구름메쉬에 적용할 버텍스 셰이더를 새로 하나 파줌.

#version 410

layout(location = 0) in vec3 pos;
layout(location = 3) in vec2 uv;

out vec2 fragUV;

void main() {
  // 구름메쉬의 pos 값에 '곱해서' 구름메쉬 크기를 조절할 벡터값
  // x, y축 방향으로 0.5만큼 곱하므로, 구름메쉬는 가로, 세로 방향으로 절반만큼 줄어들거임.
  vec3 scale = vec3(0.5, 0.5, 1.0);

  /*
    원래는 아래의 벡터값을 
    구름메쉬를 생성할 때 메쉬의 중심점으로 잡고 만들어줬음.

    근데, 메쉬의 중심점과 오브젝트 공간의 원점이 다르면,
    scale 벡터를 곱해줘서 메쉬 크기를 조절할 때
    위치가 의도치않게 달라지는 문제가 발생함.

    그래서 일단 메쉬 생성할 때 (0, 0, 0)을 중심점으로 잡고,
    버텍스 셰이더에서 이동을 시켜주는 방식으로 해줘야
    크기도 조절할 수 있고, 위치도 원하는 곳에 배치할 수 있음.
  */
  vec3 translation = vec3(-0.55, 0.0, 0.0);

  /*
    지금처럼 위치와 scale 을 버텍스 셰이더에서 동시에
    변환하고자 하는 경우,

    항상 scale 벡터를 곱해줘서, 사이즈를 먼저 조절해준 다음,
    translation 을 더해줘서 위치를 이동시켜야 함.

    이 순서를 반드시 지켜야 함.
    그래야 사이즈가 조절되는 동안에는 
    메쉬의 중심점이 원점에 존재하기 때문이지.

    그리고, 이렇게 해줘야
    GPU 가 빠른 연산을 처리할 수 있는
    MAD 연산(곱한 다음 더한다)에도 부합한 순서로 계산할 수 있음.

    참고로, translation 을 더해주지 않고 빌드해보면
    구름메쉬가 오브젝트공간 정가운데, 즉 원점에 배치되는 것을 볼 수 있음.

    즉, scale 시 메쉬의 중심점이 오브젝트 공간의 원점과
    일치하는 상태이기 때문에 의도치않은 위치변동을 막을 수 있다는 뜻이지!
  */
  gl_Position = vec4((pos * scale) + translation, 1.0);

  fragUV = vec2(uv.x, 1 - uv.y);
}