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

void open_door();  // open door
void close_door(); // close door
void set_done(int value);
int get_done();

int music(int stat);
void door(int stat);
void lock(int stat);
void oled(int stat); // open : 2, close : 1
void* bluetooth(void* argv);
void init();

int done;
int door_status;
pthread_mutex_t lock_done;
pthread_mutex_t lock_door;

pthread_t child_thread;

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

int main(){
    recognize_setOnDoneCallback(face_recognition_cb);
    recognize_init();
	init();
	int rc;

	rc = pthread_create(&child_thread,NULL,bluetooth,NULL);
	if(rc){
		perror("Bluetooth Thread Creation failed...\n");
		pthread_exit(NULL);
	}

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

    pthread_join(child_thread, NULL);

    recognize_release();

	printf("[main] goodbye.\n");

	return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#define BAUD_RATE 115200
static const char* UART2_DEV = "/dev/ttyAMA1"; //UART2 연결을 위한 장치 파일
unsigned char serialRead(const int fd); //1Byte 데이터를 수신하는 함수
void serialWrite(const int fd, const unsigned char c); //1Byte 데이터를 송신하는 함수
//1Byte 데이터를 수신하는 함수
unsigned char serialRead(const int fd)
{
	unsigned char x;
	if(read (fd, &x, 1) != 1) //read 함수를 통해 1바이트 읽어옴
		return -1;
	return x; //읽어온 데이터 반환
}
//1Byte 데이터를 송신하는 함수
void serialWrite(const int fd, const unsigned char c)
{
	write (fd, &c, 1); //write 함수를 통해 1바이트 씀
}
void* bluetooth(void* argv){
    int fd_serial ; //UART2 파일 서술자
    unsigned char dat; //데이터 임시 저장 변수
    if (wiringPiSetup () < 0) return NULL;
    if ((fd_serial = serialOpen (UART2_DEV, BAUD_RATE)) < 0){ //UART2 포트 오픈
        printf ("Unable to open serial device.\n") ;
        return NULL;
    }
    char text[1024];
    int num = 0;
    while(1){
        if(get_done() == 1) break;
        memset(text,0,sizeof(text));
        num = 0;

        while(serialDataAvail (fd_serial) ){ //읽을 데이터가 존재한다면,
            text[num++] = serialRead (fd_serial); //버퍼에서 1바이트 값을 읽음
        }

        if (!text[0]) continue;

        text[num] = "\0";
        printf("[Bluetooth] got %s\n", text);
        if(strcmp(text, "open") == 0)       open_door();
        else if(strcmp(text, "close") == 0) close_door();
        else if(strcmp(text, "bye") == 0)   set_done(1);
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

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
    initCDS();
    initUltrasonic();
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