# Error diffusion dithering

Lenna image에 대해 openCV 라이브러리를 이용하여, Lenna image를 읽어 들이고, dithering을 적용하기 전과 적용한 후에 대한 결과를 표시하는
프로그램입니다.

## Dithering

이론

특정 디바이스에서는 성능이 부족해서, 원이미지나 원영상을 똑같이 출력하기가 힘든 경우가 있습니다. 가령 예를 들어, 흑백 프린터기난 휴대용 게임기와 같은 디바이스는 성능이 제약되어
original media의 모든 색상이 사용이 불가능합니다.

이런 경우, 다른 색상이나 픽셀 값을 섞어서 시각적으로 비슷한 색상을 내도록하는 것이 dithering입니다.

알고리즘 전개 방식은 하나의 픽셀 값은 2by2, 또는 4by4 더 큰 픽셀 패턴으로 바꾸는 것입니다.

Example) 2bits 색상 값(0, 1, 2, 3)을 가질 수 있는 경우 아래와 같은 dithering matrix를 이용하여 표현 할 수 있습니다.

(https://latex.codecogs.com/png.latex?\inline&space;\begin{bmatrix}&space;0&space;&&space;2\\&space;3&space;&&space;1&space;\end{bmatrix})

## Error diffusion dithering

dithering시 발생하는 픽셀 값에 대한 quantization error를 줄이는 방법입니다.

어느 픽셀에서 발생한 error(noise)를 주변 픽셀로 확산시킵니다.


이론

## 소스코드

