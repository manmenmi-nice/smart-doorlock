<div align="center">
  
# ๐ ์ค๋งํธํ ๋์ด๋ฝ ๐

#### ์ผ๊ตด ์ธ์ ์ ๊ธํด์  ๋ฐ ์๊ฒฉ ์ ์ด ๊ฐ๋ฅ ๋์ด๋ฝ
<br>
<img src="https://github.com/manmenmi-nice/smart-doorlock/blob/GPGK-patch-1/%ED%91%9C%EC%A7%80.JPG">
<br>

<br>

## ๐บDemo๐บ
๐์ด๋ฏธ์ง ํด๋ฆญํ๋ฉด ๋ฐ๋ชจ์์์ ์ ํ๋ธ๋ก ํ์ธํ  ์ ์์ต๋๋ค.

<br>

[![Video Label](http://img.youtube.com/vi/oTXYqvq-VLE/0.jpg)](https://youtu.be/oTXYqvq-VLE?t=0s)
<br>
๐ ์ผ๊ตด ์ธ์

<br>

[![Video Label](http://img.youtube.com/vi/_aF4jKydWK8/0.jpg)](https://youtu.be/_aF4jKydWK8?t=0s)
<br>
๐ ๋ธ๋ฃจํฌ์ค ํต์ 

<br>

[![Video Label](http://img.youtube.com/vi/mKcttgKRzoE/0.jpg)](https://youtu.be/mKcttgKRzoE?t=0s)
<br>
๐ ํ๊ฒฝ ๋ฐ๊ธฐ ์ธ์



<br>

## ๐ ๏ธ Using Tools ๐ ๏ธ
<img src="https://img.shields.io/badge/-Embedded%20System-blue?style=for-the-badge">
<img src="https://img.shields.io/badge/-C--language-brightgreen?style=for-the-badge">
<img src="https://img.shields.io/badge/-Raspberry--Pi-yellow?style=for-the-badge">
<img src="https://img.shields.io/badge/-Python-orange?style=for-the-badge">

<br>
<br>

## ์์คํ ๊ตฌ์ฑ๋
<img src="https://github.com/manmenmi-nice/smart-doorlock/blob/GPGK-patch-1/architecture.png">

<br>
<br>

## ํ๋ก ๊ตฌ์ฑ๋
<img src="https://github.com/manmenmi-nice/smart-doorlock/blob/GPGK-patch-1/System_Structural_Diagram.png">

<br>
<br>

## ์๋ ๊ธฐ๋ฅ


### โ๏ธ ๋ค์ค ์์ โ๏ธ

|๊ธฐ๋ฅ๋ช|์ ์ฉ๋ด์ญ|์ค๋ช
|---|---|---|
|๋ฉํฐ ํ๋ก์ธ์ค|OLED, ๋ถ์ , ์๋ธ๋ชจํฐ, stepper๋ชจํฐ|ํ๋ก์ธ์ค ์์ฑํด์ ํ๋ก์ธ์ค๋ผ๋ฆฌ ํต์ |
|๋ฉํฐ ์ฐ๋ ๋|๋ธ๋ฃจํฌ์ค, ์นด๋ฉ๋ผ|์ง์์ ์ธ ์์  ๋๊ธฐ ํ์์ ์ฌ์ฉ|
|IPC|Message Queue|ํ์ด์ฌ ์ผ๊ตด์ธ์ ํ๋ก์ธ์ค์ ๋ฉ์ธํ๋ก์ธ์ค ๊ฐ ํต์ |
|Mutex|๋ฌธ์ด๋ฆผ ์ํ, ํ๋ก๊ทธ๋จ ์ข๋ฃ ์ํ|์ค๋ ๋๊ฐ ๋ฐ์ดํฐ ๊ณต์  ์์์ ์ ๊ทผ์ ์ฌ์ฉ|

<br>

### ๐ ์ผ๊ตด ์ธ์ ๋ฐ ์ ๊ธ ๐

|๊ธฐ๋ฅ๋ช|์ผ์|ํต์ |์ค๋ช
|---|---|---|---|
|์ผ๊ตด ์ธ์|์นด๋ฉ๋ผ|CSI|์ผ๊ตด ์ธ์, ์ฌ๋ฐ๋ฅธ ์ฌ๋ ์ธ์งํ  ์ ์ ๊ธ ํด์ |
||์คํ์์ค ํ์ฉ||https://github.com/ageitgey/face_recognition|
|์ ๊ทผ ํ์ธ|์ด์ํ ์ผ์|GPIO|์ฌ๋ ์ ๊ทผ ํ์ธ ๋ฐ ๊ฑฐ๋ฆฌ์ ๋ฐ๋ฅธ ์นด๋ฉ๋ผ ON/OFF|
|์ ๊ธ ์ฅ์น ์๋|Servo ๋ชจํฐ|PWM|๋ชจํฐ๋ฅผ ํตํ ์ ๊ธ / ์ ๊ธํด์ |


### ๐ฑ ๋ฌด์  ๋ฐ์ดํฐ ํต์  ๐ฑ

|๊ธฐ๋ฅ๋ช|์ผ์|ํต์ |์ค๋ช
|---|---|---|---|
|๋ฌด์  ๋ฐ์ดํฐ ํต์ |Bluetooth|UART|์ ๊ธ ์ฅ์น ์๊ฒฉ์ ์ด ๋ฐ ์๋ฆผ์ ์ก|


### ๐ช ๋ฌธ ์๋ ๐ช

|๊ธฐ๋ฅ๋ช|์ผ์|ํต์ |์ค๋ช
|---|---|---|---|
|๋ฌธ ์ด๋ฆผ / ๋ซํ|Stepper ๋ชจํฐ|GPIO|๋ชจํฐ๋ฅผ ํตํ ์ด๋ฆผ / ๋ซํ|


### ๐ ๋ณด์กฐ ๊ธฐ๋ฅ ๐

|๊ธฐ๋ฅ๋ช|์ผ์|ํต์ |์ค๋ช
|---|---|---|---|
|๋ชจ๋ํฐ ์ถ๋ ฅ|OLED Display|I2C|๋ฌธ ์ํ, ์๋ด๋ฌธ๊ตฌ ๋ฑ์ ์ถ๋ ฅ|
||๋๋ผ์ด๋ฒ ํ์ฉ||Adafruit ssd 1306 Driver|
|์ผ๊ฐ ์กฐ๋ช ๋ฐ๊ธฐ|์กฐ๋์ผ์|I2C|์ฃผ๋ณ ๋ฐ๊ธฐ ์ธ์|
||LED||์ด๋์ ๊ฐ์ง์ LED ์๋|
||ADC||์๋ ๋ก๊ทธ -> ๋์งํธ ์ ํธ ๋ณํ|
|์คํผ์ปค|๋ถ์ |PWM|์๋ด ์๋ฆฌ ์ถ๋ ฅ|

<br>
<br>

## ์คํ ๋ฐฉ๋ฒ
|๋์|๋ช๋ น์ด
|---|---|
|๋น๋|sh ./build.sh|
|์คํ|sudo ./smart-doorlock|

<br>
<br>

ยฉ๏ธ ๊ธ์ค๊ณต๊ณผ๋ํ๊ต ์๋ฒ ๋๋ ์์คํ 2์กฐ - ๋ง๋ฉ๋ฏธ ๋์ด์ค
