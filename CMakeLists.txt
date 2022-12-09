cmake_minimum_required(VERSION 3.18.4)
project(smart_doorlock C)

set(CMAKE_C_STANDARD 11)

add_executable(smart_doorlock thread/main.c
        face_recognition/recognize.c
        bluetooth/bluetooth.c
        brightness/cds.c
        ultrasonic/ultrasonic.c
        oled/oled.c)

add_executable(tone tone/tone.c)
add_executable(stepper stepper/stepper.c)
add_executable(servo servo/servo.c)
add_executable(oled oled/src/oled.c oled/src/ssd1306_i2c.c)

target_link_libraries(smart_doorlock wiringPi pthread rt)
target_link_libraries(tone wiringPi pthread)
target_link_libraries(stepper wiringPi)
target_link_libraries(servo wiringPi)
target_link_libraries(oled wiringPi)
