#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>

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

int main ()
{
	int fd_serial ; //UART2 파일 서술자
	unsigned char dat; //데이터 임시 저장 변수
	if (wiringPiSetup () < 0) return 1 ;
	if ((fd_serial = serialOpen (UART2_DEV, BAUD_RATE)) < 0){ //UART2 포트 오픈
		printf ("Unable to open serial device.\n") ;
		return 1 ;
	}
	while(1){
		if(serialDataAvail (fd_serial) ){ //읽을 데이터가 존재한다면,
			dat = serialRead (fd_serial); //버퍼에서 1바이트 값을 읽음
			printf ("%c", dat) ;
			fflush (stdout) ;
			serialWrite(fd_serial, dat); //입력 받은 데이터를 다시 보냄 (Echo)
		}
		delay (10);
	}
}

