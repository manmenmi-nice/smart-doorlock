#include <wiringPi.h> 
#include <stdio.h>
//Step GPIO 
//IN1(12) ~ IN4(21)

int pin_arr[4] = {12, 16, 20, 21}; 
int one_phase[8][4] = {
	{1,0,0,0}, 
	{1,1,0,0}, 
	{0,1,0,0}, 
	{0,1,1,0}, 
	{0,0,1,0}, 
	{0,0,1,1},
	{0,0,0,1}, 
	{1,0,0,1}};

int one_two_Phase_Rotate(int steps, int dir){

	if (!dir){
		for(int i = 0; i<steps; i++){
			// printf("%d\n", i);
			digitalWrite(pin_arr[0], one_phase[i%8][0]); 
			digitalWrite(pin_arr[1], one_phase[i%8][1]); 
			digitalWrite(pin_arr[2], one_phase[i%8][2]); 
			digitalWrite(pin_arr[3], one_phase[i%8][3]); 
			delay(1);
		}
	}else{
		for(int i = steps; i>0; i--){
			// printf("%d\n", i);
			digitalWrite(pin_arr[0], one_phase[i%8][0]); 
			digitalWrite(pin_arr[1], one_phase[i%8][1]); 
			digitalWrite(pin_arr[2], one_phase[i%8][2]); 
			digitalWrite(pin_arr[3], one_phase[i%8][3]); 
			delay(1);
		}
	}

	digitalWrite(pin_arr[0], 0);
	digitalWrite(pin_arr[1], 0);
	digitalWrite(pin_arr[2], 0);
	digitalWrite(pin_arr[3], 0);
  
	return 0; 

}

void one_two_Phase_Rotate_Angle(float angle, int dir){
	one_two_Phase_Rotate(angle/5.625*64,dir);
}

void init_Step()
{
	for(int i = 0 ; i< 4; i++)
	{
		pinMode(pin_arr[i], OUTPUT);
	}
}

int main(int argc, char* argv[]) {
	wiringPiSetupGpio(); /* wiringPi   */ 
	init_Step();
	int angle, dir;
	angle = atoi(argv[1]);
	dir = atoi(argv[2]);
	//printf("각도, 방향: ");
	//scanf("%d %d", &angle, &dir);
	one_two_Phase_Rotate_Angle(angle, dir);
	return 0; 
}
