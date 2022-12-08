#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>
#include <stdlib.h>

#define TONE 19

#define C 523
#define C2 554
#define D 587
#define D2 622
#define E 659
#define F 349
#define G 392
#define A 440
#define B 493

int main(int argc, char** argv){
	wiringPiSetupGpio();
  
	softToneCreate(TONE);

	switch (atoi(argv[1])){
	case 0:
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
	case 1:
		break;
	default:
		break;
	}
  
	softToneWrite(TONE, 262);

	return 0;

}
