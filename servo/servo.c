#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#define KILO 1000
#define MEGA 1000000
#define GIGA 1000000000

#define SERVO 18

#define MULTIPLIER 1
#define RANGE 1024
#define CLOCK 50

void pwmControl(int ang){

	pinMode(SERVO, PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetRange(RANGE);

	int divisor = 19.2*MEGA/CLOCK/(RANGE);
	printf("divisor:%d\n",divisor);

	pwmSetClock(divisor);

	while(1){
		//scanf("%d", &ang);
		int duty = ang/180.*(120-18)+18;
		printf("duty: %d\n", duty);
		pwmWrite(SERVO, duty);
		break;
	}

}

int main(int argc, char** argv){

	wiringPiSetupGpio();
	pwmControl(atoi(argv[1]));

	return 0;

}
