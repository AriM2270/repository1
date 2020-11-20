//code created by Chandra Kiran Saladi and modified
//https://github.com/ChandraKiranSaladi/MSP432-Ultrasonic-UART/blob/master/MSP432-Ultrasonic.c

uint32_t i;
uint32_t error = 0;

void uart(void){
    /*         UART                      */
        CS->KEY = CS_KEY_VAL;                   // Allow register access for clock system module (ie allows clock divider to work) 
        CS->CTL0 = 0;                           // Reset the parameters for CS module
        CS->CTL0 = CS_CTL0_DCORSEL_3;           
        CS->CTL1 = CS_CTL1_SELA_2 |             
                CS_CTL1_SELS_3 |                
                CS_CTL1_SELM_3;                 
        CS->KEY = 0;                            
}

void gpio(void){
    // Configure GPIO
        P1->DIR |= BIT0;                        // Set P1.0 as output for onboard LED 
        P1->OUT |= BIT0;                        // P1.0 high
        P1->OUT &= ~BIT0;                       // P1.0 low
}

void uart2(void){
    // Configure UART pins
            P1->SEL0 |= BIT2 | BIT3;                // set 2-UART pin as secondary function


        P2->DIR &= ~BIT7;            // P2.7 as input
        P2->REN |= BIT7;            // P2.7 pull down enabled
        P2->OUT &= ~BIT7;            // P2.7 initial output 0, as changing it to 1 triggers LED 

        P2->SEL0 = 0;
        P2->SEL1 = 0;

        P2->IFG = 0;
        P2->IE |= BIT7;
        P2->IES &= ~BIT7;



        // Configure UART
        EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI (UART ports) in reset mode 
        EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Force eUSCI to remain in reset
                EUSCI_B_CTLW0_SSEL__SMCLK;      // Configure eUSCI clock source to be the SMCLK
        EUSCI_A0->BRW = 78;                     
        EUSCI_A0->MCTLW = (2 << EUSCI_A_MCTLW_BRF_OFS) |
                EUSCI_A_MCTLW_OS16;

        EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize eUSCI
        EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;    // Clear eUSCI interrupt flag for RX port 
        EUSCI_A0->IE &= ~EUSCI_A_IE_RXIE;        // Disable interrupt for RX
}

void timer(void){
        TIMER_A0->CCTL[0]= TIMER_A_CCTLN_CCIE;       //Enable TimerA0 interrupt 
        TIMER_A0->CCR[0] = 1000-1;                   
        TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;                  //set TimerA0 to upmode

        __enable_irq();             // Enables interrupts for whole system 
}

int uart_puts(const char *str) //transmit UART data to the main file 
{
    int status = -1;

    if (str != '\0') {
        status = 0;

        while (*str != '\0') {
            while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

            EUSCI_A0->TXBUF  = *str; //transmit UART data 
            
            if (*str == '\n') {
                while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
                EUSCI_A0->TXBUF = '\r';
            }

            str++;
        }
    }

    return status; //returning raw ultrasonic data 
}
