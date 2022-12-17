#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <wiringPi.h>
#include "../face_recognition/recognize.h"
#include "../brightness/cds.h"
#include "../ultrasonic/ultrasonic.h"
#include "../bluetooth/bluetooth.h"
#include "../oled/oled.h"
#include "../servo/lock.h"
#include "../tone/music.h"
#include "../stepper/door.h"

#define LED 26

void open_door();  // open door
void close_door(); // close door
void set_done(int value);
int get_done();

void init();

int done;
int door_status;
pthread_mutex_t lock_done;
pthread_mutex_t lock_door;
int isRecognizerAvailable = 1;

void face_recognition_cb(int result){
    switch (result){
        case 0:
            printf("[callback] Fail\n");
            oled_set(OLED_RECOGNITION_FAILED);
            isRecognizerAvailable = 1;
            break;
        case 1:
            printf("[callback] OK\n");
            open_door();
            isRecognizerAvailable = 1;
            break;
        case 2:
            printf("[callback] Photo taken\n");
            oled_set(OLED_RECOGNIZING);
            break;
    }
}

void bluetooth_on_message_cb(const char* msg){
    printf("[bluetooth_callback] message: %s\n", msg);
    if(msg[0]=='o')        open_door();
    else if(msg[0]=='c')   close_door();
    else if(msg[0]=='b')   set_done(1);
}

int main(){
    wiringPiSetupGpio();
    pinMode(LED, OUTPUT);

    recognize_setOnDoneCallback(face_recognition_cb);
    bluetooth_setOnMessageCallback(bluetooth_on_message_cb);

    recognize_init();

    printf("[main] Waiting for face recognizer initialization... (35s)\n");
    sleep(35);

    bluetooth_init();
    cds_init();
    ultrasonic_init();

    init();

    while(1){
        int brightness = cds_getBrightness();
        //printf("[main] Brightness: %d. ", brightness);

        if(brightness < 150){ // Bright!
            //printf("Bright!\n");
            digitalWrite(LED, HIGH);
        }else{ // Dark!
            //printf("Dark!\n");
            digitalWrite(LED, LOW);
        }

        if (isRecognizerAvailable) {
            int distance = ultrasonic_getDistance();
            //printf("[main] Distance: %d\n", distance);
            if (ultrasonic_getDistance() < 10) {
                delay(100);
                if (ultrasonic_getDistance() < 10) {
                    printf("[main] Obstacle detected\n");
                    oled_set(OLED_TAKING_PHOTO);
                    sleep(1);
                    oled_set(OLED_TAKING_PHOTO_3);
                    sleep(1);
                    oled_set(OLED_TAKING_PHOTO_2);
                    sleep(1);
                    oled_set(OLED_TAKING_PHOTO_1);
                    sleep(1);
                    recognize_start();
                    isRecognizerAvailable = 0;
                }
            }
        }

        if(get_done() == 1) break;

    }

    recognize_release();

    printf("[main] goodbye.\n");

    return 0;
}

void open_door(){
    pthread_mutex_lock(&lock_door);
    if(door_status == 0){
        oled_set(OLED_UNLOCKED);
        lock(1);

        pid_t pid = music(MUSIC_DOOR_OPEN);
        door(0);
        waitpid(pid, NULL, 0);

        door_status = 1;
    }
    pthread_mutex_unlock(&lock_door);
}

void close_door(){
    pthread_mutex_lock(&lock_door);
    if(door_status == 1){
        lock(1);

        pid_t pid = music(MUSIC_DOOR_CLOSE);
        door(1);
        waitpid(pid, NULL, 0);

        lock(1);
        lock(0);
        oled_set(OLED_LOCKED);
        door_status = 0;
    }
    pthread_mutex_unlock(&lock_door);
}


void init(){
    done = 0;
    door_status = 0;
    pthread_mutex_init(&lock_done, NULL);
    pthread_mutex_init(&lock_door, NULL);
}

void set_done(int value){
    pthread_mutex_lock(&lock_done);
    done = value;
    pthread_mutex_unlock(&lock_done);
}

int get_done(){
    int value = 0;
    pthread_mutex_lock(&lock_done);
    value = done;
    pthread_mutex_unlock(&lock_done);
    return value;
}