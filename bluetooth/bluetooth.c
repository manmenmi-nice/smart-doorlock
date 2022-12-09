//
// Created by dfkdream on 22. 12. 9.
//

#include "bluetooth.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>

BLUETOOTH_ONMESSAGE_CB bluetooth_cb;
int fd_bluetooth;

#define BAUD_RATE 115200
static const char* UART2_DEV = "/dev/ttyAMA1"; //UART2 연결을 위한 장치 파일

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

void* bluetoothWorker(void* argv){
    printf("[Bluetooth.bluetoothWorker] bluetoothWorker started\n");
    char text[1024];
    int num = 0;

    while(1){
        memset(text,0,sizeof(text));
        num = 0;

        while(serialDataAvail (fd_bluetooth) ){ //읽을 데이터가 존재한다면,
            text[num++] = serialRead (fd_bluetooth); //버퍼에서 1바이트 값을 읽음
        }

        if (!text[0]) continue;

        text[num] = "\0";
        printf("[Bluetooth.bluetoothWorker] got %s\n", text);

        if (bluetooth_cb) bluetooth_cb(text);
    }
}

pthread_t bluetooth_init(){
    if (wiringPiSetupGpio () < 0) return 0;
    if ((fd_bluetooth = serialOpen (UART2_DEV, BAUD_RATE)) < 0){ //UART2 포트 오픈
        printf ("[Bluetooth] Unable to open serial device.\n") ;
        return 0;
    }

    pthread_t tid;
    int rc = pthread_create(&tid,NULL,bluetoothWorker,NULL);
    if(rc){
        perror("[Bluetooth] Failed to create Bluetooth Thread: \n");
        pthread_exit(NULL);
    }

    return tid;
}

void bluetooth_setOnMessageCallback(BLUETOOTH_ONMESSAGE_CB cb){
   bluetooth_cb = cb;
}