# Code for ECEN2440 Final Project 
Notes for Ultrasonic Code 
Mostly leveraged from https://github.com/ChandraKiranSaladi/MSP432-Ultrasonic-UART. Uses EUSCI pins 2.7 and 2.6 with a voltage divider on 2.7 

Notes for Servo Code 
Leveraged from Tyler's IMU Testbench file uploaded to Canvas. Sends signals through I2C from pins P1.7 as SCL and P1.6 as SDA from the MSP to the PCA. Depending on the state being run, signals are sent to the different servos from the PCA with the address of the servo, as well as the number of degrees the corresponding servo is meant to move. The signals sent to the servos are applied using I2C as well.
