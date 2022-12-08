#include<stdio.h>
#include<wiringPi.h>

#define TP 23
#define EP 24

int main(void)
{
	int distance=0;

	long startTime;
	long travelTime;

	if(wiringPiSetupGpio () == -1)
	{
		printf("Unable GPIO Setup"); 
		return 1;
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

		int distance = travelTime / 58;

		printf( "Distance: %dcm\n", distance);
		delay(200);
	}
}
