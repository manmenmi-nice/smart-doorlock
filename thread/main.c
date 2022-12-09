#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <wiringSerial.h>
#include <string.h>
#include <sys/wait.h>
#include "../face_recognition/recognize.h"
#include "../brightness/cds.h"
#include "../ultrasonic/ultrasonic.h"
#include "../bluetooth/bluetooth.h"

void open_door();  // open door
void close_door(); // close door
void set_done(int value);
int get_done();

int music(int stat);
void door(int stat);
void lock(int stat);
void oled(int stat); // open : 2, close : 1
void init();

int done;
int door_status;
pthread_mutex_t lock_done;
pthread_mutex_t lock_door;

void face_recognition_cb(int result){
    switch (result){
        case 0:
            printf("[callback] Fail\n");
            // TODO: OLED 업데이트
            break;
        case 1:
            printf("[callback] OK\n");
            // TODO: OLED 업데이트
            // TODO: 문 열기
            break;
        case 2:
            printf("[callback] Photo taken\n");
            // TODO: OLED 업데이트
            break;
    }
}

void bluetooth_on_message_cb(const char* msg){
    printf("[bluetooth_callback] message: %s", msg);
    if(strcmp(msg, "open") == 0)       open_door();
    else if(strcmp(msg, "close") == 0) close_door();
    else if(strcmp(msg, "bye") == 0)   set_done(1);
}

int main(){
    recognize_setOnDoneCallback(face_recognition_cb);
    bluetooth_setOnMessageCallback(bluetooth_on_message_cb);

    recognize_init();
    bluetooth_init();
    initCDS();
    initUltrasonic();

	init();

	while(1){
		{ // test
            recognize_start();
			open_door();
			close_door();
			set_done(1);
		}

        int brightness = getBrightness();
        printf("[main] Brightness: %d. ", brightness);

        if(brightness < 150){ // Bright!
            printf("Bright!\n");
            // TODO: LED 끄기
		}else{ // Dark!
            printf("Dark!\n");
            // TODO: LED 켜기
		}

        int distance = getDistance();
        printf("[main] Distance: %d. ", distance);
		if(getDistance() < 100){
            printf("Obstacle detected\n");
            // TODO: 디바운스 로직 추가
            // TODO: 대기 및 사진 촬영 요청 추가
            // TODO: OLED 업데이트
		}else
            printf("\n");

        sleep(10); // 얼굴 인식 완료시까지 대기

		if(get_done() == 1) break;
	}

    recognize_release();

	printf("[main] goodbye.\n");

	return 0;
}

void open_door(){
	pthread_mutex_lock(&lock_door);
	if(door_status == 0){
		oled(2);
		lock(1);
		door(0);
		door_status = 1;
	}
	pthread_mutex_unlock(&lock_door);
}

void close_door(){
	pthread_mutex_lock(&lock_door);
	if(door_status == 1){
		lock(1);
		door(1);
		lock(1);
		lock(0);
		oled(1);
		door_status = 0;
	}
	pthread_mutex_unlock(&lock_door);
}

int music(int stat){
	pid_t pid = fork();
	int status;
	if(pid>0){
		return pid;
		//waitpid(pid, &status, 0);
		//printf("(music %d) complete\n", stat);
	}
	else if(pid == 0){
		char buf[1024];
		sprintf(buf,"%d",stat); 
		char* cmd[] = {"tone", buf, NULL};
		execv(cmd[0], cmd);
	}
	else{
		printf("(music) fork error...\n");
	}
}

void door(int stat){
	pid_t pid = fork();
	int status;
	if(pid>0){
		pid_t pid2 = music(stat);
		waitpid(pid, &status, 0);
		waitpid(pid2, &status, 0);
		// printf("(door %d) complete\n", stat);
	}
	else if(pid == 0){
		char* cmd[] = {"stepper", "90", stat==0?"0":"1", NULL};
		execv(cmd[0], cmd);
	}
	else{
		printf("(door) fork error...\n");
	}
}

void lock(int stat){
	pid_t pid = fork();
	int status;
	if(pid>0){
		usleep(800000);
		waitpid(pid, &status, 0);
		// printf("(lock %d) complete\n", stat);
	}
	else if(pid == 0){
		char* cmd[] = {"servo", stat==0?"90":"0", NULL};
		execv(cmd[0], cmd);
	}
	else{
		printf("(lock) fork error...\n");
	}
}

void oled(int stat){
	pid_t pid = fork();
	int status;
	if(pid>0){
		waitpid(pid, &status, 0);
		// printf("(oled %d) complete\n", stat);
	}
	else if(pid == 0){
		char buf[1024];
		sprintf(buf,"%d",stat); 
		char* cmd[] = {"oled", buf, NULL};
		execv(cmd[0], cmd);
	}
	else{
		printf("(oled) fork error...\n");
	}
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