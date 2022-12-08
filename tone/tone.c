#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#define KILO 1000
#define MEGA 1000000
#define GIGA 1000000000

#define RANGE 1000
#define CLOCK 25

#define C 523
#define C2 554
#define D 587
#define D2 622
#define E 659
#define F 349
#define G 392
#define A 440
#define B 493

void initMyTone(int gpio, int freq){
	pinMode(gpio, PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetRange(RANGE);

	int divisor = 19.2*MEGA/freq/(RANGE);
	printf("divisor:%d\n",divisor);

	pwmSetClock(divisor);
	pwmWrite(gpio, 0);
}

void myTone(int gpio, int freq){
	int divisor = 19.2*MEGA/freq/(RANGE);
	printf("divisor:%d\n",divisor);

	pwmSetClock(divisor);
	pwmWrite(gpio, 50);
}

int main(int argc, char** argv){

	wiringPiSetupGpio();
	initMyTone(19,E);


	{
		myTone(19,E);
		delay(300);
		myTone(19,D2);
		delay(300);
		myTone(19,E);
		delay(300);
		myTone(19,D2);
		delay(300);
		myTone(19,E);
		delay(300);
		myTone(19,B);
		delay(300);
		myTone(19,D);
		delay(300);
		myTone(19,C);
		delay(300);
		myTone(19,A);
		delay(300);
	}

	initMyTone(19,262);

	return 0;

}
