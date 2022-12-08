#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#define BAUD_RATE 115200 //블루투스의 보율이 바뀔 경우 이 값을 변경해야함
static const char* UART2_DEV = "/dev/ttyAMA1"; //UART2 연결을 위한 장치 파일

unsigned char serialRead(const int fd);
void serialWrite(const int fd, const unsigned char c);
void serialWriteBytes (const int fd, const char *s);

//여러 바이트의 데이터를 씀
void serialWriteBytes (const int fd, const char *s)
{
	write (fd, s, strlen (s)) ;
}

//1바이트 데이터를 읽음
unsigned char serialRead(const int fd)
{
	unsigned char x;
	if(read (fd, &x, 1) != 1) //read 함수를 통해 1바이트 읽어옴
		return -1;
	return x; //읽어온 데이터 반환
}

//1바이트 데이터를 씀
void serialWrite(const int fd, const unsigned char c)
{
	write (fd, &c, 1); //write 함수를 통해 1바이트 씀
}

int main ()
{
	int fd_serial ; //디바이스 파일 서술자
	unsigned char dat; //임시 데이터 저장 변수
	char buf[100]; //명령어 저장 변수
	if (wiringPiSetup () < 0) return 1 ;
	if ((fd_serial = serialOpen (UART2_DEV, BAUD_RATE)) < 0) //UART2 포트 오픈
	{
		printf ("Unable to open serial device.\n") ;
		return 1 ;
	}while(1){
		printf("Enter the AT Command: ");
		scanf("%s", buf); //사용자로 부터 AT 명령어를 입력 받음
		serialWriteBytes(fd_serial, buf); //BT로 AT 명령어를 전송함
		printf("Response: ");
		fflush(stdout);
		delay(2000); //Wait for read
		while(serialDataAvail (fd_serial)){ //버퍼에 읽을 데이터가 있을 때까지 반복
			dat = serialRead (fd_serial); //버퍼에서 1바이트 값을 읽음
			printf ("%c", dat) ; //읽은 데이터 콘솔에 출력
			fflush(stdout);
		}
		printf("\n");
		delay (10);
	}
}
