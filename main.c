#include "msp.h"
#include "stdint.h"
#include "i2c.h"
#include "pca9685.h"
#include "servo.h"
#include "stIMU.h"
#include "stdio.h"

int miliseconds = 0;
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

        //        P2->IE &= ~BIT7;          // disable interupt
        P2->DIR |= BIT6;          // trigger pin as output
        P2->OUT |= BIT6;          // generate pulse
        Delay(100);               // for 10us
        P2->OUT &= ~BIT6;         // stop pulse
        //        P2->IE |= BIT7;          // disable interupt
        P2->IFG = 0;              // clear P2 interrupt just in case anything happened before
        P2->IES &= ~BIT7;         // rising edge on ECHO pin
        Delay(30000);             // delay for 30ms (after this time echo times out if there is no object detected)
        distance = sensor/58;     // converting ECHO lenght into cm
        char buffer[50];
        sprintf(buffer,"distance is %d\n",distance);
        uart_puts(buffer);
        if(distance < 300 && distance != 0)
            P1->OUT |= BIT0;  //turning LED on if distance is less than 20cm and isn't 0
         //TurnRight
        while(state == 4){
            servo_write(ULL,360);
            servo_write(ULA,-360)
        }
        else
            P1->OUT &= ~BIT0;

    }

}

// Timer A0 interrupt
void PORT2_IRQHandler(void)
{

    if(P2->IFG & BIT7)  //is there interrupt pending?
    {
        if(!(P2->IES & BIT7)) // is this the rising edge?
        {

            TIMER_A0->CTL |= TIMER_A_CTL_CLR;   // clears timer A
            miliseconds = 0;
            P2->IES |=  BIT7;  //falling edge
        }
        else
        {
            sensor = (long)miliseconds*1000 + (long)TIMER_A0->R;    //calculating ECHO length
            //            P1->OUT ^= BIT0;
            P2->IES &=  ~BIT7;  //falling edge

        }
        P2->IFG &= ~BIT7;             //clear flag
    }
}

void TA0_0_IRQHandler(void)
{
    //    Interrupt gets triggered on clock cycle
    miliseconds++;
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
}
