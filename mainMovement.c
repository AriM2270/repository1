#include "pca9685.h"
								//used code from Tyler, in the IMU_Testbench code posted on canvas.								//modified by Michael Pogrebitskiy

volatile int state = 0;


void main(void)
 {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    //set up Button Interrupt
    P1->DIR &= ~BIT1;
    P1->OUT |= BIT1;
    P1->REN |= BIT1;
    P1->IES |= BIT1;
    P1->IFG = 0;
    P1->IE  |= BIT1;

    //enable interrupts
    NVIC_EnableIRQ(PORT1_IRQn);
	__enable_irq();

    I2C_OPEN_STRUCT_TypeDef i2c_open_struct;

    i2c_open_struct.uca10 = 0;                              //no self 10-bit Address
    i2c_open_struct.ucsla10 = 0;                            //no 10-bit Addressing
    i2c_open_struct.ucmm = 0;                               //No Multi-Master
    i2c_open_struct.ucmst = EUSCI_B_CTLW0_MST;              //Master Mode
    i2c_open_struct.ucsselx = EUSCI_B_CTLW0_SSEL__SMCLK;    //SMCLK to be selected (3MHz)
    i2c_open_struct.ucbrx = 30;                             //Prescaler for Selected Clock.
                                                            //(100kHz)

    //This sets up the I2C driver to operate with the
    //correct settings.
    //EUSCI_B0 uses P1.7 as SCL and P1.6 as SDA
    i2c_open(EUSCI_B0, &i2c_open_struct);

    int16_t mx, my, mz;
    uint8_t data;
    int i;

    while(state == 0){
        for(i=0; i<200000; i++);
        i2c_start(EUSCI_B0, LIS3MDL_MAG_I2C_ADDRESS, READ, &data, 1, LIS3MDL_MAG_STATUS_REG);
    }

    //Standing Position
    while(state == 1){
        servo_write(URL,90-45); //Because the Orientations are opposite of the other servos,
        servo_write(LRL,90-45); //the direction the servo must move to stand up is also opposote
        servo_write(URA,90+45);
        servo_write(LRA,90+45);
        servo_write(ULL,90+45);
        servo_write(LLL,90+45);
        servo_write(ULA,90-45); //See Above Comment
        servo_write(LLA,90-45);*/

    }				//I looked at how Tyler coded the standing position above, and tried to form the moving positions based on
				//the logic from the above code.

    //Moving Position1
    while(state == 2){		//coded from here down. Logic comes from to move,	    		
        servo_write(URL,-360);	//the front left leg must move simultaneously with the back right leg.
        servo_write(LRL,-360);	//This portion moves the front left and back right leg.
        servo_write(URA,360);
        servo_write(LRA,360);
        state = 3;		//After this state, it changes to state 3 to continue moving.

    }

    //Moving Position2
    while(state == 3){
        servo_write(ULA, -360);
        servo_write(LLA, -360);
	servo_write(ULL, 360); //This state moves the front right leg, and the back left leg together.
        servo_write(LLL, 360);
        state = 2;		//Changes the state to state 2, to continue moving forward.
    }

    //TurnRight
    while(state == 4){		//This state turns right by moving the front left leg, and the back left leg, while keeping the rest stationary.
        servo_write(URL, -360);	
        servo_write(LRL, -360);
	servo_write(ULA, 360);
	servo_write(LLA, 360);

    }

}


