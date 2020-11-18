#include "pca9685.h"
#include "math.h"

/*
 * pca9685.c
 *
 *  Created on: Jun 22, 2020
 *      Author: Tyler
 
        modified by Michael Pogrebitskiy
 */



//this function converts degrees to a count out of 4095
uint16_t degreeConv(int degrees){
    if(degrees < 0) degrees = 0;
    if(degrees >180)degrees = 180;

    return round(1.97*degrees + 123);
}

void servo_write(uint8_t servo, uint8_t degrees){

    uint16_t count = degreeConv(degrees);

    uint8_t countH = count >> 8;
    uint8_t countL = count & LOW_MASK;

    switch(servo){
            
    case 0:
        servo = PCA_SERVO0_BASE; //Depending on the servo chosen, sets the servo to the correct BASE.
        break;
    case 1:
        servo = PCA_SERVO1_BASE;
        break;
    case 2:
        servo = PCA_SERVO2_BASE;
        break;
    case 3:
        servo = PCA_SERVO3_BASE;
        break;
    case 4:
        servo = PCA_SERVO4_BASE;
        break;
    case 5:
        servo = PCA_SERVO5_BASE;
        break;
    case 6:
        servo = PCA_SERVO6_BASE;
        break;
    case 7:
        servo = PCA_SERVO7_BASE;
        break;
    case 8:
        servo = PCA_SERVO8_BASE;
        break;
    case 9:
        servo = PCA_SERVO9_BASE;
        break;
    case 10:
        servo = PCA_SERVO10_BASE;
        break;
    case 11:
        servo = PCA_SERVO11_BASE;
        break;
    case 12:
        servo = PCA_SERVO12_BASE;
        break;
    case 13:
        servo = PCA_SERVO13_BASE;
        break;
    case 14:
        servo = PCA_SERVO14_BASE;
        break;
    case 15:
        servo = PCA_SERVO15_BASE;
        break;
    default:
        assert(false);
    }

    uint8_t info[2]; //creates an array to hold corresponding values

    info[0] = servo;
    info[1] = 0x00;

    i2c_start(EUSCI_B0, PCA_ADDRESS, WRITE, info, 2, 0x00); 

    servo++; //sets to next servo

    info[0] = servo;
    info[1] = 0x00;

    i2c_start(EUSCI_B0, PCA_ADDRESS, WRITE, info, 2, 0x00);

    servo++;

    info[0] = servo;
    info[1] = countL;

    i2c_start(EUSCI_B0, PCA_ADDRESS, WRITE, info, 2, 0x00);

    servo++;

    info[0] = servo;
    info[1] = countH;

    i2c_start(EUSCI_B0, PCA_ADDRESS, WRITE, info, 2, 0x00); 

}

}


