1. rotate_skeleton_v2.cpp

- Purpose of this code
소스코드와 같은 위치에 존재하는 image 파일을 읽어들여 입력된 회전 각도만큼 회전해 window 창으로 보여주는 code이다. 

- How to run this code
원하는 이미지를 코드와 같은 파일경로에 저장한뒤, imread로 Mat 형식으로 변환해준다.
함수 myrotate(const Mat input, float angle, const char* opt)로 호출하면 회전된 이미지(Mat)가 반환된다.

- How to adjust parameters (if any)
- How to define default parameters
angle : float 타입, 회전을 원하는 각도를 입력
opt : interpolation 타입으로, "nearest" or "bilinear"으로 2가지 방법을 지원한다.

2.Stitching.cpp

- Purpose of this code
소스코드와 같은 위치에 존재하는 이미지 2개(left, right)와 2개의 이미지에 매칭되는 픽셀값들을
읽어들여 right이미지를 left 이미지에 맞춰 affine을 진행하고
이미지의 겹치는 부분을 blending하여 합친 이미지 1개를 window 창으로 보여주는 code이다.

- How to run this code
원하는 두 이미지를 코드와 같은 파일경로에 저장한뒤 cal_affine 함수를 호출해 affine transformation matrix를 생성해 affine된 이미지를 도출해낸다.
affine된 이미지와 left이미지를 합치는 blend_stitching 함수를 호출하여 하나의 합쳐진 이미지를 만들어낸다.

- How to adjust parameters (if any)
- How to define default parameters
2개의 이미지에 매칭되는 픽셀값들은 적어도 3개이상이 필요하다. 디폴트는 28개로 설정되어있다.
또한 affine된 이미지를 생성하는 과정에서 bilinear interpolation으로 구현되었으며,
이미지를 합치는 blend_stitching에서의 alpha값은 0.5가 디폴트로 설정되어있다.