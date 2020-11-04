# Laplacian filtering
	
	Laplacian 필터를 적용하여, 픽셀값의 변화가 갑자기 일어나는 부분을 찾아 바꿔주는 프로그램입니다.
	
## Image processing

이미지는 각 픽셀 값을 원소로 가지는 하나의 행렬로 생각할 수 있습니다.

이 때, 한 픽셀의 값에 대해 여러 연산을 적용할 수 있습니다.

* Point operation

	1. Binarization
		원본 이미지의 픽셀값 q(x, y)에 대해 threshold값 이상이면 밝기를 255로, 이하일 경우 밝기를 0으로 합니다.
		
	2. Inversion
		원본 이미지의 픽셀값 q(x, y)의 밝기 값을 반전시킵니다.
		
	3. Histogram stretching
		원본 이미지의 <strong>특정 구간</strong>의 픽셀값 q(x, y)의 밝기 값을 증가시키거나 감소합니다.


* Area operation

이미지의 각 픽셀 값을 주변의 인접한 픽셀(neighbor pixel)값과 필터 h에 대해 convolution 연산을 수행하는 방법입니다.

convolution : ![convolution equation](<img src="https://latex.codecogs.com/png.latex?q*h&space;=&space;\sum_{i&space;=&space;-\frac{m}{2}}^{\frac{m}{2}}\sum_{j&space;=&space;-\frac{m}{2}}^{\frac{m}{2}}h(j,&space;i)q(x&space;-&space;j,&space;y&space;-i&space;)" title="q*h = \sum_{i = -\frac{m}{2}}^{\frac{m}{2}}\sum_{j = -\frac{m}{2}}^{\frac{m}{2}}h(j, i)q(x - j, y -i )" />)

	

	
## Laplacian filtering

## 소스코드