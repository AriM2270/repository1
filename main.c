//code created by Chandra Kiran Saladi and modified 
//https://github.com/ChandraKiranSaladi/MSP432-Ultrasonic-UART/blob/master/MSP432-Ultrasonic.c

#include "msp.h"
#include "stdint.h"
#include "i2c.h"
#include "pca9685.h"
#include "servo.h"
#include "stIMU.h"
#include "stdio.h"

int mili = 0;
int distance = 0;
long sensor = 0;


void initiateTrigger(void){

}

static void Delay(uint32_t loop)
{
    volatile uint32_t i;

    for (i = 0 ; i < loop ; i++);
}

int main(void)
{

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
            WDT_A_CTL_HOLD;
    uart();
    gpio();
    uart2();
    timer();
    uart_puts(const char *str)

    while(1){

        P2->DIR |= BIT6;          // set 2.6 as an output to trigger must use 2.6 and 2.7 because they're the eUSCI ports (ie the UART ports) 
        P2->OUT |= BIT6;          // generate the output trigger 
        Delay(100);               
        P2->OUT &= ~BIT6;         // stop the output trigger 
        P2->IFG = 0;              // clear P2.6 interrupt
        P2->IES &= ~BIT7;         // P2.7 gets set low 
        Delay(30000);             
        distance = sensor/58;     // convert ECHO into cm
        char buffer[50];
        uart_puts(buffer);
        if(distance < 300 && distance !=< 0)
            P1->OUT |= BIT0;  //turning LED on if distance is less than 20cm and distance isn't 0 or negative
                //TurnRight
                while(state == 4){
                    servo_write(ULL,360);
                    servo_write(ULA,-360)
        else
            P1->OUT &= ~BIT0;

    }

}

void PORT2_IRQHandler(void)
{

    if(P2->IFG & BIT7)  //check for interrupt from P2.7 
    {
        if(!(P2->IES & BIT7)) //if we're on the falling edge of the P2.7 interrupt
        {

            TIMER_A0->CTL |= TIMER_A_CTL_CLR;   // clear timer A
            mili = 0;
            P2->IES |=  BIT7; 
        }
        else
        {
            sensor = (long)mili*1000 + (long)TIMER_A0->R;    //calculate ECHO length
            P2->IES &=  ~BIT7; //set it low again 

        }
        P2->IFG &= ~BIT7;             //clear interrupt flag
    }
}

void TA0_0_IRQHandler(void)
{
    //trigger the Timer A interrupt on clock
    mili++;
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
}
