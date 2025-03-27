# Signed Distance Field
부호있는 거리장이라는 의미, 표면과의 거리값을 미리 계산하여 영역 내에 저장함.</br>
표면과의 거리를 미리 계산해 놓았기에 물체의 내 외부를 더욱 더 빠르게 빠르게 판단할 수 있음.</br>
SDF로부터 계산된 표면과의 거리값 distance의 부호에 따라 특정 지점이 물체의 외부 내부를 판단 가능함.</br>

만약 SDF의 특정 지점이 distance > 0 일 경우</br>
그 지점은 물체 밖이라는 의미.</br>

만약 SDF의 특정 지점이 distance = 0 일 경우</br>
그 지점은 물체의 표면 위라는 의미</br>

만약 SDF의 특정 지점이 distance < 0 일 경우</br>
그 지점은 물체의 내부라는 의미</br>

## SDF 생성 방법
SDF는 QuadTree로 구성되며, BFS 방식으로 만들어 낼 수 있음</br>
노드를 분할할 때, Node의 깊이가 임계 깊이보다 낮고 물체의 경계 부분일 경우 노드를 4분함.</br>
노드 하나에는 4개의 코너값이 저장되는데, 각 코너값은 물체로부터의 거리값을 가짐</br>
즉, 노드를 분할하는 과정 중 코너값으로부터 물체의 거리를 계산하여 저장함.</br>

아래는 생성된 SDFTree를 시각화 한 것,</br>
![SDFTree](https://github.com/user-attachments/assets/4adab3bd-55a1-44f5-9b72-c995b0e11559)</br>

## SDF의 응용
SDF를 활용하면 물체의 내 외부를 빠르게 판단할 수 있다는 장점이 있음.</br>
즉 이를 응용하면, 두 물체간 충돌 여부를 빠르게 판단할 수 있다는 의미임.</br>
실제로 SDF값을 미리 계산하여 구워낸 다음, 이를 선형 보간법으로 물체의 내 외부를 판단하는 기법이 주로 사용됨</br> 
이는 게임 물리 시뮬레이션이나, 로보틱스, 의학 CT MRI 사진에서의 볼륨 렌더링 등 여러 분야에서 활용될 수 있음</br>

여기서는 간단한 로프와 원형 물체의 충돌 시뮬레이션을 소개함</br>

### SDF 충돌 예제

SDF 충돌 알고리즘</br>

1. 특정 물체가 SDF 내로 들어 온 여부를 SDFTree를 BFS로 탐색하면서 확인한다.</br>
2. 만약 물체가 SDF에 들어왔을 경우, 물체가 존재하는 SDF Node를 반환한다.</br>
3. 물체 Node에서 물체의 x, y 좌표와 SDF 코너값간의 선형 보간을 수행해 물체 표면까지의 거리를 구해낸다.</br>
  3 - 1. 만약 3D 환경이라면, Trilinear Interpolation을 수행한다.</br>
  3 - 2. 만약 2D 환경이라면, Bilinear Interpolation을 수행한다.</br>
4. 구해낸 SDF값을 기준으로 Δd 만큼 유한차분법을 수행해 물체의 표면을 향하는 벡터인 SDF Gradient를 구해낸다.</br>
5. SDF Gradient값 만큼 충돌된 물체를 위치 보정을 하여 물체가 충돌한 물체를 관통하지 못하게 한다.<br>

위 알고리즘대로 수행한 결과는 아래와 같음</br>

![_2025_03_27_13_00_14_567-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/23842960-fa0c-4550-9b2a-7190065c5f56)</br>

## ADF (Adaptive Signed Distance Field)

SDF의 분할 과정에서 실제 Distance값과 계산된 코너값의 Distance값의 오차를 계산하여,</br>
만약 그 오차가 임계 범위 안에 있을 경우 노드를 분할하지 않은 SDF, </br>
오차를 보면서 분할하기 때문에, 계산량은 많지만,</br>
경계 부분을 무조건 분할하지 않기 때문에 메모리 효율이 더 높음,</br>
그리고 ADF를 통해 계산된 distance 값은 SDF로부터 계산된 distance와 큰 차이를 보이지 않음.</br>

아래는 생성된 ADFTree를 시각화 한 것,</br>
![image](https://github.com/user-attachments/assets/8ad46e8e-7108-4f96-88ce-70b61f7f3a17)</br>
기존의 SDFTree의 노드 수보다 ADF의 노드 수가 확연이 적다는 것을 볼 수 있음.</br>
그렇기에 메모리 효율에서 ADF가 더 우월함.</br>
