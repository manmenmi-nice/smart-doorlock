#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <wiringSerial.h>

void open_door();  // open door
void close_door(); // close door
void set_done(int value);
int get_done();

int music(int stat);
void door(int stat);
void lock(int stat);
void oled(int stat); // open : 2, close : 1
void* ultraSonic(void* argv);
void* cds(void* argv);
void* bluetooth(void* argv);
void init();

int done;
pthread_mutex_t lock_done;
pthread_mutex_t lock_door;

pthread_t child_thread[3];

int brightness;
pthread_mutex_t lock_brightness;
int get_brightness();
void set_brightness(int value);
int distance;
pthread_mutex_t lock_dist;
int get_distance();
void set_distance(int value);

int main(){
	init();
	int rc;
	rc = pthread_create(&child_thread[0],NULL,ultraSonic,NULL);
	if(rc){
		perror("UltraSonic Thread Creation failed...\n");
		pthread_exit(NULL);
	}
	rc = pthread_create(&child_thread[1],NULL,cds,NULL);
	if(rc){
		perror("CDS Thread Creation failed...\n");
		pthread_exit(NULL);
	}
	rc = pthread_create(&child_thread[2],NULL,bluetooth,NULL);
	if(rc){
		perror("Bluetooth Thread Creation failed...\n");
		pthread_exit(NULL);
	}
	while(1){
		open_door();
		close_door();

		set_done(1);
		if(get_done() == 1) break;
	}
	for(int i=0;i<3;i++)
		pthread_join(child_thread[i], NULL);
	printf("goodbye child.\n");

	return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#define SLAVE_ADDR_01 0x48
static const char* I2C_DEV = "/dev/i2c-1";
void* cds(void* argv){
	int i2c_fd;
	int cnt = 0;
	int preVal = 0;
	int curVal = 0;
	int threshold = 150;
	int adcChannel = 0;
	if(wiringPiSetupGpio()  < 0 ){
		printf("wiringPiSetup() is failed\n");
		return NULL;
	}
	if ((i2c_fd = wiringPiI2CSetupInterface (I2C_DEV, SLAVE_ADDR_01)) < 0 ){
	    printf("wiringPi2CSetup Failed: \n");
		return NULL;
	}
	printf("I2C start....\n"); 

	while(1){
		wiringPiI2CWrite(i2c_fd, 0x40 | adcChannel);
		preVal= wiringPiI2CRead(i2c_fd);
		curVal = wiringPiI2CRead(i2c_fd);
		printf("[%d] Previous value = %d, ", cnt, preVal); 
		printf("Current value= %d, ", curVal);
		set_brightness(curVal);
		if(curVal < threshold) printf("Bright!\n");
		else printf("Dark!\n");
		delay(500);
		cnt++;
		if(get_done() == 1) break;
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#define TP 23
#define EP 24
void* ultraSonic(void* argv){
	long startTime;
	long travelTime;
	if(wiringPiSetupGpio () == -1)
	{
		printf("Unable GPIO Setup"); 
		return NULL;
	}
	pinMode (TP, OUTPUT);
	pinMode (EP, INPUT);
	for(;;)
	{
		digitalWrite (TP, LOW);
		delayMicroseconds(2);
		digitalWrite (TP, HIGH);
		delayMicroseconds(10);
		digitalWrite (TP, LOW);

		while(digitalRead(EP) == LOW);
		startTime = micros();
		while(digitalRead(EP) == HIGH);
		travelTime = micros() - startTime;

		if (travelTime >= 38000){
		    printf("out of range\n");
			delay(200);
			continue;
		}

		set_distance(travelTime / 58);
		printf( "Distance: %dcm\n", travelTime / 58);
		delay(200);

		if(get_done() == 1) break;
	}
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
	while(1){
		if(serialDataAvail (fd_serial) ){ //읽을 데이터가 존재한다면,
			dat = serialRead (fd_serial); //버퍼에서 1바이트 값을 읽음
			printf ("%c", dat);
			fflush (stdout);
			serialWrite(fd_serial, dat); //입력 받은 데이터를 다시 보냄 (Echo)
		}
		delay (10);
		if(get_done() == 1) break;
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

void open_door(){
	pthread_mutex_lock(&lock_door);
	oled(2);
	lock(1);
	door(0);
	pthread_mutex_unlock(&lock_door);
}

void close_door(){
	pthread_mutex_lock(&lock_door);
	lock(1);
	door(1);
	lock(1);
	lock(0);
	oled(1);
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
		printf("(door %d) complete\n", stat);
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
		printf("(lock %d) complete\n", stat);
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
		printf("(oled %d) complete\n", stat);
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
	pthread_mutex_init(&lock_brightness, NULL);
	pthread_mutex_init(&lock_dist, NULL);
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

void set_brightness(int value){
	pthread_mutex_lock(&lock_brightness);
	brightness = value;
	pthread_mutex_unlock(&lock_brightness);
}

int get_brightness(){
	int value = 0;
	pthread_mutex_lock(&lock_brightness);
	value = brightness;
	pthread_mutex_unlock(&lock_brightness);
	return value;
}

void set_distance(int value){
	pthread_mutex_lock(&lock_brightness);
	distance = value;
	pthread_mutex_unlock(&lock_brightness);
}

int get_distance(){
	int value = 0;
	pthread_mutex_lock(&lock_dist);
	value = distance;
	pthread_mutex_unlock(&lock_dist);
	return value;
}