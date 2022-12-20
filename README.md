<div align="center">
  
# 🔒 스마트한 도어락 🔑

#### 얼굴 인식 잠금해제 및 원격 제어 가능 도어락
<br>
<img src="https://github.com/manmenmi-nice/smart-doorlock/blob/GPGK-patch-1/%ED%91%9C%EC%A7%80.JPG">
<br>

<br>

## 📺Demo📺
🎞이미지 클릭하면 데모영상을 유튜브로 확인할 수 있습니다.

<br>

[![Video Label](http://img.youtube.com/vi/oTXYqvq-VLE/0.jpg)](https://youtu.be/oTXYqvq-VLE?t=0s)
<br>
📌 얼굴 인식

<br>

[![Video Label](http://img.youtube.com/vi/_aF4jKydWK8/0.jpg)](https://youtu.be/_aF4jKydWK8?t=0s)
<br>
📌 블루투스 통신

<br>

[![Video Label](http://img.youtube.com/vi/mKcttgKRzoE/0.jpg)](https://youtu.be/mKcttgKRzoE?t=0s)
<br>
📌 환경 밝기 인식



<br>

## 🛠️ Using Tools 🛠️
<img src="https://img.shields.io/badge/-Embedded%20System-blue?style=for-the-badge">
<img src="https://img.shields.io/badge/-C--language-brightgreen?style=for-the-badge">
<img src="https://img.shields.io/badge/-Raspberry--Pi-yellow?style=for-the-badge">
<img src="https://img.shields.io/badge/-Python-orange?style=for-the-badge">

<br>
<br>

## 시스템 구성도
<img src="https://github.com/manmenmi-nice/smart-doorlock/blob/GPGK-patch-1/architecture.png">

<br>
<br>

## 회로 구성도
<img src="https://github.com/manmenmi-nice/smart-doorlock/blob/GPGK-patch-1/System_Structural_Diagram.png">

<br>
<br>

## 작동 기능


### ⚙️ 다중 작업 ⚙️

|기능명|적용내역|설명
|---|---|---|
|멀티 프로세스|OLED, 부저, 서브모터, stepper모터|프로세스 생성해서 프로세스끼리 통신|
|멀티 쓰레드|블루투스, 카메라|지속적인 수신 대기 필요시 사용|
|IPC|Message Queue|파이썬 얼굴인식 프로세스와 메인프로세스 간 통신|
|Mutex|문열림 상태, 프로그램 종료 상태|스레드간 데이터 공유 자원을 접근시 사용|

<br>

### 🔑 얼굴 인식 및 잠금 🔑

|기능명|센서|통신|설명
|---|---|---|---|
|얼굴 인식|카메라|CSI|얼굴 인식, 올바른 사람 인지할 시 잠금 해제|
||오픈소스 활용||https://github.com/ageitgey/face_recognition|
|접근 확인|초음파 센서|GPIO|사람 접근 확인 및 거리에 따른 카메라 ON/OFF|
|잠금 장치 작동|Servo 모터|PWM|모터를 통한 잠금 / 잠금해제|


### 📱 무선 데이터 통신 📱

|기능명|센서|통신|설명
|---|---|---|---|
|무선 데이터 통신|Bluetooth|UART|잠금 장치 원격제어 및 알림전송|


### 🚪 문 작동 🚪

|기능명|센서|통신|설명
|---|---|---|---|
|문 열림 / 닫힘|Stepper 모터|GPIO|모터를 통한 열림 / 닫힘|


### 🐈 보조 기능 🐈

|기능명|센서|통신|설명
|---|---|---|---|
|모니터 출력|OLED Display|I2C|문 상태, 안내문구 등을 출력|
||드라이버 활용||Adafruit ssd 1306 Driver|
|야간 조명 밝기|조도센서|I2C|주변 밝기 인식|
||LED||어두움 감지시 LED 작동|
||ADC||아날로그 -> 디지털 신호 변환|
|스피커|부저|PWM|안내 소리 출력|

<br>
<br>

## 실행 방법
|동작|명령어
|---|---|
|빌드|sh ./build.sh|
|실행|sudo ./smart-doorlock|

<br>
<br>

©️ 금오공과대학교 임베디드 시스템 2조 - 만멘미 나이스
