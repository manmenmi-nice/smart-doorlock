#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#define KILO 1000
#define MEGA 1000000
#define GIGA 1000000000

#define RANGE 1000
#define CLOCK 50

#define C 262
#define D 294
#define E 330
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
		myTone(19,D);
		delay(100);
		myTone(19,C);
		delay(200);
		myTone(19,D);
		delay(200);
		myTone(19,E);
		delay(200);
		myTone(19,E);
		delay(200);
		myTone(19,E);
		delay(400);
		
		myTone(19,D);
		delay(200);
		myTone(19,D);
		delay(200);
		myTone(19,D);
		delay(400);
		
		myTone(19,E);
		delay(200);
		myTone(19,E);
		delay(200);
		myTone(19,E);
		delay(400);
		
		myTone(19,E);
		delay(300);
		myTone(19,D);
		delay(100);
		myTone(19,C);
		delay(200);
		myTone(19,D);
		delay(200);
		myTone(19,E);
		delay(200);
		myTone(19,E);
		delay(200);
		myTone(19,E);
		delay(400);
		
		myTone(19,D);
		delay(200);
		myTone(19,D);
		delay(200);
		myTone(19,E);
		delay(300);
		myTone(19,D);
		delay(100);
		myTone(19,C);
		delay(800);
	}

	initMyTone(19,262);

	return 0;

}
