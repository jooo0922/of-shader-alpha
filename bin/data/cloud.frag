#version 410

uniform sampler2D tex;
in vec2 fragUV;
out vec4 outCol;

void main(){
  outCol = texture(tex, fragUV);

  /*
    위에 까지는 알파테스트 셰이더와 구조가 거의 비슷함.

    다만 알파블렌딩 셰이더에서는
    cutoff 기준값을 넘는 프래그먼트들에 대해
    discard 처리를 해버리는 것이 아니라, 

    알파채널의 값이 1인 프래그먼트들, 즉 배경을 제외한
    구름 영역 프래그먼트들의 투명도를 0.8로 떨어트림으로써,
    '반투명'한 프래그먼트들을 만들어서 
    다음 파이프라인으로 넘겨줌. 
  */
  if (outCol.a > 0.8) {
    outCol.a = 0.8;
  }
}