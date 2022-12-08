//
// Created by dfkdream on 22. 12. 9.
//

#include "cds.h"
#include <pthread.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

int brightness = 0;
pthread_mutex_t lock_brightness;

#define SLAVE_ADDR_01 0x48
static const char* I2C_DEV = "/dev/i2c-1";
int i2c_fd;

void initCDS(){
    pthread_mutex_init(&lock_brightness, NULL);
    if ((i2c_fd = wiringPiI2CSetupInterface (I2C_DEV, SLAVE_ADDR_01)) < 0 ){
        perror("wiringPi2CSetup Failed: \n");
        return NULL;
    }
}

int getBrightness(){
    int preVal, curVal;

    pthread_mutex_lock(&lock_brightness);

    wiringPiI2CWrite(i2c_fd, 0x40 | adcChannel);
    preVal= wiringPiI2CRead(i2c_fd);
    curVal = wiringPiI2CRead(i2c_fd);

    pthread_mutex_unlock(&lock_brightness);

    return curVal;
}