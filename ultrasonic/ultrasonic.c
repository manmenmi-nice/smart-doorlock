#include "ultrasonic.h"
#include <stdio.h>
#include <pthread.h>
#include <wiringPi.h>

#define TP 23
#define EP 24

pthread_mutex_t lock_ultrasonic;

void initUltrasonic(){
    pthread_mutex_init(&lock_ultrasonic, NULL);

    if(wiringPiSetupGpio () == -1)
    {
        printf("Unable GPIO Setup");
        return NULL;
    }
    pinMode (TP, OUTPUT);
    pinMode (EP, INPUT);
}

int getDistance(){
    long startTime;
    long travelTime;

    pthread_mutex_lock(&lock_ultrasonic);

    digitalWrite (TP, LOW);
    delayMicroseconds(2);
    digitalWrite (TP, HIGH);
    delayMicroseconds(10);
    digitalWrite (TP, LOW);

    while(digitalRead(EP) == LOW);
    startTime = micros();
    while(digitalRead(EP) == HIGH);
    travelTime = micros() - startTime;

    pthread_mutex_unlock(&lock_ultrasonic);

    return travelTime / 58;
}