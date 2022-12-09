#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>
#include <stdlib.h>

#define TONE 19

#define C 261
#define D 293
#define E 330
#define F 349
#define G 392
#define A 440
#define B 493
#define CC 523
#define CC2 554
#define DD 587
#define DD2 622
#define EE 659

int main(int argc, char** argv){
	wiringPiSetupGpio();
  
	softToneCreate(TONE);

	switch (atoi(argv[1])){
	case 1:
		softToneWrite(TONE, E); delay(300);
		softToneWrite(TONE, D2); delay(300);
		softToneWrite(TONE, E); delay(300);
		softToneWrite(TONE, D2); delay(300);
		softToneWrite(TONE, E); delay(300);
		softToneWrite(TONE, B); delay(300);
		softToneWrite(TONE, D); delay(300);
		softToneWrite(TONE, C); delay(300);
		softToneWrite(TONE, A); delay(300);
		break;
	case 0:
		softToneWrite(TONE, E); delay(200*2);
		softToneWrite(TONE, F); delay(100*2);
		softToneWrite(TONE, G); delay(200*2);
		softToneWrite(TONE, A); delay(200*2);
		softToneWrite(TONE, E); delay(100*2);
		softToneWrite(TONE, G); delay(400*2);
		softToneWrite(TONE, C); delay(400*2);
		softToneWrite(TONE, CC); delay(33*2);
		softToneWrite(TONE, CC); delay(33*2);
		softToneWrite(TONE, CC); delay(33*2);
		softToneWrite(TONE, B); delay(33*2);
		softToneWrite(TONE, B); delay(33*2);
		softToneWrite(TONE, B); delay(33*2);
		softToneWrite(TONE, A); delay(33*2);
		softToneWrite(TONE, A); delay(33*2);
		softToneWrite(TONE, A); delay(133*2);
		softToneWrite(TONE, E); delay(100*2);
		softToneWrite(TONE, G); delay(400*2);
		softToneWrite(TONE, F); delay(400*2);
		break;
	default:
		break;
	}
  
	softToneWrite(TONE, 262);

	return 0;

}
